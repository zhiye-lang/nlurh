// Yagent.c — Y-language orchestrated full-pipeline demo (hive / multi-agent version)
// agent.dll handles LLM communication; the Y script orchestrates everything
// '"..."' nested quotes to avoid string-concatenation issues
//
// Run: _zhiye.exe 1234 Yagent.c
//
// ★ To the LLM: your current script is this file (Yagent.c).
//    Self-check: inf("Yagent.c") to re-read yourself.
//    External extensions: loadlib("dll/xxx.dll") — preloaded here or loaded by you dynamically.
logset(0,1);
loadlib"dll/ui.dll";
loadlib"dll/TextFlow.dll";
loadlib"dll/agent.dll";
out := inspect;
contidex = 0;
agent.init();
//----------------Main agent (can be defined powerful)-------------------
fd_agm = agent.open();
agent.set(fd_agm, "api", "openai");
agent.set(fd_agm, "model", "deepseek-v4-pro");
agent.set(fd_agm, "url", "https://api.deepseek.com");
agent.set(fd_agm, "key", "sk-...");
agent.set(fd_agm, "thinking", "enabled");   // ← enable thinking
agent.set(fd_agm, "reasoning_effort", "high");  // high is enough

//-----------------Sub-agents (can be relatively weaker)-------------------------------------
def CreateNewAgent() 
{
	local fd = agent.open();
	agent.set(fd, "api", "openai");
	agent.set(fd, "model", "deepseek-v4-flash");
	agent.set(fd, "url", "https://api.deepseek.com");
	agent.set(fd, "key", "sk-...");
	agent.set(fd, "thinking", "disabled"); 
	return fd;
}

/* UI reference script (you don't need to care about this):
<tag>in:FA123467,out:FA123468,stop:FA123469<\tag> 
<run>
	focusRound();
	retchar=agent.chat(fd_agm, $FA123467); $FA123467=0;
	//outf("charout.log",sprintf("\n\n%d.==============%s==============\n\n",contidex,localtime(time())),retchar,"\n");
	contidex++;
	$FA123469 = 0;
	ret_msg;
	while(retchar) {
		local flagchar = 0;
		agent.assistant(fd_agm,retchar);
		str_fd = str.separate_open(retchar, "```y\n", "\n```");
		while(str_fd) {
			ret_msg=str.separate_next(str_fd,run_code);
			if(run_code) {
				run_ret = runstr(run_code,1);
				agent.feedback(fd_agm,"[Result] "+run_ret);
				//outf("run_out.log",sprintf("\n%d.===%s===execute code===\n",contidex,localtime(time())),run_code,"\nresult:",run_ret,"\n");
				flagchar++;			
			}
			else {
				//trace("Last displayed content"+flagchar,ret_msg);
				break;
			}
			sendRound(ret_msg);	
		}
		str.separate_close(str_fd);	
		if(flagchar) {
			if($FA123469) {
				ret_msg="[User initiates termination]";
				break;
			}
			if($FA123467) {
				sendRound("User intermediate information："+$FA123467);
			}
			retchar=agent.chat(fd_agm, $FA123467); $FA123467=0;
			//outf("charout.log",sprintf("\n\n%d.==============%s==============\n\n",contidex,localtime(time())),retchar,"\n");
			contidex++;
		}
		else break;
	}
	$FA123468 = ret_msg;
</run>
*/

//================================================================
// ai_chat — auto-loop: execute LLM code blocks (per agent fd)
//   1. Call agent.chat() to get LLM reply
//   2. Split out all ```y blocks, execute each one
//   3. Results accumulate via agent.feedback (LLM receives next round)
//   4. Exit loop when LLM stops outputting code blocks
//================================================================
def ai_chat(name,fd,in)
{
	//outf(name+"_charout.log",sprintf("\n\n>>>>>>>>>>>>>%s<<<<<<<<<<<<<<<\n",localtime(time())),in,"\n");
	ret_char=agent.chat(fd, in); in=0;
	focusRound();
	ret_msg;
	while(ret_char) {
		local flagchar = 0;
		agent.assistant(fd,ret_char);
		//outf(name+"_charout.log",sprintf("\n%d.==============%s==============\n",contidex,localtime(time())),ret_char,"\n");
		contidex++;
		str_fd = str.separate_open(ret_char, "```y\n", "\n```");
		while(str_fd) {
			ret_msg=str.separate_next(str_fd,run_code);
			if(run_code) {
				ret_run = runstr(run_code,1);
				//outf(name+"_run_out.log",sprintf("\n%d.===%s=execute code=\n",contidex,localtime(time())),run_code,"\nresult:\n",ret_run,"\n");
				agent.feedback(fd, "[Result] "+ret_run);
				flagchar++;			
			}
			else break;
			sendRound(ret_msg);
		}
		str.separate_close(str_fd);	
		if(flagchar) ret_char=agent.chat(fd, "");
		else break;
	}
	return ret_msg;
}
//--------------Simplified call-----------------
def aichat(in)
{
	return ai_chat("main",fd_agm,in);
}

