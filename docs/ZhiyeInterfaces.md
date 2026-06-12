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



#### 1.5: Range Data Generation (range)

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



#### 1.6: Indexed Value Extraction (takat)

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



#### 1.7: Formatted Output (printf)

| Interface | printf   | Formatted output to file or console |
| --------- | -------- | ----------------------------------- |
| Param     | format   | Format string                       |
| Param     | args,... | Parameter list                      |
| Return    | str      | Returns the output string           |

```c
str printf(format, args...) // printf("Value: %d", 123) -> "Value: 123"
```



#### 1.8: Get Milliseconds (getms)

| Interface | getms | Get system millisecond count |
| --------- | ----- | ---------------------------- |
| Param     | none  |                              |
| Return    | int   | Returns system milliseconds  |

```c
int getms() // ms = getms()
```



#### 1.9: Get Time (time)

| Interface | time     | Get current time (Unix timestamp)                            |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | str/null | NTP server address, e.g. "cn.pool.ntp.org" for China's NTP server. When absent, obtains from local clock. |
| Return    | int      | Returns timestamp                                            |

```c
int time(ntp_server) // timestamp = time(); Get Unix time from local clock; timestamp = time("cn.pool.ntp.org"); Get Unix time from China's NTP server
```



#### 1.10: Time Conversion (mktime)

| Interface | mktime   | Convert time string to timestamp                    |
| --------- | -------- | --------------------------------------------------- |
| Param     | str_time | Time data in format "%04d-%02d-%02d %02d:%02d:%02d" |
| Return    | int      | Returns timestamp                                   |

```c
int mktime(str_time) // timestamp = mktime("2025-10-14 01:02:51")
```



#### 1.11: Local Time (localtime)

| Interface | localtime | Convert timestamp to local time (inverse of mktime) |
| --------- | --------- | --------------------------------------------------- |
| Param     | timestamp | Timestamp                                           |
| Return    | str_time  | Time data in format "%04d-%02d-%02d %02d:%02d:%02d" |

```c
str_time localtime(timestamp) // str_time = localtime(ts)
```



#### 1.12: Sleep (sleep)

| Interface | sleep   | Put current thread to sleep for specified duration |
| --------- | ------- | -------------------------------------------------- |
| Param     | seconds | Sleep duration in seconds, supports decimals       |
| Return    | none    |                                                    |

```c
void sleep(s) // sleep(0.01) // Sleep for 10ms
```



#### 1.13: Get Environment Variable (getenv)

| Interface | getenv   | Get environment variable value                               |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | name     | Environment variable name                                    |
| Return    | str/bool | Returns environment variable value; returns false on failure |

```c
str getenv("PATH") // path = getenv("PATH")
```



#### 1.14: Get Current Directory (getdir)

| Interface | getdir | Get current working directory  |
| --------- | ------ | ------------------------------ |
| Param     | none   |                                |
| Return    | str    | Returns current directory path |

```c
str getdir() // current_dir = getdir()
```



#### 1.15: Clear Element (clean)

| Interface | clean | Clear an element          |
| --------- | ----- | ------------------------- |
| Param     | name  | Element name              |
| Return    | void  | Nothing                   |

```c
void clean(name) 
/*
-> da="hellow"
<- da
-> out da
<- str[18]"[da]str[6]"hellow""
-> clean da
<-
-> out da
<- str[4]"[da]"
*/
```



#### 1.16: Debug Output (trace)

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



#### 1.17: Output Result (out)

| Interface | out                | Return results upstream to the call chain (console mode outputs to console; UI window mode outputs to window) |
| --------- | ------------------ | ------------------------------------------------- |
| Param     | variable name/null | Variable name to output; when empty, outputs all current variables |
| Return    | str                | Returns the result as a string (caller receives it directly) |

```c
str out(name,...) // Returns processed result as a string to the caller
```

> **Difference between out and trace**: `out` uses the "result channel" — console mode outputs to console, UI mode outputs to window. `trace` uses the "debug channel" — always outputs to debug console. To inspect variables inside threads, use `trace`; `out`'s return result is not receivable by the caller in asynchronous threads.



#### 1.18: Output to File (outf)

| Interface | outf     | Output data to file                                          |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | filename | File name                                                    |
| Param     | data     | Data to output                                               |
| Return    | int/bool | Returns file content length on success; returns false on failure |

```c
int outf(filename, data) // outf("data.bin", ' 11 22 33') -> 3 (Write 3 hex bytes to data.bin file)
```



#### 1.19: Read from File (inf)

| Interface | inf      | Read data from file   |
| --------- | -------- | --------------------- |
| Param     | filename | File name             |
| Return    | data     | Returns file contents |

```c
data inf("file.txt") // content = inf("config.ini")
```



#### 1.20: Set Function Priority (setfunc)

| Interface | setfunc   | Adjust function execution priority |
| --------- | --------- | ---------------------------------- |
| Param     | func_name | Function name                      |
| Param     | priority  | Priority value                     |
| Return    | bool      | Returns true on success            |

```c
bool setfunc("callback", 1) // setfunc("outf", 2)
```



#### 1.21: Get System Node (getnode)

| Interface | getnode | Get platform internal node (parameter/function) by name for remote invocation and script reflection |
| --------- | ------- | ------------------------------------------------------------ |
| Param     | name    | Node name string                                             |
| Return    | node    | Returns node handle; returns `false` if not found            |

