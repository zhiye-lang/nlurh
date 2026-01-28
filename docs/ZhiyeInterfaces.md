<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/%E6%94%AF%E5%8F%B6%E5%B9%B3%E5%8F%B0%E6%8E%A5%E5%8F%A3.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>

------

# Description of Built-in Interface Functions for the Zhiye Integration Platform

Definition and usage instructions for Y language interfaces.

#### 1.1: Log Setting (logset)

| Interface | logset        | Description                                                  |
| --------- | ------------- | ------------------------------------------------------------ |
| Input     | level         | Log level (0 Debug, 1 Info, 2 Warning, 3 Error, 4 None)      |
| Input     | flagfile/null | Flag for output to file: 1 outputs to file, 0 or null outputs to console |
| Returns   | bool          | Returns `true` on success                                    |

```c
bool logset(level, flagfile) // logset(2,0) -> true Outputs warnings to console. logset(0,1) Outputs debug info to file zhiye1M_0.log.
```

#### 1.2: Get Length (len)

| Interface | len  | Description                         |
| --------- | ---- | ----------------------------------- |
| Input     | data | Input data                          |
| Returns   | int  | Returns the byte length of the data |

```c
int len(data) // len("hello") -> 5
```

#### 1.3: Fill Data (fill)

| Interface | fill | Description                    |
| --------- | ---- | ------------------------------ |
| Input     | val  | Value to fill                  |
| Input     | len  | Length of the buffer to create |
| Returns   | buf  | Returns the filled buffer data |

```c
buf fill(val, len) // fill(0x00, 4) -> ' 00 00 00 00'
```

#### 1.4: Data Packing (pack)

| Interface | pack | Description                                                  |
| --------- | ---- | ------------------------------------------------------------ |
| Input     | flag | Packing format (">h2" big-endian 2-byte hex, "<s3" little-endian 3-byte string length, ">d4" big-endian 4-byte decimal) |
| Input     | data | Data to pack                                                 |
| Returns   | buf  | Returns the formatted length buffer data                     |

```c
buf sys.pack(format, data...) // pack(">h",14) -> ' 00 0E'
```

#### 1.5: Offset Length (offset)

| Interface | offset   | Description                                 |
| --------- | -------- | ------------------------------------------- |
| Input     | data     | Input data                                  |
| Input     | sta      | Starting offset                             |
| Input     | len/null | Length; if null, takes all data from offset |
| Returns   | data     | Returns the data starting from the offset   |

```c
result offset(data, sta,len/null) // offset("abcdefghijk",3,2) -> "de"
```

#### 1.6: Formatted Output (printf)

| Interface | printf   | Description               |
| --------- | -------- | ------------------------- |
| Input     | format   | Format string             |
| Input     | args,... | Argument list             |
| Returns   | str      | Returns the output string |

```c
str printf(format, args...) // printf("Value: %d", 123) -> "Value: 123"
```

#### 1.7: Get Milliseconds (getms)

| Interface | getms | Description                 |
| --------- | ----- | --------------------------- |
| Input     | None  |                             |
| Returns   | int   | Returns system milliseconds |

```c
int getms() // ms = getms()
```

#### 1.8: Get Time (time)