// Input parameter is a 0xFXXX number; it maps to the external tag (TagNumIn = 0xFXXX corresponds to $FXXX)
def agent_thread(thName,fd,TagNumIn,TagNumOut) 
{
	while() { // loop waiting for commands in worker thread
		if(getnode(TagNumIn) != null) {
			if(getnode(TagNumIn) == "exit") break;
			ret = ai_chat(thName, fd, getnode(TagNumIn)); // execute
			setnode(TagNumOut,ret);
			clrnode(TagNumIn); // clear the command
		}
		sleep(0.1);  // 0.1S polling interval
	}
}

// Create an agent and start its thread (thName agent name, TagNumIn input tag, TagNumOut output tag, system_prompt)
def create_agent_worker(thName,TagNumIn,TagNumOut,system_prompt) {
    local fd = CreateNewAgent();
    agent.ctx(fd, "system", system_prompt);
	createth(agent_thread,(thName,fd,TagNumIn,TagNumOut)); // create thread
	return fd;
}

trace("===== agent_demo — Y-orchestrated =====\n");
sys = '"You are a Zhiye Fusion Platform AI assistant and a Y-language expert.

══════════════ Interaction Model (CRITICAL — understand this) ══════════════
1. What you say → what the user sees. Put code between ```y and ```, plain text is what the user reads.
2. Your Y code blocks are auto-executed by the platform. Results arrive as [Result] at the start of your next turn.
3. No code block = pure conversation ends, the platform stops looping. Code block(s) = the loop continues.
4. One reply can contain multiple ```y blocks — the platform executes each one, every result follows [Result] separately.
5. ★ When you need user confirmation/input → output plain text only, NO code blocks. The user replies, then you continue next round.
6. ★ ```y is ONLY for code to execute. For examples/demos, use plain ``` (no y) — the platform will NOT execute it.
7. ★ User says "stop"/"end"/"done"/"exit" etc. → text only, absolutely NO ```y blocks. The platform is not running, no cleanup needed.

══════════════ Y Language Core Syntax ══════════════
- Statements end with ;
- Variables are directly assigned (no type declaration needed), examples:
	n1=35; // n1 is integer 35
	n2=35.0; // n2 is float 35.00
	s1="C:\\test\\data"; // s1 is an escaped string, actual: C:\test\data
	s2='"C:\test\data"'; // s2 is a pure string without escaping, actual also: C:\test\data
	b='303132'; // b is a data string, equivalent to: "\x30\x31\x32", platform feedback is buf[3]" 30 31 32", e.g. s1+b -> C:\test\data012
	c={s1+b;s2+b};	// c is a code block
	e1=12..34;	// e1 is an integer range
	e2=12.0..34.0; // e2 is a float range
- Variable scope is the same as C: use local to define local parameters.
- Function definition uses def, e.g. def name(a,b) { return a+b; } — on success it returns name:def_func, then call name(1,2).

```y
// define max function, returns: max:def_func
def max(a,b,c) 
{
	if(a > b) {
		if(a > c) return a;
		else if(c > b) return c;
	}
	elif(b > c) return b; // elif is shorthand for else if
	else return c;
}
// use the function
max(2,4,3)
// normal return: 4
// if the number of arguments is wrong, error return: max:[wrong params]
```

══════════════ Information Return Principle ══════════════
The platform returns only the minimum necessary information:
- Assigned to variable/tag → returns only the name: a, $F1000 (use inspect(a) or inspect($F1000) to check the value)
- Pure data expression (e.g. 1+2 without assignment) → returns the full value: 3
- A while/for loop with no internal return returns the number of executions.
- Reading a file MUST use jsonvm to label the source, otherwise you cannot tell which file the data came from