```c
node getnode(name)
// getnode("time") -> Get the time function node
// getnode("time")(); -> Execute the platform time() function.
/*
JSON data: jsonbuf='"{"run":"max", "data":7}"'  // '"..."' nested quotes can contain any character
Y-language execution:
fd = json.load(jsonbuf);
getnode(json.get(fd,"run"))(json.get(fd,"data"),5); // Use getnode(json.get(fd,"name")) to get the function node, then (json.get(fd,"data"),5) to call with concatenated parameters.
json.free(fd); // Release the json fd handle
Explanation: Call the platform max function to compare the data value with 5; obviously returns 7.
*/
```



#### 1.22: Run Script (runsc)

| Interface | runsc  | Run specified script data       |
| --------- | ------ | ------------------------------- |
| Param     | script | Script string data(unicode)     |
| Return    | result | Returns script execution result |

```c
result runsc{script} // runc{1+2*3} -> {int:7 }
```



#### 1.23: Run String Code (runstr)

| Interface | runstr  | Run code from a string   |
| --------- | ------- | ------------------------ |
| Param     | strcode | Code string(utf-8)       |
| Return    | result  | Returns execution result |

```c
result runstr(strcode) // runstr("print(\"hello\")") -> ( str[5]"hello" )  or runstr(inf("code.c")) executes code from code.c
```



#### 1.24: JSON Processing (jsonvm)

| Interface | jsonvm      | Process JSON data (supports internal script execution) |
| --------- | ----------- | ------------------------------------------------------ |
| Param     | json_script | JSON data or configuration                             |
| Return    | str         | Returns JSON data                                      |

```c
fd jsonvm(json_script) // aa ="hellow";bb="123";jsonvm{"key":aa+bb} -> str[19]"{"key":"hellow123"}"
```



#### 1.25: User Input (input)

| Interface | input  | Get user input     |
| --------- | ------ | ------------------ |
| Param     | prompt | Prompt message     |
| Return    | str    | Returns user input |

```c
str input("Enter name: ") // name = sys.input("Your name: ")
```



#### 1.26: Load Library (loadlib)

| Interface | loadlib | Dynamically load a library file |
| --------- | ------- | ------------------------------- |
| Param     | libname | Library file name               |
| Return    | bool    | Returns true on success         |

```c
bool loadlib("mylib.dll") // loadlib("test.dll")
```



#### 1.27: Unload Library (unload)

| Interface | unload  | Unload a loaded library                      |
| --------- | ------- | -------------------------------------------- |
| Param     | libname | Library file name; supports partial matching |
| Return    | bool    | Returns true on success                      |

```c
bool unload(handle) // unload("test")
```



#### 1.28: Create Thread

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



#### 1.29: Kill Thread

| Interface | killth | Find the thread by handle and terminate it (Note: killth may have resource release issues; it's best for Ycode to end on its own) |
| --------- | ------ | ------------------------------------------------------------ |
| Param     | fd     | Handle returned by createth                                  |
| Return    | bool   | Returns true on success; returns false on failure            |

```c
bool killth(fd) // fh=createth({...}); ...... killth(fh)
```



#### 1.30: Create Timer

| Interface | starttimer         | Create a scheduled task to run a Y-language code block at a specified time |
| --------- | ------------------ | ------------------------------------------------------------ |
| Param     | Ycode              | Y-language code block enclosed in {}                         |
| Param     | starttimeS         | Start time (in seconds), supports decimals (e.g. 0.2 = 200ms) |
| Param     | IntervaltimeS/null | Interval time (in seconds), supports decimals (e.g. 0.01 = 10ms); omit for one-time execution |
| Return    | fd                 | Returns timer handle on success; returns false on failure    |

```c
fd starttimer(Ycode,starttimeS,IntervaltimeS/null) 
// ft=starttimer({out(11)},5);  // Execute {out(11)} after 5 seconds; the code block prints 11 to the window.
// a=1; ft=starttimer({out(a);a++;},2,0.5);  // Execute {out(a);a++;} after 2 seconds, then repeat every 0.5s; prints the value of a, then increments a by 1.
```



#### 1.31: Stop Timer

| Interface | stoptimer | Find the timer by handle and stop it              |
| --------- | --------- | ------------------------------------------------- |
| Param     | fd        | Handle returned by starttimer                     |
| Return    | bool      | Returns true on success; returns false on failure |

```c
bool stoptimer(fd) // ft=starttimer({...},...); ...... stoptimer(ft)
```



#### 1.32: URL Parse (urlparse)

| Interface | urlparse         | Parse a URL string           |
| --------- | ---------------- | ---------------------------- |
| Param     | url              | URL string                   |
| Return    | host, port, type | Returns parsed URL structure |

```c
parsed urlparse(url) // urlparse("https://www.example.com") -> "www.example.com",443,0x10
```



#### 1.33: Network Connect (connect)

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



#### 1.34: Send Data (send)

| Interface | send   | Send data over network       |
| --------- | ------ | ---------------------------- |
| Param     | socket | Socket handle                |
| Param     | data   | Data to send                 |
| Return    | int    | Returns number of bytes sent |

```c
int send(sock, buffer) // sent = send(connection, sedata)
```



#### 1.35: Receive Data (recv)

| Interface | recv     | Receive data from network    |
| --------- | -------- | ---------------------------- |
| Param     | socket   | Socket handle                |
| Param     | timeoutS | Maximum wait time in seconds |
| Return    | buf      | Returns received data        |

```c
data recv(sock, timeoutS) // redata = recv(fd, 20)
```



#### 1.36: Disconnect (disconnect)

| Interface | disconnect | Disconnect network connection |
| --------- | ---------- | ----------------------------- |
| Param     | socket     | Socket handle                 |
| Return    | bool       | Returns true on success       |

```c
bool disconnect(sock) // disconnect(fd)
```

