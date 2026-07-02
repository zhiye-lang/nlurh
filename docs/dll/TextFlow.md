<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/dll/TextFlow_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>


# TextFlow Component Library TextFlow.dll Usage Guide

Y-Language Interface Specification (Array names `"str"` and `"json"`: use the `out str` / `out json` commands to display all subset interfaces)

```jade
---<out str
[str].sub(convert find outc replace betweens betweenb )
---<out json
[json].sub(free get load press )
```

------

#### 1. String Group Functions (Array name `"str"`)

##### 1.1: Batch Content Extraction (betweenb)

| Interface | `str.betweenb`   | Batch-extracts content between markers                        |
| --------- | ---------------- | ------------------------------------------------------------- |
| Parameter | `Inbuf`          | Input data                                                    |
| Parameter | `tagSta`         | Start marker                                                  |
| Parameter | `tagEnd/len`     | End marker or extraction length                               |
| Parameter | `max/null`       | Maximum number of results to retrieve; if omitted, only 1 result is returned |
| Returns   | `outbuf`         | Success: extracted data  Failure: `false`                     |

```c
buf str.betweenb(inbuf, tag, tagEnd/len, max/null) // outbuf = str.betweenb(inf"test.txt", "第", "\r\n", 5000);
```

##### 1.2: Content Extraction (betweens)

| Interface | `str.betweens`   | Splits content into inside/outside markers                    |
| --------- | ---------------- | -------------------------------------------------------------- |
| Parameter | `Inbuf`          | Input data                                                     |
| Parameter | `tagSta`         | Start marker                                                   |
| Parameter | `tagEnd`         | End marker                                                     |
| Parameter | `out/null`       | Optional output: receives content outside the markers           |
| Returns   | `str`            | Returns content between markers (first match)                   |

```c
str str.betweens(inbuf, tagSta, tagEnd, out/null)
// code = str.betweens(response, "```y", "```", text);
//   code → Y code between markers (for execution)
//   text → display text outside markers (for the user)
```

> **Vs. betweenb**: `betweens` returns only the first match (single); `betweenb` retrieves all matches in batch.


##### 1.3: String Separator (separate_open / separate_next / separate_close)

An iterator-style API for splitting data by markers — ideal for responses containing multiple code blocks.

| Interface | `separate_open`  | Create a separator handle, splitting data by markers           |
| --------- | ---------------- | -------------------------------------------------------------- |
| Parameter | `data`           | Input data                                                     |
| Parameter | `tagSta`         | Start marker                                                   |
| Parameter | `tagEnd`         | End marker                                                     |
| Returns   | `fd`             | Separator handle; `false` on failure                           |

| Interface | `separate_next`  | Fetch the next separated segment                               |
| --------- | ---------------- | -------------------------------------------------------------- |
| Parameter | `fd`             | Separator handle                                               |
| Parameter | `inner/null`     | Optional output: receives content between markers; null = outer text only |
| Returns   | `outer`          | Returns the next outer (non-code) segment; `false` when done   |

| Interface | `separate_close` | Close the separator                                            |
| --------- | ---------------- | -------------------------------------------------------------- |
| Parameter | `fd`             | Separator handle                                               |
| Returns   | `bool`           | `true` on success                                              |

```c
fd = str.separate_open(data, "```y", "```");

// Iterate all segments
while (outer = str.separate_next(fd, inner)) {
    trace("Outer text:", outer);        // text outside code blocks
    if (inner != false) {
        result = runstr(inner);         // execute code block
        agent.feedback(result);
    }
}
str.separate_close(fd);
```

> **Vs. betweens**: `betweens` returns only the first match (single-shot); the separator is designed for responses with **multiple** code blocks, iterating them one by one.


##### 1.4: Batch Content Replacement (replace)

| Interface | `str.replace`    | Batch-replaces specified content                              |
| --------- | ---------------- | ------------------------------------------------------------- |
| Parameter | `Inbuf`          | Input data                                                    |
| Parameter | `sourc`          | Target string to replace                                      |
| Parameter | `replace/null`   | Replacement content; if omitted, the matched block is deleted |
| Returns   | `outbuf`         | Success: replaced data  Failure: `false`                      |

```c
buf str.replace(Data, sourc, replace/NULL) // outbuf = str.replace(inf"test.txt", "从..", "以..");
```



##### 1.5: Array Code Output (outc)