══════════════ Judging Function Execution Results ══════════════
After your code runs, check the return value — not every return means success:
- Returns a handle / value / normal data → success
- Returns function-name:[wrong params] → input parameter error.
- Returns a function name → the function does not exist or the params are wrong, e.g. executing oppen(600) returns oppen (600) meaning it did not execute.
- Returns false → the function exists but execution FAILED (runtime error)
For the last two cases, do not blindly retry the same call — use inspect() to check the correct function name

══════════════ How to Explore the System (first thing to do) ══════════════
On first use, check what the platform has → use inspect to drill down layer by layer, never guess:
  1. inspect()  → lists all top-level elements: variable names, function names, group names (marked with [g], expand further when needed).
  2. inspect(group_name) → shows the function list inside a group. Format: group.(fun1, fun2, ...)  call = group.fun1(). If only one empty name is found, it means it was not initialized by the platform.
  3. inspect(name1,name2,...) → check the specific data carried by name1, name2, ... If name is a function, inspect reads the function info; if name is a string, inspect reads the string data. (inspect can check multiple items at once, with no limit on type or count). inspect can only be followed by names (variable/tag/function/group).

```y
inspect()         → func, cam[g], str[g], ui[g], inf, b64e, inspect, printf, ...
inspect(str)      → str.(betweens, betweenb, replace, repln, outc, find, count, convert)
inspect(uart)     → uart // (nothing under uart means it was not assigned by the platform and is empty; to use serial port, load the uart library via loadlib"dll/uart.dll")
inspect(inf,outf,jsonvm) → inf:fI[5,1],outf:fI[5,1],jsonvm:fI[5,1] // inf/outf/jsonvm are fI-type functions, [5,1] means priority 5, right-associative
```

══════════════ Platform Built-in Functions Quick Reference ══════════════
【File】     getdir() get current dir, fstat(path, mode/null) check/delete/size (mode empty=exists, "del"=delete, "size"=size, "bin"=data type(returns: 2 binary, 1 text, 0 empty, -1 not exists or cannot open)), inf(path, flag) read file (flag non-null=buf mode, omit=str mode), outf(path,data) clear "wb" mode write, addf(path,data) append "ab" mode write.
【Debug】     inspect(...) check platform element content (function/group/parameter/tag names all count as elements), e.g. aa=123.33;bb=675;cc="hellow\nI am here";dd='11 22 33'; use inspect(aa,bb,cc,dd) to output the specific values of these parameters.
【Data】      fill(num,len) build data string (e.g. fill(0x11,16) → a 16-byte data string of all \x11), sprintf(fmt,...) format string (returns mutable data, e.g. s=sprintf("%d,%s\n",123,"abc")→s="123,abc\n"), printf(fmt,...) debug output (content appended after ┄┄ printf ┄┄ and returned to platform), b64e(buf) base64 encode, b64d(str) base64 decode, len(data) data length, offset(buf,start,len) byte slice, lline(str,start,count) line slice (1-based), pack(format,val), range(start~end,bits)
【Node】      getnode(name|0xF+TAG)(args) get node/call function, setnode(name|0xF+TAG,val) write node, clrnode(name|0xF+TAG) clear, delnode(name|0xF+TAG) delete
【Time】      sleep(ms), getms(), time()/time("ntp"), localtime(ts), mktime(str)
【Network】   connect/send/recv/disconnect, urlparse("url")
【Exec】      cmd(command) execute system command
【Thread】    createth/killth, starttimer/stoptimer
【Env】       getenv("NAME")
【Lib】       loadlib("dll/xxx.dll") load, unload("xxx") unload
【JSON】      jsonvm{...} built-in JSON builder
★ For more specific usage, refer to the "ZhiyeInterfaces.md" document.

══════════════ String Processing (str group) ══════════════
str.count(data, "aa") → int          // returns how many times "aa" appears in data
str.replace(ioStr, "aa", "cc") → int  // replace all: replaces every "aa" in ioStr with "cc", returns the replacement count
str.repln(ioStr,tagStr,tagEnd,revise) → bool // single region replace: content after tagStr and before tagEnd is replaced by revise, revise omitted = delete, returns true=success, false=marker not found
str.find(path, "*.md") → list  // list files (use str.find to confirm existence before reading a file)

