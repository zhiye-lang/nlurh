<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/%E6%94%AF%E5%8F%B6%E5%B9%B3%E5%8F%B0%E6%8E%A5%E5%8F%A3.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>
# Zhiye Platform Built-in Interface Functions

Y-Language Interface Definitions and Usage Guide



#### 1.1: Log Settings (logset)

| Interface | logset        | Set system log parameters                                    |
| --------- | ------------- | ------------------------------------------------------------ |
| Param     | level         | Log level (0=debug, 1=info, 2=warning, 3=error, 4=none)      |
| Param     | flagfile/null | Output to file flag; 1 outputs to file, 0 or absent outputs to console |
| Return    | bool          | Returns true on success                                      |

```c
bool logset(level, flagfile) // logset(2,0) -> true: output at warning level to console; logset(0,1): output at debug level to file zhiye1M_0.log 
```



#### 1.2: Get Length (len)

| Interface | len  | Get data length                     |
| --------- | ---- | ----------------------------------- |
| Param     | data | Input data                          |
| Return    | int  | Returns the byte length of the data |

```c
int len(data) // len("hello") -> 5
```



#### 1.3: Fill Data (fill)

| Interface | fill | Generate buffer data |
| --------- | ---- | -------------------- |
| Param     | val  | Target buffer value  |
| Param     | len  | Fill value           |
| Return    | buf  | Filled data          |

```c
buf fill(val, len) // fill(0x00, 4) -> ' 00 00 00 00'
```



#### 1.4: Data Packing (pack)

| Interface | pack | Pack data into a specific format                             |
| --------- | ---- | ------------------------------------------------------------ |
| Param     | flag | Pack format (">h2" big-endian 2-byte hex, "<s3" little-endian 3-byte string length, ">d4" big-endian 4-byte decimal) |
| Param     | len  | Length value                                                 |
| Return    | buf  | Formatted length buffer data                                 |

```c
buf sys.pack(format, data...) // pack(">h",14) -> ' 00 0E'
```



#### 1.5: Data Truncation (offset)

| Interface | offset               | Extract a substring/sub-buffer at a given position and length      |
| --------- | -------------------- | ------------------------------------------------------------------ |
| Parameter | str/buf              | Source data (string or buffer)                                     |
| Parameter | start                | Start position (0-based)                                           |
| Parameter | len/null             | Length to extract; if empty, extracts to the end                   |
| Returns   | str/buf              | Returns extracted data, same type as input                         |

```c
str/buf offset(str/buf, start, len/null)
// offset("abcdefg", 2, 3)    → "cde"
// offset("abcdefg", 2)       → "cdefg"    (len empty → to the end)
// offset(buf, 4, 8)          → 8 bytes from buf starting at byte 4
```

> **Vs. takat**: `takat` extracts from structured composite data (collections/ranges/modified parameters); `offset` does byte-level slicing on raw str/buf. The former destructures structured data, the latter slices raw binary.


#### 1.6: Line Slice (lline)
| Interface | lline                | Slice text by line number (for reading code, logs, etc.)     |
| --------- | -------------------- | -------------------------------------------------- |
| Param     | data                 | Source text (str)                                     |
| Param     | start                | Starting line number (1-based)                        |
| Param     | count/null           | How many lines to return; null = to end               |
| Return    | str                  | The sliced lines                                      |

```c
str lline(str, start, count/null)
// lline(code, 42)          → full text of line 42
// lline(code, 42, 5)       → lines 42~46 (5 lines total)
// lline(code, 1)           → from line 1 to end (entire file)
```
#### 1.7: Base64 Encode/Decode (b64e/b64d)

| Interface | b64e                  | Encode binary data to Base64 string                         |
| --------- | --------------------- | ----------------------------------------------------------- |
| Param     | data(buf)             | Binary data                                                 |
| Return    | str                   | Base64 encoded string                                       |

| Interface | b64d                  | Decode Base64 string to binary data                         |
| --------- | --------------------- | ----------------------------------------------------------- |
| Param     | base64_str            | Base64 encoded string                                       |
| Return    | buf                   | Decoded binary data                                         |

```c
str  b64e(buf)
buf  b64d(str)
// a = b64e(inf("img.png"))       → Base64 encoded string
// b = b64d(a)                     → original binary data
// b64e('112233')                  → "ESIz"
// b64d("ESIz")                    → ' 11 22 33'
```
#### 1.8: Range Data Generation (range)