| Interface | `str.outc`    | Outputs array data as C code, appended to `const_cdata.c`    |
| --------- | ------------- | ------------------------------------------------------------- |
| Parameter | `name`        | Array name used in `const_cdata.c`                           |
| Parameter | `data, ...`   | Output data; supports unlimited number of arguments           |
| Returns   | `bool`        | Success: `true`  Failure: `false`                             |

```c
bool str.outc(name, data...) // str.outc("data", buf) 
```



##### 1.6: File Search (find)

| Interface | `str.find`      | Searches for files of specified types starting from a given directory (recursively) |
| --------- | --------------- | ----------------------------------------------------------------------------------- |
| Parameter | `path`          | Root directory for the search                                                       |
| Parameter | `type`          | File type filter (string format); supports wildcards, separated by `;`              |
| Parameter | `subflag/null`  | Recursion depth: `0` = unlimited; if omitted, defaults to `0`                       |
| Returns   | `sub`           | Success: set of matched filenames (space == sub..)  Failure: `false`                |

```c
sub str.find(path, type, subflag/null) // str.find("path", "*.c;*.h");  // all .c and .h files under the path directory
```



##### 1.7: File Encoding Conversion (convert)

| Interface | `str.convert` | Converts the encoding of specified files (checks current encoding first; converts only if different from the target, then overwrites the source file) |
| --------- | ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Parameter | `pathname`    | File path(s) to convert                                                                                                                                |
| Parameter | `CodePage`    | Target encoding — string: `"utf8"`, `"gbk"`, `"utf8-bom"` or numeric: `936`, `65001`, `65002`                                                         |
| Returns   | `cont, max`   | Success: two values — number of files converted and total number of files                                                                              |

```c
sub str.convert(pathname, CodePage) // str.convert("pathname", "utf8");  // convert the specified file to UTF-8
    // Combined with find: str.convert(str.find("G:\\test", "*.h;*.c"), "utf8"); // convert all .c and .h files under G:\test to UTF-8
```



------

### 2. JSON Functions (Array name `"json"`)

##### 2.1: Extract JSON Data (press)

| Interface | `json.press` | Retrieves the value of a key from JSON data (combines load, get, and free) |
| --------- | ------------ | -------------------------------------------------------------------------- |
| Parameter | `jsonbuf`    | JSON data                                                                  |
| Parameter | `key`        | Key to look up; use `"."` to access nested objects                         |
| Returns   | `str`        | Success: string value of the key  Failure: `false`                         |

```c
str json.press(jsonbuf, key) 
// json.press("{\"abb\":\"cdd\",\"struct\":{\"abb\":\"cdd\"}}", "struct.abb") 
// or json.press('"{"abb":"cdd","struct":{"abb":"cdd"}}"', "struct.abb") 
/*
 jd = jsonvm{"abb":"cdd","sub":{"dir":"location"+getdir(),"path":"env"+getenv"PATH"}}  // jsonvm is the platform's built-in JSON scripting feature
json.press(jd, "sub.dir");
json.press(jd, "sub.path")
```



##### 2.2: Load JSON Data (load)

| Interface | `json.load` | Loads JSON data into memory                   |
| --------- | ----------- | --------------------------------------------- |
| Parameter | `jsonbuf`   | JSON data                                     |
| Returns   | `fd`        | Success: handle  Failure: `false`             |

```c
fd json.load(jsonbuf) // data = "cdd"; fd = json.load(jsonvm{"abb": data+"caa"});  // jsonvm is the platform's built-in JSON scripting feature
```



##### 2.3: Read JSON Key Value (get)

| Interface | `json.get` | Reads the value of a key via a JSON handle                    |
| --------- | ---------- | ------------------------------------------------------------- |
| Parameter | `fd`       | JSON data handle                                              |
| Parameter | `key`      | Key to look up; use `"."` to access nested objects            |
| Returns   | `str`      | Success: string value of the key  Failure: `false`            |

```c
str json.get(fd, key) // json.get(fd, "abb");  // continuing from example 2.2, returns "cddcaa"
```



##### 2.4: Free JSON Data (free)

| Interface | `json.free` | Releases data cached by `json.load`           |
| --------- | ----------- | --------------------------------------------- |
| Parameter | `fd`        | JSON data handle                              |
| Returns   | `bool`      | Success: `true`  Failure: `false`             |

```c
bool json.free(fd) // releases data loaded by json.load
```