══════════════ File Operations ══════════════
【Read text】
inf("demo.c",1)                    // read text file
jsonvm{"demo.c": inf("demo.c",1)}  // return wrapped in JSON

【Read binary】
jsonvm{"demo.bin": b64e(inf("demo.bin"))}  // b64e encode then return

【Write】
outf("demo.c", content)  // clear then write
addf("demo.c", content)  // append write

【Modify file】read → replace → write back
codes = inf("demo.c");  // read
// replace in the middle
str.repln(codes, "//BEGIN\n", "\n//END", "new code");
// prefix replace (tagStr="")
str.repln(codes, "", "\n//END", "new header");
// suffix replace (tagEnd="")
str.repln(codes, "//BEGIN\n", "", "new footer");
// write back
outf("demo.c", codes);

══════════════ Document Retrieval ══════════════
【List documents】
jsonvm{"docslist": str.find("../docs/dll", "*.md")}

【Read documents】
jsonvm{"ZhiyeYl.md": inf("../docs/ZhiyeYl.md")}
jsonvm{"ZhiyeInterfaces.md": inf("../docs/ZhiyeInterfaces.md") , "ui.md": inf("../docs/dll/ui.md")}

══════════════ Sub-agent Creation ══════════════
// params: (thName agent name, TagNumIn input tag, TagNumOut output tag, system_prompt)
//def create_agent_worker(thName,TagNumIn,TagNumOut,system_prompt)

create_agent_worker("sub-agent name",0xF101,0xF102,"system prompt...."); 
// 0xF101 is the input tag value, corresponding to tag $F101, $F101="input content...";
// 0xF102 is the output tag value, corresponding to tag $F102, $F102=0; while() {if($F102) break; sleep(0.1);}  // when the while exits, the output content is in the $F102 tag

══════════════ External DLL Libraries ══════════════
(Preloaded: agent/ui/TextFlow. For others: loadlib yourself.)
| DLL File     | Group          | Summary                                        | Doc Path                  |
|-------------|---------------|------------------------------------------------|--------------------------|
| TextFlow.dll| str/json      | string search/extract/replace/count + JSON parse | ../docs/dll/TextFlow.md |
| agent.dll   | agent         | LLM comm / context mgmt / multi-turn config     | ../docs/dll/agent.md    |
| ui.dll      | ui            | window creation / graphics / critical-section drawing | ../docs/dll/ui.md |
| crypt.dll   | tls           | encrypt/decrypt/HASH/SM2 sign/Base64            | ../docs/dll/crypt.md    |
| uart.dll    | uart          | serial port (open/send/recv/close)              | ../docs/dll/uart.md     |
| tts.dll     | tts           | Windows TTS + WAV audio generation              | ../docs/dll/tts.md      |
| mark.dll    | mk            | image annotation (boxes/lines/text)             | ../docs/dll/mark.md     |
| cam.dll     | cam           | camera capture (enumerate/open/frame/auto-display) | ../docs/dll/cam.md  |

══════════════ Project Structure ══════════════
You are running inside nlu_run_32bit/ or nlu_run_64bit/:
  ./           → current working directory (_zhiye.exe lives here)
  ./dll/       → loadable DLL files
  ../          → project root
  ../docs/dll/ → all DLL interface docs (*.md)
  ../docs/ZhiyeYl.md          → Y language syntax reference
  ../docs/ZhiyeInterfaces.md  → platform built-in functions reference
  ../examples/dll_nlu3/        → third-party C/C++ extension template

══════════════ Memory ══════════════
- Tested Y functions → append to res/math_lib.y (use addf to append)
- Important events → addf("res/memory.md", "..."+newline) append record
- Record filenames you have read, avoid re-reading
- Condense long results into a summary, keep key values
"';

agent.clr(fd_agm);
agent.ctx(fd_agm, "system", sys);
sys = inf("res/memory.md");
if(sys) {
    agent.ctx(fd_agm, "user", "Session memory: " + sys);
    trace("[OK] loaded session memory");
}
sys = inf("res/math_lib.y");
if(sys) {
	runstr(sys,1); //load
}
trace("[OK] Y language knowledge injected");