| Interface | range | Generate buf data from range parameters            |
| --------- | ----- | -------------------------------------------------- |
| Param     | rng   | Range parameter, format: `start~end[:step]`        |
| Param     | bits  | Bit width, supports: `8`, `16`, `32`               |
| Return    | buf   | Returns generated little-endian byte sequence data |

```c
buf range(rng, bits) 
/*
-> range(1~10,8)
<- buf[10]' 01 02 03 04 05 06 07 08 09 0A'
-> range(2~10:2,8)
<- buf[5]' 02 04 06 08 0A'
-> range(1~9:2,8)
<- buf[5]' 01 03 05 07 09'
-> range(0xAA00~0xAA10,16)
<- buf[34]' 00 AA 01 AA 02 AA 03 AA 04 AA 05 AA 06 AA 07 AA 08 AA 09 AA 0A AA 0B AA 0C AA 0D AA 0E AA 0F AA 10 AA'
-> range(0x12345678~0x1234567F,32)
<- buf[32]' 78 56 34 12 79 56 34 12 7A 56 34 12 7B 56 34 12 7C 56 34 12 7D 56 34 12 7E 56 34 12 7F 56 34 12'
*/
```

`takat` — an aptly named function: "take at", meaning take + position — phonetically, semantically, and graphemically intuitive.

#### 1.9: Indexed Value Extraction (takat)

| Interface | takat       | Extract pure values from modified content                    |
| --------- | ----------- | ------------------------------------------------------------ |
| Param     | sub/data+[] | Modifier parameter, formats: `sub[1~3]`, `data[4~9]`, `sub[5]` |
| Return    | sub/data    | Returns corresponding new data (unmodified); external libraries cannot parse modifier parameters and need takat for purification. |

```c#
-> sub=[11 22 33 44 55 66 77 88 99]
<- sub
-> takat(sub[1~3])
<- [int:22 int:33 int:44 ]
-> takat(sub[1])
<- int:22
-> data="1234567890"
<- data
-> takat(data[1~3])
<- str[3]"234"
```



#### 1.10: Format String (sprintf)

| Interface | sprintf  | Format args into a string, return to variable (no output) |
| --------- | -------- | -------------------------------------------------------- |
| Param     | format   | Format string                                             |
| Param     | args,... | Parameter list                                            |
| Return    | str      | Returns the formatted string                              |

```c
str sprintf(format, args...) // a=sprintf("Value: %d", 123) → a="Value: 123"
```

> **sprintf vs printf**: `sprintf` returns the formatted string for variable assignment. `printf` is for debug output, appended after the `┄┄ printf ┄┄` marker in feedback.



#### 1.11: Debug Output (printf)

| Interface | printf   | Formatted output to platform debug channel (after ┄┄ printf ┄┄) |
| --------- | -------- | -------------------------------------------------------------- |
| Param     | format   | Format string                                                   |
| Param     | args,... | Parameter list                                                  |
| Return    | none     | Content appended to end of feedback                              |

```c
printf(format, args...) // printf("Current: %d", 123) → feedback shows: ┄┄ printf ┄┄\nCurrent: 123
```



#### 1.11: Get Milliseconds (getms)

| Interface | getms | Get system millisecond count |
| --------- | ----- | ---------------------------- |
| Param     | none  |                              |
| Return    | int   | Returns system milliseconds  |

```c
int getms() // ms = getms()
```



#### 1.12: Get Time (time)

| Interface | time     | Get current time (Unix timestamp)                            |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | str/null | NTP server address, e.g. "cn.pool.ntp.org" for China's NTP server. When absent, obtains from local clock. |
| Return    | int      | Returns timestamp                                            |

```c
int time(ntp_server) // timestamp = time(); Get Unix time from local clock; timestamp = time("cn.pool.ntp.org"); Get Unix time from China's NTP server
```



#### 1.13: Time Conversion (mktime)

| Interface | mktime   | Convert time string to timestamp                    |
| --------- | -------- | --------------------------------------------------- |
| Param     | str_time | Time data in format "%04d-%02d-%02d %02d:%02d:%02d" |
| Return    | int      | Returns timestamp                                   |

```c
int mktime(str_time) // timestamp = mktime("2025-10-14 01:02:51")
```



#### 1.14: Local Time (localtime)

