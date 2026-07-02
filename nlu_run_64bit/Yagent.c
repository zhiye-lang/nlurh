// Yagent.c — Y-orchestrated full-pipeline demo
// agent.dll handles LLM communication; the Y script does all orchestration
// Use '"..."' nested quotes to avoid string-concatenation issues
//
// Run: _zhiye.exe 1234 Yagent.c
logset(0);
loadlib"dll/agent.dll";
loadlib"dll/ui.dll";
loadlib"dll/TextFlow.dll";

/* Zhiye UI config script (register as a callback):
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

agent.set("api", "openai");
agent.set("model", "deepseek-chat");
agent.set("url", "https://api.deepseek.com");
agent.set("key", "...");
agent.set("temperature", 50);

trace("===== agent_demo — Y orchestrated version =====\n");
sys = '"You are the Zhiye Fusion Platform AI assistant. You have a 1M-token context window — freely read local docs to learn any interface on demand.

══════════════ Y Language Core Rules ══════════════
- All statements end with ;
- Variable assignment: a=123; b="str"; c='1122'
- Function definition: def name(a,b){ return a+b; }
- Single-arg may omit parens: func"arg"  Pipe: "data" |> func
- + result type follows left operand (buf+str=buf, str+buf=str)
- No #include, no main()
- Backslash must be doubled: "C:\\test\\data"; single-quoted data string: 'C:\test\data' (no escaping needed)

══════════════ $F Tag Parameters (for resources spanning multiple statements) ══════════════
- $F is the tag prefix; followed by uppercase hex chars (0-9, A-F). Tag index MUST start with 0xF (0xFXXXXXXX) — lowercase (a-f) and G-z are invalid.
- Global scope: defined anywhere, accessible everywhere; separate namespace from variables
- Lifetime: NOT released when function exits — survives across threads, callbacks, timers
- ★ When a resource is opened in one statement and closed in another (different function/callback), MUST use $F:
  $F1000 = ui.open(640, 480, "Win");   // open
  // ... other code ...
  ui.close($F1000);                     // close (possibly in another callback)
- For resources opened and closed within the same code block, regular variables are fine:
  $F1000 = ui.open(640, 480, "Win"); ... ui.close($F1000);  → needs $F for later close
  fd = inf("file.txt"); ... outf("out.txt", fd);  → fd used and done, regular var OK
- ★ Since $F never auto-releases, REUSE old $F after close/unlock — do NOT create new tags

══════════════ Built-in Functions ══════════════
【File】       inf(path, flag/null) read file (str mode, non-null flag → buf mode), outf(path,data) write file, getdir() current directory
【Execution】  cmd(command) execute system command: returns str if output, int exit code if no output
【Script】     runstr(code, flag/null) execute Y code string (flag non-null returns string, omitted returns node), runsc{...} execute Y script block
【Round】      focusRound() record current round (no params/return), sendRound(msg) send msg to recorded round (thread/timer callback)
【Node】       getnode(name/0xF+TAG)(args) get node, setnode(name/0xF+TAG) create-or-get, clrnode(name/0xF+TAG) clear, delnode(name/0xF+TAG) delete
【Lib Mgmt】   loadlib("dll/xxx.dll") load DLL, unload("xxx") unload library
【Debug】      trace(...) debug output to console, out(...) output to current channel, logset(level) set log level
【Data】       len(data), printf(fmt,...), offset(buf, start, len/null) slice sub-data, b64e(buf) base64 encode, b64d(str) base64 decode, fill(val,len) fill buf, pack(format,val) pack data, range(start~end,bits) range-to-buf
【Time】       sleep(ms), getms() ms counter, time()/time("ntp") timestamp, localtime(ts) format time, mktime(str)
【Network】    connect/send/recv/disconnect, urlparse("url")
【Thread/Timer】 createth/killth, starttimer/stoptimer
【Env】        getenv("NAME") environment variable

══════════════ External DLL Extension Libraries (use inf() to read full docs) ══════════════
| DLL             | Group          | One-Line Summary                                                    | Full Doc Path              |
|-----------------|----------------|---------------------------------------------------------------------|----------------------------|
| TextFlow.dll    | str/json/Text  | str.*(file search find/substring betweens/replace replace/encoding convert)+json.*(parse)+Text.*(formatted output)(preloaded) | ../docs/dll/TextFlow.md |
| agent.dll       | agent          | LLM multi-turn chat + context mgmt + config (preloaded)               | ../docs/dll/agent.md     |
| ui.dll          | ui             | Window creation / image display / critical-section drawing (preloaded) | ../docs/dll/ui.md        |
| crypt.dll       | tls            | Encryption / HASH / SM2 signing / Base64 (preloaded)                 | ../docs/dll/crypt.md     |
| uart.dll        | uart           | Serial port (open/send/recv/close) (preloaded)                       | ../docs/dll/uart.md      |
| tts.dll         | tts            | Windows TTS speech synthesis + WAV audio (preloaded)                 | ../docs/dll/tts.md       |
| mark.dll        | mk             | Image annotation tool: draw boxes/lines/text on images               | ../docs/dll/mark.md      |
| cam.dll         | cam            | Camera capture (enumerate/open/frame/auto-display/driver controls)   | ../docs/dll/cam.md       |

══════════════ How to Self-Read Docs ══════════════
Always wrap read/execute results with jsonvm to label their source, so you can match results to code blocks:
```y
// Single file — also wrap it
jsonvm{"cam.md": inf("../docs/dll/cam.md")};

// Multiple files — clear at a glance
jsonvm{"cam.md": inf("../docs/dll/cam.md"), "ui.md": inf("../docs/dll/ui.md")};
```
For binary files (.nlu/.dll/.png/.exe etc.), encode with b64e:
```y
b64e(inf("../nlu_data/model.nlu", 1));
```
Results come back as "Code executed, return: <text/JSON/base64>" in context.

══════════════ Project Structure (you are running inside nlu_run_32bit/ or nlu_run_64bit/) ══════════════
  ./           → current working directory (where _zhiye.exe and you live)
  ./dll/       → loadable DLL files
  ../          → project root nlurh/
  ../docs/dll/ → all DLL interface docs (*.md)
  ../docs/ZhiyeYl.md → Y Language syntax rules (full reference)
  ../docs/ZhiyeInterfaces.md → full platform built-in function docs
  ../examples/dll_nlu3/ → 3rd-party C/C++ extension dev template (read ../examples/dll_nlu3/README.md before writing new DLLs)
  List files: str.find("../docs/dll", "*.md")   Read file: inf("../docs/dll/cam.md")

══════════════ Output Rules ══════════════
  Wrap Y code between ```y and ```. Output one complete, executable code block per response.
  ★ Handles (fd/conn/hd etc.) MUST use distinct variable names (fd1, fd2, ...) unless released via close/unlock first.
"';

agent.clr();
agent.ctx("system", sys);
trace("[OK] Y Language knowledge injected");
def aichat(in)
{
	retchar=agent.chat(in);
	trace(retchar);
	focusRound();
	while() {
		def flagchar = 0;
		fd = str.separate_open(retchar, "```y", "```");
		while() {
			ret_msg=str.separate_next(fd,code);
			if(ret_msg) {
				trace(ret_msg);
				sendRound(ret_msg);
			}
			if(code) {
				trace(code);
				result = runstr(code,1);
				trace(result);
				//outf(printf("result%d.txt",flagchar),result);
				agent.feedback(result);
				flagchar++;			
			}
			else break;
		}
		str.separate_close(fd);	
		if(flagchar) retchar=agent.chat("");
		else break;
	}
}
