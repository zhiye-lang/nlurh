<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/dll/agent_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>

# AI Agent Component Library agent.dll Usage Guide

Y-Language Interface Specification

#### 1. AI Agent Basic Interfaces (Group "agent"; use `out agent` to list all sub-interfaces)

```apl
-> out agent
[agent].sub(chat ctx clr feedback set get info )
```

------

##### 1.1: AI Chat

| Interface | `agent.chat` | Send a message to the LLM and receive a reply                          |
| --------- | ------------ | --------------------------------------------------------------------- |
| Parameter1 | `text/null`        | User input text; when empty, triggers LLM with the existing context only |
| Parameter2 | `reasoning_out` (optional) | Reasoning chain output variable for deep-thinking models; ignored if omitted |
| Returns    | `str`              | Success: AI reply text; Failure: error description                    |

`agent.chat` is the core interface. Each call automatically appends the input text as a `user` message, calls the LLM, and appends the reply as an `assistant` message — preserving multi-turn conversation context.

For deep-thinking models (e.g., DeepSeek R1, QwQ), the API may return a `null` `content` field while the real reply is in the `reasoning` field. In this case, `agent.chat` automatically returns `reasoning` as the reply text. You can also capture the raw reasoning chain via the second parameter.

```y
// Basic Q&A
resp = agent.chat("Compute 12345*67890 in Y Language");

// Use with agent.ctx — no need to repeat the context
agent.ctx("system", "You are a translation assistant");
agent.ctx("user", "Hello");
resp = agent.chat("");        // Trigger LLM, no new message appended
resp = agent.chat("Goodbye"); // Append user message then trigger, context continues

// Capture the reasoning chain of a deep-thinking model
resp = agent.chat("1+1=?", think);
trace("Reply:", resp);
trace("Reasoning:", think);
```

##### 1.2: Context Management

| Interface | `agent.ctx`  | Manage conversation context by appending messages to multi-turn dialog |
| --------- | ------------ | --------------------------------------------------------------------- |
| Parameter | `role`       | Message role: `"system"` / `"user"` / `"assistant"`                   |
| Parameter | `text`       | Message content (supports `'"..."'` nested quote syntax for long text) |
| Returns   | `bool`       | Success: `true`; Failure: `false`                                     |

**Best practice for injecting knowledge in chunks:**

```y
// Method 1: Multiple calls, one piece of knowledge per call (recommended)
agent.ctx("system", "You are the Zhiye platform AI assistant");
agent.ctx("system", "ui.open(w,h,name) returns an integer handle");
agent.ctx("system", "Multiple windows must use different variables: fd1, fd2, ...");

// Method 2: Use '"..."' nested quote to inject long text in one call (supports newlines and quotes)
sys = '"You are the Zhiye platform AI assistant.

Y Language core rules:
- All statements end with ;
- Variables are assigned directly, no declaration needed
- Function definition: def name(a,b){ return a+b; }
- ui.open(w,h,name) -> fd; multiple windows use fd1, fd2...

DLL interfaces:
- ui.*: open/fill/close/gmap
- tls.*: md/b64e/stb/bts
- str.*: betweens/betweenb/replace/find
"';
agent.ctx("system", sys);
```

##### 1.3: Clear Context

| Interface | `agent.clr`  | Clear all conversation context and start a fresh session |
| --------- | ------------ | -------------------------------------------------------- |
| Parameter | none         |                                                          |
| Returns   | `bool`       | Success: `true`                                          |

```y
agent.clr();  // Clear old context before starting a new topic
```

##### 1.4: Execution Result Feedback

| Interface | `agent.feedback` | Feed the previous code execution result back to the LLM to ensure variable reuse across turns |
| --------- | ---------------- | --------------------------------------------------------------------------------------------- |
| Parameter | `text`           | Result text of the previous code execution (e.g. return value of `runstr`)                    |
| Returns   | `bool`           | Success: `true`; Failure: `false`                                                            |

`agent.feedback` stores the result in an internal buffer. On the **next `agent.chat()` call, the feedback is automatically prefixed to the user message** before being sent to the LLM. The buffer holds only the latest 1 entry — new overwrites old; it does not accumulate in context.