| Interface | localtime | Convert timestamp to local time (inverse of mktime) |
| --------- | --------- | --------------------------------------------------- |
| Param     | timestamp | Timestamp                                           |
| Return    | str_time  | Time data in format "%04d-%02d-%02d %02d:%02d:%02d" |

```c
str_time localtime(timestamp) // str_time = localtime(ts)
```



#### 1.15: Sleep (sleep)

| Interface | sleep   | Put current thread to sleep for specified duration |
| --------- | ------- | -------------------------------------------------- |
| Param     | seconds | Sleep duration in seconds, supports decimals       |
| Return    | none    |                                                    |

```c
void sleep(s) // sleep(0.01) // Sleep for 10ms
```



#### 1.16: Pause (pause)

| Interface | pause  | Pause code execution, wait for continue instruction |
| -------- | ------ | --------------------------------------------------- |
| Param    | none   | Pause directly                                      |
| Param    | msg    | Optional string, output to debug interface on pause |
| Return   | none   |                                                     |

```c
void pause()       // pause()   Pause directly
void pause(msg)    // pause("Confirm to continue")  Pause and print prompt to debug interface
```



#### 1.17: Get Environment Variable (getenv)

| Interface | getenv   | Get environment variable value                               |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | name     | Environment variable name                                    |
| Return    | str/bool | Returns environment variable value; returns false on failure |

```c
str getenv("PATH") // path = getenv("PATH")
```



#### 1.17: Get Current Directory (getdir)

| Interface | getdir | Get current working directory  |
| --------- | ------ | ------------------------------ |
| Param     | none   |                                |
| Return    | str    | Returns current directory path |

```c
str getdir() // current_dir = getdir()
```



#### 1.18: Debug Output (trace)

| Interface | trace              | Output debug info to console (always to console in any run mode) |
| --------- | ------------------ | -------------------------------------------------------- |
| Param     | variable name/null | Variable name to output; when empty, outputs all current variables |
| Return    | none               | Outputs only to debug console, no return value; usable inside threads |

```c
trace(name,...) // ab = 20; trace(ab) / trace ab -> [ab]num:20  // Output to debug console
/*
// When no input parameter, prints all meta names (functions and parameters exist as meta elements).
-> trace() // trace with no parameters
<json><str><tls><tts><uart><ui><setfont><stoptimer><starttimer><killth><createth><disconnect><recv><send><connect><urlparse><stoptimer><starttimer><killth><createth><unload><loadlib><runstr><input><jsonvm><runsc><setfunc><inf><outf><out><getdir><getenv><sleep><localtime><mktime><time><getms><printf><offset><pack><fill><len><logset><return><last><||><&&><--><++><=<><:=><:<><^=><|=><&=><%=><-=><+=></=><*=><!=><==><<=><>=><<><>><=><^><|><&><-><+><%></><*><!><~><.>
<-
-> trace json // trace with json parameter; json is a collection imported by TextFlow.dll 
[json].sub(free get load press )
<-
-> trace ui // trace with ui parameter; ui is a collection imported by ui.dll
[ui].sub(smap gmap setdpi close cursor recovery backup get save pull fill push open )
<-
*/
```



#### 1.19: System Inspection (inspect)

| Interface | inspect            | Inspect elements inside nodes/variables/groups (function names, group names, params, tags are all elements) |
| --------- | ------------------ | ------------------------------------------------- |
| Param     | name/null          | Element name to inspect; when empty, lists all top-level elements |
| Param     | name ...           | Multiple names allowed, unlimited count and types   |
| Return    | str                | Returns inspection result as a string              |

```c
str inspect(name,...) // Inspect platform element contents
// inspect()       → list all top-level elements
// inspect(str)    → str.(betweens, betweenb, find, count, ...)
// inspect(aa,bb)  → inspect both aa and bb at once
```

> **Difference between inspect and trace**: `inspect` uses the "result channel" — returns to the caller. `trace` uses the "debug channel" — always outputs to debug console. To check variables inside threads, use `trace`.



#### 1.20: File Check / Delete / Stat (fstat)
| Interface | fstat              | File existence check / delete / stat                          |
| --------- | ------------------ | ------------------------------------------------------------ |
| Param     | path               | File path                                                     |
| Param     | mode/null          | Operation mode: null=check existence, "del"=delete, "size"=file size |
| Return    | bool/int           | Check existence returns true/false; delete returns true on success; size returns int bytes |