| Interface | time     | Description                                                  |
| --------- | -------- | ------------------------------------------------------------ |
| Input     | str/null | NTP server address (e.g., "cn.pool.ntp.org" for China's national time service). If null, gets time locally. |
| Returns   | int      | Returns Unix timestamp                                       |

```c
int time(ntp_server) // timestamp = time(); Gets Unix time locally. timestamp = time("cn.pool.ntp.org"); Gets Unix time from the national time service.
```

#### 1.9: Time Conversion (mktime)

| Interface | mktime   | Description                                 |
| --------- | -------- | ------------------------------------------- |
| Input     | str_time | Time string "%04d-%02d-%02d %02d:%02d:%02d" |
| Returns   | int      | Returns Unix timestamp                      |

```c
int mktime(str_time) // timestamp = mktime("2025-10-14 01:02:51")
```

#### 1.10: Local Time (localtime)

| Interface | localtime | Description (opposite of mktime)            |
| --------- | --------- | ------------------------------------------- |
| Input     | timestamp | Unix timestamp                              |
| Returns   | str_time  | Time string "%04d-%02d-%02d %02d:%02d:%02d" |

```c
str_time localtime(timestamp) // str_time = localtime(ts)
```

#### 1.11: Sleep (sleep)

| Interface | sleep   | Description                          |
| --------- | ------- | ------------------------------------ |
| Input     | seconds | Seconds to sleep (supports decimals) |
| Returns   | void    |                                      |

```c
void sleep(s) // sleep(0.01) // Sleep for 10ms
```

#### 1.12: Get Environment Variable (getenv)

| Interface | getenv   | Description                                               |
| --------- | -------- | --------------------------------------------------------- |
| Input     | name     | Environment variable name                                 |
| Returns   | str/bool | Returns environment variable value, or `false` on failure |

```c
str getenv("PATH") // path = getenv("PATH")
```

#### 1.13: Get Current Directory (getdir)

| Interface | getdir | Description                            |
| --------- | ------ | -------------------------------------- |
| Input     | None   |                                        |
| Returns   | str    | Returns current working directory path |

```c
str getdir() // current_dir = getdir()
```

#### 1.14: Output to Console (out)

| Interface | out                | Description                                                  |
| --------- | ------------------ | ------------------------------------------------------------ |
| Input     | variable name/null | Variable name to output. If empty, outputs all current variables. |
| Returns   | bool               | Returns `true` on success                                    |

```c
bool out(name,...) // ab =  20; out(ab) /out ab -> [ab]num:20
```

#### 1.15: Output to File (outf)

| Interface | outf     | Description                                                  |
| --------- | -------- | ------------------------------------------------------------ |
| Input     | filename | File name                                                    |
| Input     | data     | Data to output                                               |
| Returns   | int/bool | On success, returns file content length; on failure, returns `false` |

```c
int outf(filename, data) // outf("data.bin", ' 11 22 33') -> 3 (Saves 3 hex bytes into data.bin file)
```

#### 1.16: Read from File (inf)

| Interface | inf      | Description          |
| --------- | -------- | -------------------- |
| Input     | filename | File name            |
| Returns   | data     | Returns file content |

```c
data inf("file.txt") // content = inf("config.ini")
```

#### 1.17: Set Function Priority (setfunc)

| Interface | setfunc   | Description               |
| --------- | --------- | ------------------------- |
| Input     | func_name | Function name             |
| Input     | priority  | Priority value            |
| Returns   | bool      | Returns `true` on success |

```c
bool setfunc("callback", 1) // setfunc("outf", 2)
```

#### 1.18: Run Script (runsc)

| Interface | runsc      | Description                     |
| --------- | ---------- | ------------------------------- |
| Input     | Scriptdata | Script data                     |
| Returns   | result     | Returns script execution result |

```c
result runsc(Scriptdata) // runsc{1+2*3} -> {num:7 }
```

#### 1.19: JSON Processing (jsonvm)

| Interface | jsonvm      | Description (supports internal script execution) |
| --------- | ----------- | ------------------------------------------------ |
| Input     | json_script | JSON data or configuration                       |
| Returns   | str         | Returns JSON data                                |

```c
fd jsonvm(json_script) // aa ="hellow";bb="123";jsonvm{"key":aa+bb} -> str[19]"{"key":"hellow123"}"
```

#### 1.20: User Input (input)

| Interface | input  | Description        |
| --------- | ------ | ------------------ |
| Input     | prompt | Prompt message     |
| Returns   | str    | Returns user input |

```c
str input("Enter name: ") // name = sys.input("Your name: ")
```

#### 1.21: Run String (runstr)

| Interface | runstr  | Description              |
| --------- | ------- | ------------------------ |
| Input     | strcode | Code string              |
| Returns   | result  | Returns execution result |

```c
result runstr(strcode) // runstr("print(\"hello\")") -> ( str[5]"hello" ) or runstr(inf("code.c")) Executes code from code.c
```

#### 1.22: Load Library (loadlib)

| Interface | loadlib | Description               |
| --------- | ------- | ------------------------- |
| Input     | libname | Library file name         |
| Returns   | bool    | Returns `true` on success |

```c
bool loadlib("mylib.dll") // loadlib("test.dll")
```

#### 1.23: Unload Library (unload)

| Interface | unload  | Description                                |
| --------- | ------- | ------------------------------------------ |
| Input     | libname | Library file name (supports partial match) |
| Returns   | bool    | Returns `true` on success                  |

```c
bool unload(handle) // unload("test")
```

#### 1.23: Create Thread

| Interface | createth | Description (allocates CPU/memory to execute Y code independently) |
| --------- | -------- | ------------------------------------------------------------ |
| Input     | Ycode    | Y language code block enclosed in {}                         |
| Returns   | fd       | Returns thread handle on success, `false` on failure         |

```c
fd createth(Ycode) // fh=createth({ while(1) {out11;sleep(1000);}})
```

#### 1.24: Terminate Thread

| Interface | killth | Description (Note: `killth` may have resource leak issues; it's better for Ycode to end itself) |
| --------- | ------ | ------------------------------------------------------------ |
| Input     | fd     | Handle returned by `createth`                                |
| Returns   | bool   | Returns `true` on success, `false` on failure                |

```c
bool killth(fd) // fh=createth({...}); ...... killth(fh)
```

#### 1.25: Create Timer

| Interface | starttimer         | Description                                                  |
| --------- | ------------------ | ------------------------------------------------------------ |
| Input     | Ycode              | Y language code block enclosed in {}                         |
| Input     | starttimeS         | Start time (seconds, supports float, e.g., 0.2 for 200ms)    |
| Input     | IntervaltimeS/null | Interval time (seconds, supports float). Omit for one-time execution. |
| Returns   | fd                 | Returns timer handle on success, `false` on failure          |

```c
fd starttimer(Ycode,starttimeS,IntervaltimeS/null)
// ft=starttimer({out(11)},5); // Executes {out(11)} after 5 seconds, printing 11 to console.
// a=1; ft=starttimer({out(a);a++;},2,0.5); // Executes {out(a);a++;} after 2 seconds, then every 0.5 seconds. Prints value of `a` then increments it.
```

#### 1.26: Stop Timer

| Interface | stoptimer | Description                                   |
| --------- | --------- | --------------------------------------------- |
| Input     | fd        | Handle returned by `starttimer`               |
| Returns   | bool      | Returns `true` on success, `false` on failure |

```c
bool stoptimer(fd) // ft=starttimer({...}，...); ...... stoptimer(ft)
```

#### 1.28: URL Parsing (urlparse)

| Interface | urlparse         | Description                  |
| --------- | ---------------- | ---------------------------- |
| Input     | url              | URL string                   |
| Returns   | host，port，type | Returns parsed URL structure |

```c
parsed urlparse(url) // urlparse("https://www.example.com") -> "www.example.com",443,0x10
```

#### 1.29: Network Connection (connect)

| Interface | connect | Description           |
| --------- | ------- | --------------------- |
| Input     | host    | Host address          |
| Input     | port    | Port number           |
| Input     | type    | Connection type       |
| Returns   | socket  | Returns socket handle |

```c
sock connect(host, port，type)
 //demo1: connect("192.168.1.222",50000,"tcp") -> fd
 //demo2: connect("www.baidu.com",443,"ssl")  -> fd
 //demo3: connect(urlparse("https://www.example.com"))  -> fd
```

#### 1.30: Send Data (send)

| Interface | send   | Description                  |
| --------- | ------ | ---------------------------- |
| Input     | socket | Socket handle                |
| Input     | data   | Data to send                 |
| Returns   | int    | Returns number of bytes sent |

```c
int send(sock, buffer) // sent = send(connection, sedata)
```

#### 1.31: Receive Data (recv)

| Interface | sys.recv | Description                  |
| --------- | -------- | ---------------------------- |
| Input     | socket   | Socket handle                |
| Input     | timeoutS | Maximum wait time in seconds |
| Returns   | buf      | Returns received data        |

```c
data recv(sock, timeoutS) // redata = recv(fd, 20)
```

#### 1.32: Disconnect (disconnect)

| Interface | disconnect | Description               |
| --------- | ---------- | ------------------------- |
| Input     | socket     | Socket handle             |
| Returns   | bool       | Returns `true` on success |

```c
bool disconnect(sock) // disconnect(fd)
```

#### 1.33: Set Font (setfont)

| Interface | setfont   | Description               |
| --------- | --------- | ------------------------- |
| Input     | font_desc | Font description string   |
| Returns   | bool      | Returns `true` on success |

```c
bool setfont("Arial,12") // .setfont("SimSun,14")
```