```y
// Feed code execution results back during a multi-turn loop
r = agent.chat($FA123467);
code = str.betweens(r, "```y", "```");
trace(code);
result = runstr(code);            // Result e.g.: {cont.return (fd1 ; fd2 ; true ; true )}
agent.feedback(result);           // Will be auto-prefixed on next chat

// Next turn, LLM will see:
// [Execution feedback] {cont.return (fd1 ; fd2 ; true ; true )}
// Draw a line in the green window
// → directly uses fd2 on the existing window, no re-creation
```

**Key properties:**
- No accumulation — at most 1 entry retained, new overwrites old
- No extra message slot — prefixed to the next user message, not a separate message
- `agent.clr()` or `agent.feedback("")` can actively clear pending feedback

---

##### 1.5: Configuration Set

| Interface | `agent.set`  | Set an AI agent configuration parameter             |
| --------- | ------------ | --------------------------------------------------- |
| Parameter | `key`        | Configuration item name                            |
| Parameter | `value`      | Configuration value (string or numeric)           |
| Returns   | `bool`       | Success: `true`; Failure: `false`                  |

Supported configuration items:

| key | Type | Default | Description |
| --- | ---- | ------- | ----------- |
| api | str | `"openai"` | API type: `"openai"` / `"claude"` / `"ollama"` |
| model | str | `"gpt-4o"` | Model name |
| url | str | `"https://api.openai.com"` | API endpoint URL |
| key | str | `""` | API key |
| max_steps | int | `10` | Max consecutive conversation steps |
| timeout | int | `60000` | LLM call timeout (ms) |
| temperature | int | `70` | Sampling randomness (0~100) |
| verbose | int | `0` | `1` = print LLM request logs for debugging |

```y
// DeepSeek (OpenAI-compatible format)
agent.set("api", "openai");
agent.set("model", "deepseek-chat");
agent.set("url", "https://api.deepseek.com");
agent.set("key", "sk-xxxxx");

// DeepSeek (Anthropic-compatible format)
agent.set("api", "claude");
agent.set("model", "deepseek-chat");
agent.set("url", "https://api.deepseek.com/anthropic");
agent.set("key", "sk-xxxxx");

// OpenAI
agent.set("api", "openai");
agent.set("model", "gpt-4o");
agent.set("key", "sk-xxxxx");

// Claude
agent.set("api", "claude");
agent.set("model", "claude-sonnet-4-20250514");
agent.set("key", "sk-ant-xxxxx");
agent.set("url", "https://api.anthropic.com");
```

##### 1.6: Configuration Get

| Interface | `agent.get`  | Read a specific configuration item's value     |
| --------- | ------------ | ---------------------------------------------- |
| Parameter | `key`        | Configuration item name                        |
| Returns   | `str/bool`   | Success: value; Failure: `false`               |

```y
model = agent.get("model");       // -> "deepseek-chat"
api = agent.get("api");           // -> "openai"
temperature = agent.get("temp");  // -> "70"
```

##### 1.7: Status Query

| Interface | `agent.info`  | Get all current configuration and runtime state   |
| --------- | ------------- | ------------------------------------------------ |
| Parameter | none          |                                                  |
| Returns   | `str` (JSON)  | Status info in JSON format                      |

```y
info = agent.info();
// JSON result:
// {"model":"deepseek-chat","api":"openai","url":"https://api.deepseek.com",
//  "max_steps":10,"temperature":70,"verbose":0,"msgs":3,"initialized":1}
```

------

### Typical Usage Patterns

#### Pattern 1: AI Suggestion + Y Execution

AI generates Y Language code; the Y script extracts and runs it:

```y
loadlib"dll/agent.dll";
loadlib"dll/TextFlow.dll";

agent.set("api", "openai");
agent.set("model", "deepseek-chat");
agent.set("url", "https://api.deepseek.com");
agent.set("key", "sk-xxxxx");

// Inject rules
agent.clr();
agent.ctx("system", '"You are the Zhiye platform AI assistant. Wrap Y code between ```y and ```."');

// Ask
agent.ctx("user", "Use ui.open to create a 400x300 red window");
resp = agent.chat("");

// Extract and execute
code = str.betweens(resp, "```y", "```");
if(code) {
    runstr(code);
}
```

#### Pattern 2: Multi-turn Conversation

Context is automatically maintained across consecutive turns:

```y
agent.clr();
agent.ctx("system", "You are a translation assistant. Translate the user's input into English.");

agent.ctx("user", "Hello");
r1 = agent.chat("");     // "Hello"
agent.ctx("user", "Nice weather today");
r2 = agent.chat("");     // "The weather is nice today."
agent.ctx("user", "Goodbye");
r3 = agent.chat("");     // "Goodbye."
```

