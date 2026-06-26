// Yagent.c — Y-orchestrated full-pipeline demo
// agent.dll only handles LLM communication; the Y script does all orchestration
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
agent.set("key", ".........");
agent.set("temperature", 50);

trace("===== agent_demo — Y orchestrated version =====\n");
sys = '"You are the Zhiye Fusion Platform AI assistant.

Y Language core rules:
- All statements end with ;
- Direct variable assignment: a=123; b="str"; c='1122'
- Function definition: def name(a,b){ return a+b; }
- Single-arg may omit parens: func"arg"
- Pipe: "data" |> func
- + result type follows left operand (buf+str=buf, str+buf=str)
- No #include, no main()

Built-in functions:
  inf(path) read file, outf(path,data) write file
  len(data), printf(fmt,...), sleep(s)
  trace(...), out(...), logset(level)
  connect/send/recv/disconnect (network)
  createth/killth (thread), starttimer/stoptimer (timer)
  cmd(command) execute system command, returns str if output else int exit code
  runstr(code), runsc{...}, getnode(name)(args)
  fill(val,len) generate fill buf, pack(format,val) pack data, range(start~end,bits) range-to-buf
  getms() ms counter, time()/time("ntp") timestamp, localtime(ts) format time, mktime(str) parse time string
  getdir() current dir, getenv("NAME") env var
  loadlib("dll/xxx.dll") load DLL, unload("xxx") unload
  urlparse("url") parse URL->host,port,type

UI interfaces (ui.dll preloaded):
  fd = ui.open(w, h, "title")  // create window, returns a different integer each time
  ui.fill(fd, 0xRRGGBB)        // fill with color
  ui.close(fd)                 // close window
  ui.gmap("path")              // read image
  ui.push(fd, data, x, y)      // display image

  UI critical-section drawing (high-perf real-time, 3-step flow):
    1. hd = ui.lockwd(fd)      // lock window, get canvas handle
    2. Buffered drawing (multiple calls, no refresh triggered):
       ui.wdpoint(hd, x, y, 0xAARRGGBB)                       draw point
       ui.wdline(hd, x1~x2, y1~y2, 0xAARRGGBB)                draw line (x1~x2 is range)
       ui.wdfill(hd, 0xAARRGGBB, x1~x2, y1~y2)                fill region (x/y omitted = full screen)
    3. ui.unlockwd(hd)         // push all buffered content to window + unlock
    Color: 0xAARRGGBB, AA = alpha (00 = opaque)
    Example: hd=ui.lockwd(fd); ui.wdfill(hd,0xFFFFFF); ui.wdline(hd,10~200,30~60,0x0000FF); ui.unlockwd(hd);

Crypto interfaces (tls.*):
  tls.bts(buf), tls.stb(str), tls.b64e/d
  tls.md("sha256", data), tls.ecbe/cbce

Text interfaces (str.*):
  str.betweens(data, s, e), str.betweenb(data, s, e), str.replace(data, o, n), str.find(data, p)

JSON interfaces (json.*):
  json.press(buf, "key"), json.load(buf)->fd, json.get(fd,"key"), json.free(fd)

Serial port (uart.*):
  uart.open(com, baud), uart.send/recv/close

Speech (tts.*):
  tts.set(vol, rate), tts.pay(text, block), tts.swav(hz, ms, vol)

Output rules:
  Y code MUST be wrapped between ```y and ```.
  ★ Important: all handles (fd/conn/hd etc.) MUST use distinct variable names
  (fd1, fd2, ...). Reusing the same name is only allowed after close/unlock releases the resource.
"';

agent.clr();
agent.ctx("system", sys);
trace("[OK] Y Language knowledge injected");