```c
bool fstat(path)                  // check if file exists
bool fstat(path, "del")           // delete file
int  fstat(path, "size")          // file size in bytes
// fstat("config.txt")            → true   file exists
// fstat("temp.dat", "del")       → true   deleted successfully
// fstat("data.bin", "size")      → 2048  2KB
```
#### 1.21: Output to File (outf)

| Interface | outf     | Output data to file (append mode)                                          |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | filename | File name                                                    |
| Param     | data     | Data to output                                               |
| Return    | int/bool | Returns file content length on success; returns false on failure |

```c
int outf(filename, data ...) // outf("data.bin", ' 11 22 33') -> 3 (Append 3 hex bytes to data.bin file)
```



#### 1.22: Read from File (inf)

| Interface | inf      | Read data from file                                           |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | filename | File name                                                   |
| Param     | flag/null| Read mode: omit/null = string mode; any value (e.g. 1) = binary mode |
| Return    | str/buf  | String mode returns str, binary mode returns buf             |

```c
str inf(filename)          // text mode (default)
buf inf(filename, 1)       // binary mode
// text = inf("config.ini")           → string
// bin  = inf("img.png", 1)           → binary buffer
// base64_str = b64e(inf("img.png", 1))  → binary read + base64 encode
```



#### 1.23: Set Function Priority (setfunc)

| Interface | setfunc   | Adjust function execution priority |
| --------- | --------- | ---------------------------------- |
| Param     | func_name | Function name                      |
| Param     | priority  | Priority value                     |
| Return    | bool      | Returns true on success            |

```c
bool setfunc("callback", 1) // setfunc("outf", 2)
```



#### 1.24: Get System Node (getnode)

| Interface | getnode  | Get platform internal node (parameter/function/TAG) by name or TAG index          |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | name/TAG | String node name (e.g. `"time"`, `"str"`) or numeric TAG label (`0xF` prefix, e.g. `0xFA123467`, `0xF1A`) |
| Return    | node     | Returns node handle; returns `false` if not found            |

- String input: look up regular variables, functions, and parameters by name
- `0xF` numeric input: look up TAG label nodes by hex index (e.g. `getnode(0xFA123467)` is equivalent to `$FA123467`)
- Return value can be called as a function `node(args)`, or assigned to variables

```c
node getnode(name/TAG)
// getnode("time")       → get the time function node
// getnode("time")();    → execute the platform time() function
// getnode(0xFA123467)   → get the node for tag $FA123467

// Dynamic node names — ideal for loops or config-driven scenarios
func_name = "max";
getnode(func_name)(7, 5);     // equivalent to max(7,5), returns 7

// Dynamic TAG labels — access tags by numeric index
for (i=0; i<3; i++) {
    tag = 0xF1000 + i;          // construct TAG index
    node = getnode(tag);        // retrieve $F1000, $F1001, $F1002
    trace(i, node);
}
/*
JSON data: jsonbuf='"{"run":"max", "data":7}"'  // '"..."' can contain any characters
fd = json.load(jsonbuf);
getnode(json.get(fd,"run"))(json.get(fd,"data"),5);
json.free(fd);
// Calls platform max() to compare data with 5, obviously returns 7.
*/
```
#### 1.25: Create or Get Node (setnode)

| Interface | setnode  | Get platform node, create it automatically if it doesn't exist                      |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | name/TAG | String node name or `0xF` numeric TAG label                  |
| Return    | node     | Returns node handle (existing if present, newly created otherwise) |

- The only difference from `getnode`: never returns `false` — auto-creates an empty node if missing
- Ideal for: dynamically generating parameter names or TAG labels inside loops, without pre-declaration

```c
node setnode(name/TAG)
// setnode("filename")   → create 'filename' if missing, return existing otherwise
// setnode(0xF2000)      → create $F2000 if missing, return existing otherwise

// Dynamically create parameters in a loop
for (i=0; i<10; i++) {
    name = printf("item%d", i);           // compose parameter names: "item0", "item1", ...
    setnode(name) = i * 10;       // create if missing and assign
    trace(name);              // verify: item0, item1, ... == 0, 1, ...
}
trace(item0); // item0 is invalid here — it is a local variable in the for loop and released after exit

// Dynamically allocate TAG labels in a loop
for (i=0; i<5; i++) {
    setnode(0xF2000 + i) = inf("file" + i);  // $F2000~$F2004 auto-created and assigned
}
```
#### 1.26: Clear Node (clrnode)