#### Pattern 3: Full Y-Orchestrated Workflow

```y
loadlib"dll/agent.dll";
loadlib"dll/TextFlow.dll";
loadlib"dll/ui.dll";

agent.set("api", "openai");
agent.set("model", "deepseek-chat");
agent.set("url", "https://api.deepseek.com");
agent.set("key", "sk-xxxxx");

// Inject full knowledge
agent.clr();
agent.ctx("system", '"You are the Zhiye platform AI assistant. Wrap all Y code between ```y and ```."');
agent.ctx("system", "ui.open(w,h,name)->fd, ui.fill(fd,0xRRGGBB,0), ui.close(fd)");
agent.ctx("system", "Multiple windows must use different variables: fd1, fd2...");

// Multi-turn dialog
agent.ctx("user", "Create a red 400x300 window");
r1 = agent.chat("");
code1 = str.betweens(r1, "```y", "```");
if(code1) runstr(code1);

agent.ctx("user", "Also create a blue 300x200 window");
r2 = agent.chat("");
code2 = str.betweens(r2, "```y", "```");
if(code2) runstr(code2);
```

------

### Notes

1. **agent.dll only handles LLM communication**; all orchestration logic is done in Y Language. It does not include a built-in system prompt, does not loop the LLM, and does not extract Y code blocks.
2. **Context lives on the C heap** — it does not consume platform 4096-node slots. Each message is stored via `agent.ctx()` and can be cleared with `agent.clr()`.
3. **Long text input**: use the `'"..."'` nested-quote syntax to avoid string-concatenation issues and escape-quote hassle.
4. **API key security**: agent.dll holds the API key in process memory until unloaded; it is cleared automatically on exit.
5. **Node consumption**: each `agent.chat()` call consumes 1 node for the returned reply text (size = reply length).
6. **Multi-turn limit**: LLM context windows are finite (typically 4K~128K tokens); `max_steps` controls the max consecutive conversation steps.
7. **Network dependency**: agent.dll uses WinHTTP for remote API calls — network connectivity is required.

------

### Runnable Example: `Yagent.c`

A complete, runnable Y-orchestrated demo is shipped in `nlu_run_64bit/Yagent.c`. It pre-loads `agent.dll`, configures the LLM, injects the full Y-Language knowledge base as a system prompt, and registers a callback that runs each LLM reply as Y code.

**Run it:**

```cmd
_zhiye.exe 1234 Yagent.c
```

**Key structure of `Yagent.c`:**

1. **Configuration** — set `api` / `model` / `url` / `key` / `temperature`
2. **System prompt** — a long `"..."` block containing:
   - Y Language core rules
   - Built-in function reference
   - DLL interface cheat sheets (ui, tls, str, json, uart, tts, cmd)
   - Critical-section drawing flow (lockwd / wdpoint / wdline / wdfill / unlockwd)
   - Output rule: wrap Y code in ` ```y ` ... ` ``` `
   - Handle-naming convention (`fd1, fd2, ...`)
3. **Callback registration** — a `<tag>...<\tag>` block tells the UI chat window to forward user input to `agent.chat()`, then extract & execute the Y code in the reply:

```c
/* Zhiye UI chat-window callback
<tag>FA123467<\tag>
<run>
{
    code = str.betweens(agent.chat($FA123467), "```y", "```");
    trace(code);
    result = runstr(code);
    agent.feedback("Code executed, return: " + result);
    return result;
}
</run>
*/
```

This way, in the Zhiye chat window, every user message becomes a Y-code-generation request: the LLM replies with code, the script extracts & runs it, and the result is fed back to the LLM for the next turn.

**Required DLLs** (place in `nlu_run_64bit/dll/`):
- `agent.dll` — LLM communication
- `ui.dll` — window/UI
- `TextFlow.dll` — string utilities (`str.betweens`, `str.find`, etc.)

**Required runtime files** in `nlu_run_64bit/`:
- `_zhiye.exe`
- `Yagent.c` (this file)
- `支叶agent.cmd` (optional Windows shortcut)

------

**Base Code for C Program Reference Definitions**: [Natural Language Understanding NLU - Third-party Function Access](https://gitee.com/kebo521/nlu3)
