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



#### 1.6: Formatted Output (printf)

| Interface | printf   | Formatted output to file or console |
| --------- | -------- | ----------------------------------- |
| Param     | format   | Format string                       |
| Param     | args,... | Parameter list                      |
| Return    | str      | Returns the output string           |

```c
str printf(format, args...) // printf("Value: %d", 123) -> "Value: 123"
```



#### 1.7: Get Milliseconds (getms)

| Interface | getms | Get system millisecond count |
| --------- | ----- | ---------------------------- |
| Param     | none  |                              |
| Return    | int   | Returns system milliseconds  |

```c
int getms() // ms = getms()
```



#### 1.8: Get Time (time)

| Interface | time     | Get current time (Unix timestamp)                            |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | str/null | NTP server address, e.g. "cn.pool.ntp.org" for China's NTP server. When absent, obtains from local clock. |
| Return    | int      | Returns timestamp                                            |

```c
int time(ntp_server) // timestamp = time(); Get Unix time from local clock; timestamp = time("cn.pool.ntp.org"); Get Unix time from China's NTP server
```



#### 1.9: Time Conversion (mktime)

| Interface | mktime   | Convert time string to timestamp                    |
| --------- | -------- | --------------------------------------------------- |
| Param     | str_time | Time data in format "%04d-%02d-%02d %02d:%02d:%02d" |
| Return    | int      | Returns timestamp                                   |

```c
int mktime(str_time) // timestamp = mktime("2025-10-14 01:02:51")
```



#### 1.10: Local Time (localtime)

| Interface | localtime | Convert timestamp to local time (inverse of mktime) |
| --------- | --------- | --------------------------------------------------- |
| Param     | timestamp | Timestamp                                           |
| Return    | str_time  | Time data in format "%04d-%02d-%02d %02d:%02d:%02d" |

```c
str_time localtime(timestamp) // str_time = localtime(ts)
```



#### 1.11: Sleep (sleep)

| Interface | sleep   | Put current thread to sleep for specified duration |
| --------- | ------- | -------------------------------------------------- |
| Param     | seconds | Sleep duration in seconds, supports decimals       |
| Return    | none    |                                                    |

```c
void sleep(s) // sleep(0.01) // Sleep for 10ms
```



#### 1.12: Get Environment Variable (getenv)

| Interface | getenv   | Get environment variable value                               |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | name     | Environment variable name                                    |
| Return    | str/bool | Returns environment variable value; returns false on failure |

```c
str getenv("PATH") // path = getenv("PATH")
```



#### 1.13: Get Current Directory (getdir)

| Interface | getdir | Get current working directory  |
| --------- | ------ | ------------------------------ |
| Param     | none   |                                |
| Return    | str    | Returns current directory path |

```c
str getdir() // current_dir = getdir()
```



#### 1.14: Output to Console (out)

| Interface | out                | Output data to console                                       |
| --------- | ------------------ | ------------------------------------------------------------ |
| Param     | variable name/null | Variable name to output; when empty, outputs all current variables |
| Return    | bool               | Returns true on success                                      |

```c
bool out(name,...) // ab =  20; out(ab) /out ab -> [ab]num:20
/*
// When no input parameter, prints all meta names (functions and parameters exist as meta elements).
-> out() // out with no parameters
<json><str><tls><tts><uart><ui><setfont><stoptimer><starttimer><killth><createth><disconnect><recv><send><connect><urlparse><stoptimer><starttimer><killth><createth><unload><loadlib><runstr><input><jsonvm><runsc><setfunc><inf><outf><out><getdir><getenv><sleep><localtime><mktime><time><getms><printf><offset><pack><fill><len><logset><return><last><||><&&><--><++><=<><:=><:<><^=><|=><&=><%=><-=><+=></=><*=><!=><==><<=><>=><<><>><=><^><|><&><-><+><%></><*><!><~><.>
<-
-> out json // out with json parameter; json is a collection imported by TextFlow.dll 
[json].sub(free get load press )
<-
-> out ui // out with ui parameter; ui is a collection imported by ui.dll
[ui].sub(smap gmap setdpi close cursor recovery backup get save pull fill push open )
<-
*/
```



#### 1.15: Output to File (outf)

| Interface | outf     | Output data to file                                          |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | filename | File name                                                    |
| Param     | data     | Data to output                                               |
| Return    | int/bool | Returns file content length on success; returns false on failure |

```c
int outf(filename, data) // outf("data.bin", ' 11 22 33') -> 3 (Write 3 hex bytes to data.bin file)
```



#### 1.16: Read from File (inf)

| Interface | inf      | Read data from file   |
| --------- | -------- | --------------------- |
| Param     | filename | File name             |
| Return    | data     | Returns file contents |

```c
data inf("file.txt") // content = inf("config.ini")
```



#### 1.17: Set Function Priority (setfunc)

| Interface | setfunc   | Adjust function execution priority |
| --------- | --------- | ---------------------------------- |
| Param     | func_name | Function name                      |
| Param     | priority  | Priority value                     |
| Return    | bool      | Returns true on success            |