| Interface | clrnode | Clear node content (keeps the node itself)                     |
| --------- | ------- | ------------------------------------------------------------ |
| Param     | name/TAG | String node name or `0xF` numeric TAG label                  |
| Return    | void    | Nothing                                                      |

Same parameter format as `setnode`. Clears the node content to empty while keeping the node alive.

```c
void clrnode(name/TAG)
// clrnode("da")        → clear the content of da
// clrnode(0xFA123467)  → clear the content of $FA123467
// setnode("tmp") = 100;  // create and assign
// clrnode("tmp");        // tmp still exists but is empty, can be reassigned
```
#### 1.27: Delete Node (delnode)

| Interface | delnode  | Delete node from the environment                              |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | name/TAG | String node name or `0xF` numeric TAG label                  |
| Return    | bool     | Success: `true`, Failure: `false`                             |

Same parameter format as `setnode`. Completely removes the node (parameters, TAG labels, function bindings, etc.).

```c
bool delnode(name/TAG)
// delnode("tmp")       → delete regular variable tmp
// delnode(0xF2000)     → delete TAG label $F2000
// setnode("tmp") = 100; delnode("tmp");  // tmp no longer exists
```
#### 1.28: Run Script (runsc)

| Interface | runsc  | Run specified script data       |
| --------- | ------ | ------------------------------- |
| Param     | script | Script string data(unicode)     |
| Return    | result | Returns script execution result |

```c
result runsc{script} // runc{1+2*3} -> {int:7 }
```



#### 1.29: Run String Code (runstr)

| Interface | runstr            | Execute a Y language code string                             |
| --------- | ----------------- | ------------------------------------------------------------ |
| Param     | code              | Code string to execute                                        |
| Param     | flag/null         | Optional flag: any value present → return string result; omitted → return raw node |
| Return    | str/node          | With flag returns string, otherwise returns raw result node   |

```c
node runstr(code)                   // original, returns result node
str  runstr(code, 1)                // returns string result
// runstr("a = 1+2");               → returns node 3
// runstr("inf('test.txt')", 1);    → returns ":str[5]="hello""
```
#### 1.30: JSON Processing (jsonvm)

| Interface | jsonvm      | Process JSON data (supports internal script execution) |
| --------- | ----------- | ------------------------------------------------------ |
| Param     | json_script | JSON data or configuration                             |
| Return    | str         | Returns JSON data                                      |

```c
fd jsonvm(json_script) // aa ="hellow";bb="123";jsonvm{"key":aa+bb} -> str[19]"{"key":"hellow123"}"
```



#### 1.31: User Input (input)

| Interface | input  | Get user input     |
| --------- | ------ | ------------------ |
| Param     | prompt | Prompt message     |
| Return    | str    | Returns user input |

```c
str input("Enter name: ") // name = sys.input("Your name: ")
```



#### 1.32: Load Library (loadlib)

| Interface | loadlib | Dynamically load a library file |
| --------- | ------- | ------------------------------- |
| Param     | libname | Library file name               |
| Return    | bool    | Returns true on success         |

```c
bool loadlib("mylib.dll") // loadlib("test.dll")
```



#### 1.33: Unload Library (unload)

| Interface | unload  | Unload a loaded library                      |
| --------- | ------- | -------------------------------------------- |
| Param     | libname | Library file name; supports partial matching |
| Return    | bool    | Returns true on success                      |

```c
bool unload(handle) // unload("test")
```



#### 1.34: Create Thread

| Interface | createth   | Allocate CPU and memory resources from the system to independently execute a Y-language code block or invoke a specified function via callback |
| --------- | ---------- | ------------------------------------------------------------ |
| Param     | Ycode/func | Y-language code block enclosed in {} or function name (with execution parameters) |
| Param     | null/args  | Function parameters (present when first param is a function name); frozen-copied at creation, e.g. `(fd)` or `(a, b, c)` |
| Return    | fd         | Returns thread handle on success; returns false on failure   |

Two calling modes are supported:

**Mode 1: Code Block Mode**

```c
fd createth(Ycode) // fh=createth({i=0; while(10) {trace(i++); sleep(1);}})
```