```c
bool setfunc("callback", 1) // setfunc("outf", 2)
```



#### 1.18: Get System Node (getnode)

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



#### 1.19: Run Script (runc)

| Interface | runc   | Run specified script data       |
| --------- | ------ | ------------------------------- |
| Param     | script | Script string data(unicode)     |
| Return    | result | Returns script execution result |

```c
result runc{script} // runc{1+2*3} -> {int:7 }
```



#### 1.20: Run String Code (runstr)

| Interface | runstr  | Run code from a string   |
| --------- | ------- | ------------------------ |
| Param     | strcode | Code string              |
| Return    | result  | Returns execution result |

```c
result runstr(strcode) // runstr("print(\"hello\")") -> ( str[5]"hello" )  or runstr(inf("code.c")) executes code from code.c
```



#### 1.21: JSON Processing (jsonvm)

| Interface | jsonvm      | Process JSON data (supports internal script execution) |
| --------- | ----------- | ------------------------------------------------------ |
| Param     | json_script | JSON data or configuration                             |
| Return    | str         | Returns JSON data                                      |

```c
fd jsonvm(json_script) // aa ="hellow";bb="123";jsonvm{"key":aa+bb} -> str[19]"{"key":"hellow123"}"
```



#### 1.22: User Input (input)

| Interface | input  | Get user input     |
| --------- | ------ | ------------------ |
| Param     | prompt | Prompt message     |
| Return    | str    | Returns user input |

```c
str input("Enter name: ") // name = sys.input("Your name: ")
```



#### 1.23: Load Library (loadlib)

| Interface | loadlib | Dynamically load a library file |
| --------- | ------- | ------------------------------- |
| Param     | libname | Library file name               |
| Return    | bool    | Returns true on success         |

```c
bool loadlib("mylib.dll") // loadlib("test.dll")
```



#### 1.24: Unload Library (unload)

| Interface | unload  | Unload a loaded library                      |
| --------- | ------- | -------------------------------------------- |
| Param     | libname | Library file name; supports partial matching |
| Return    | bool    | Returns true on success                      |

```c
bool unload(handle) // unload("test")
```



#### 1.25: Create Thread

| Interface | createth | Allocate CPU and memory resources from the system to independently execute a Y-language code block |
| --------- | -------- | ------------------------------------------------------------ |
| Param     | Ycode    | Y-language code block enclosed in {}                         |
| Return    | fd       | Returns thread handle on success; returns false on failure   |

```c
fd createth(Ycode) // fh=createth({ while(1) {out11;sleep(1000);}})
```



#### 1.26: Kill Thread

| Interface | killth | Find the thread by handle and terminate it (Note: killth may have resource release issues; it's best for Ycode to end on its own) |
| --------- | ------ | ------------------------------------------------------------ |
| Param     | fd     | Handle returned by createth                                  |
| Return    | bool   | Returns true on success; returns false on failure            |

```c
bool killth(fd) // fh=createth({...}); ...... killth(fh)
```



#### 1.27: Create Timer

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



#### 1.28: Stop Timer

| Interface | stoptimer | Find the timer by handle and stop it              |
| --------- | --------- | ------------------------------------------------- |
| Param     | fd        | Handle returned by starttimer                     |
| Return    | bool      | Returns true on success; returns false on failure |

```c
bool stoptimer(fd) // ft=starttimer({...},...); ...... stoptimer(ft)
```



#### 1.29: URL Parse (urlparse)

| Interface | urlparse         | Parse a URL string           |
| --------- | ---------------- | ---------------------------- |
| Param     | url              | URL string                   |
| Return    | host, port, type | Returns parsed URL structure |

```c
parsed urlparse(url) // urlparse("https://www.example.com") -> "www.example.com",443,0x10
```



#### 1.30: Network Connect (connect)

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



#### 1.31: Send Data (send)

| Interface | send   | Send data over network       |
| --------- | ------ | ---------------------------- |
| Param     | socket | Socket handle                |
| Param     | data   | Data to send                 |
| Return    | int    | Returns number of bytes sent |

```c
int send(sock, buffer) // sent = send(connection, sedata)
```



#### 1.32: Receive Data (recv)

| Interface | recv     | Receive data from network    |
| --------- | -------- | ---------------------------- |
| Param     | socket   | Socket handle                |
| Param     | timeoutS | Maximum wait time in seconds |
| Return    | buf      | Returns received data        |

```c
data recv(sock, timeoutS) // redata = recv(fd, 20)
```



#### 1.33: Disconnect (disconnect)

| Interface | disconnect | Disconnect network connection |
| --------- | ---------- | ----------------------------- |
| Param     | socket     | Socket handle                 |
| Return    | bool       | Returns true on success       |

```c
bool disconnect(sock) // disconnect(fd)
```



#### 1.34: Set Font (setfont)

| Interface | setfont   | Set current font        |
| --------- | --------- | ----------------------- |
| Param     | font_desc | Font description string |
| Return    | bool      | Returns true on success |

```c
bool setfont("Arial,12") // .setfont("SimSun,14")
```