**Mode 2: Function Callback Mode (parameters frozen at creation)**
```c
fd createth(func, (args))
// Example 1: createth(uart, (fd))           // Single parameter; fd frozen at creation time
// Example 2: createth(process, (a, b, c))   // Multiple parameters; all frozen
// Ideal for creating threads inside loops; avoids external variables being overwritten
for(i=0; i<10; i++) {
    fd = uart.open(printf("com%d", i), 9600);
    createth(uart, (fd));    // fd frozen at creation moment; won't be overwritten by next loop iteration
}
```



#### 1.35: Kill Thread

| Interface | killth | Find the thread by handle and terminate it (Note: killth may have resource release issues; it's best for Ycode to end on its own) |
| --------- | ------ | ------------------------------------------------------------ |
| Param     | fd     | Handle returned by createth                                  |
| Return    | bool   | Returns true on success; returns false on failure            |

```c
bool killth(fd) // fh=createth({...}); ...... killth(fh)
```



#### 1.36: Create Timer

| Interface | starttimer         | Create a scheduled task to run a Y-language code block or callback a specified function |
| --------- | ------------------ | ------------------------------------------------------------ |
| Param     | Ycode/func         | Y-language code block enclosed in {} or function name         |
| Param     | null/args          | Function parameters (present when first param is a function name); frozen-copied at creation, e.g. `(fd)` or `(a, b, c)` |
| Param     | starttimeS         | Start time (in seconds), supports decimals (e.g. 0.2 = 200ms) |
| Param     | IntervaltimeS/null | Interval time (in seconds), supports decimals (e.g. 0.01 = 10ms); omit for one-time execution |
| Return    | fd                 | Returns timer handle on success; returns false on failure    |

Two calling modes are supported:

**Mode 1: Code Block Mode**
```c
fd starttimer(Ycode, starttimeS, IntervaltimeS/null) 
// fd=starttimer({trace(11)},5);  // Execute {trace(11)} after 5 seconds; the code block prints 11 to the debug console.
// a=1; fd=starttimer({trace(a);a++;if(a>15) stoptimer(fd);},2,0.5);  // After 2 seconds execute {...}, then repeat every 0.5s; prints the value of a to the debug console, then increments a. When a>15, the timer stops itself.
```

**Mode 2: Function Callback Mode (parameters frozen at creation)**

```c
fd starttimer(func, (args), starttimeS, IntervaltimeS/null)
// fd=starttimer(tick, (100,100), 1, 1);     // If tick is a Y function: parameters are loaded once, then the timer drives function body execution.
// fd=starttimer(trace, (fd,"error"), 1, 1);  // If log is C/C++: parameters are locked once, then the timer triggers: log(fd, "error"), fires only once.
    
def tick(a,b) {a++;b--;trace(a, b);}
```

**Parameter Binding Behavior (Y Function vs C/C++ Function)**

| Callback Type | Entry Parameters | Function-Body Local Variables |
|---------------|------------------|-------------------------------|
| Y Function    | **Bound once, changes persist between calls** | **Redefined every call, not preserved** |
| C/C++ Function | **Re-bound on every trigger** | **Redefined every call** |

Example — entry parameter persistence vs. local variable reset:
```y
def tick(a) {
    a++;                // Entry param a: first call 100→101, next 101→102...  preserved
    def b = 0;          // Local var b: re-defined every callback, always starts from 0
    b++;
    trace(a, b);
}
starttimer(tick, (100), 1, 1);
// Output: 101 1, 102 1, 103 1,...  // a grows, b resets every time
```

> **Design Rationale**: Y-language parameters are fundamentally "meta" nodes managed by the platform. They are bound once when the timer is created, and all reads/writes within the function body operate on the same node — naturally persistent. C/C++ functions use independent stack frames on every call, so parameters cannot survive across invocations. When you need "C/C++ computation power + parameter persistence", wrap the C/C++ call inside a Y function and let the state live at the Y layer.



#### 1.37: Stop Timer

| Interface | stoptimer | Find the timer by handle and stop it              |
| --------- | --------- | ------------------------------------------------- |
| Param     | fd        | Handle returned by starttimer                     |
| Return    | bool      | Returns true on success; returns false on failure |

```c
bool stoptimer(fd) // ft=starttimer({...},...); ...... stoptimer(ft)
```



#### 1.38: URL Parse (urlparse)

| Interface | urlparse         | Parse a URL string           |
| --------- | ---------------- | ---------------------------- |
| Param     | url              | URL string                   |
| Return    | host, port, type | Returns parsed URL structure |

```c
parsed urlparse(url) // urlparse("https://www.example.com") -> "www.example.com",443,0x10
```



#### 1.39: Network Connect (connect)

| Interface | connect | Establish a network connection |
| --------- | ------- | ------------------------------ |
| Param     | host    | Host address                   |
| Param     | port    | Port number                    |
| Param     | type    | Connection type                |
| Return    | socket  | Returns socket handle          |

```c
sock connect(host, port, type) 
 //demo1: connect("192.168.1.222",50000,"tcp") -> fd
 //demo2: connect("www.baidu.com",443,"ssl")  -> fd
 //demo3: connect(urlparse("https://www.example.com"))  -> fd
```



#### 1.40: Send Data (send)

| Interface | send   | Send data over network       |
| --------- | ------ | ---------------------------- |
| Param     | socket | Socket handle                |
| Param     | data   | Data to send                 |
| Return    | int    | Returns number of bytes sent |

```c
int send(sock, buffer) // sent = send(connection, sedata)
```



#### 1.41: Receive Data (recv)

| Interface | recv     | Receive data from network    |
| --------- | -------- | ---------------------------- |
| Param     | socket   | Socket handle                |
| Param     | timeoutS | Maximum wait time in seconds |
| Return    | buf      | Returns received data        |

```c
data recv(sock, timeoutS) // redata = recv(fd, 20)
```



#### 1.42: Disconnect (disconnect)

| Interface | disconnect | Disconnect network connection |
| --------- | ---------- | ----------------------------- |
| Param     | socket     | Socket handle                 |
| Return    | bool       | Returns true on success       |

```c
bool disconnect(sock) // disconnect(fd)
```



#### 1.43: Interaction Window Rounds (focusRound / sendRound)

| Interface | focusRound     | No params, no return. Execute once in the current round to record it in the environment |
| --------- | -------------- | ------------------------------------------------------------ |
| Param     | —              | —                                                            |
| Return    | void           | —                                                            |

| Interface | sendRound      | Send a message to the round recorded by focusRound (used by threads/timers created in that round) |
| --------- | -------------- | ------------------------------------------------------------ |
| Param     | msg            | Message to send                                              |
| Return    | bool           | `true` on success                                            |

For interaction window callbacks: `focusRound()` (no params) records the current round. Threads or timers created during this round can later call `sendRound(msg)` to route messages back to the recorded round — even after many subsequent interactions, messages always arrive at the originally recorded round.

```c
// During round 3
focusRound();                               // Record: this is round 3
createth:
    // Thread waits for events in background...
    sendRound("File download complete");    // Message appears in round 3's conversation
// Round 4... Round 5...                   // Main conversation moves forward
// sendRound in thread always delivers to round 3 ← unaffected by later rounds
```
#### 1.44: Execute System Command (cmd)

| Interface | `cmd`         | Invoke Windows command line to execute a system command, capture stdout+stderr |
| --------- | ------------- | ------------------------------------------------------------------------------ |
| Param     | `command`     | Command string to execute                                                      |
| Return    | `str` / `int` | With output: `str` (stdout+stderr); Without output: `int` (exit code)          |

```c
result cmd(command)
// cmd("ver")              → str[44]  Microsoft Windows [Version 10.0...]
// cmd("dir /b *.exe")     → str[51]  lists all .exe in the current directory
// cmd("echo hello")       → str[7]   "hello"
// cmd("gcc --version")    → str[273] gcc version info
// cmd("gcc bad.c")        → str[106] compile errors (stderr merged into stdout)
// cmd("exit /b 42")       → int:42   no output → returns process exit code
```

> **Implementation**: internally invokes `CreateProcessA` with `cmd /c <command>`, and merges stdout+stderr into a single pipe. If the process produces output, the captured text is returned; otherwise the exit code is returned. All CMD built-in syntax is supported (pipes, redirection, etc.).

**Typical Use Cases**

```c
// Get system info
ver_info = cmd("ver");
gcc_ver  = cmd("gcc --version");

// Compile a C extension DLL
result = cmd("gcc -shared myext.c -o dll/myext.dll");
if (type(result) == "int") { trace("compile failed, exit code=", result); }

// Combine with agent to auto-compile AI-generated code
code = str.betweens(resp, "```c", "```");
outf("_gen.c", code);
cmd("gcc -shared _gen.c -o dll/_gen.dll");
loadlib("dll/_gen.dll");
```

