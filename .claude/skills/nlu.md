---
name: nlu
description: Zhiye Unity Platform complete knowledge base. Auto-loads when user mentions "Zhiye", "zhiye", "支叶", "Y Language", "NLU", DLL extension development, or platform interfaces. Covers: C/C++ DLL development/build/test, Y Language syntax, all built-in DLL interfaces (ui/crypt/uart/tts/mark/TextFlow), platform APIs (file/network/thread/timer). Trigger words: Zhiye, zhiye, use Zhiye to, Zhiye platform, Y Language, Y Language syntax, build dll, develop extension, how to use, loadlib, DLL interface, NLU.
---

# Zhiye Platform DLL Extension Knowledge Base (Hierarchical Index)

> **Knowledge Hierarchy**: Zhiye Platform → Y Language → Built-in APIs → External DLL Libraries → Groups → Interfaces → Usage
> Each level is self-contained. Drill down by heading level as needed. This project is the nlurh runtime distribution (32-bit + 64-bit platforms).

---

## L0 · Platform Overview

- Runtime root: `nlurh/`, contains `nlu_run_32bit/` and `nlu_run_64bit/` complete environments
- Launch: double-click `_zhiye.exe` and enter password `1234`, or double-click `支叶.cmd`, or CLI `_zhiye.exe 1234 Ycode.c`
- Console encoding: **UTF-8**, log macro: `LG()` / debug: `trace()`
- DLLs go in `<runtime>/dll/`, load at startup with `loadlib"dll/xxx.dll"`
- 32-bit: memory-efficient; 64-bit: double-precision + long ints, supports OpenCV, Ollama, etc.
- Homepage: https://y-lang.tech
- Video tutorials: Search "支叶认知" on Kwai/TikTok/Bilibili

### Startup Script Ycode.c (Auto-loaded)
```c
logset(0);                    // Enable debug logging
loadlib"dll/ui.dll";          // Pre-load common libraries
loadlib"dll/crypt.dll";
loadlib"dll/uart.dll";
loadlib"dll/tts.dll";
loadlib"dll/TextFlow.dll";
loadlib"dll/freetype.dll";
loadlib"dll/mark.dll";
```

---

## L1 · Y Language Syntax

### Type System
| Type | Syntax | Example |
|------|--------|---------|
| int | decimal/hex/octal | `123`, `0xFA12` |
| num | with decimal point | `78.64` |
| str | `"..."` escapes `\n\t\r\"\\\xHH\uHHHH` | `"hello"` |
| buf | `'...'` mixed hex formats (8/16/32bit) | `'112233'`, `'\x11㍄0 0x11,0x2233,0x44556677'` |
| range | `start~end[:step]` | `10~100:2` |

**Core Rule**: `+` result type follows the **left operand** — `str+buf`→str, `buf+str`→buf

### Syntax at a Glance
```c
// Variables (no type declaration needed — type is assigned on value)
a = 123; b = "str"; c = '11 22 33';

// Control flow
if (cond) {} else {}
while (cond) {}
for (i=0; i<10; i++) {}
switch (x) { case 90~100 -> "Excellent"; }  // Range matching supported

// Functions (support overloading)
def name(a, b) { return a + b; }
f"arg"            // Single-argument: parentheses optional
"data" |> func    // Pipe: left value as first argument of right function

// Unique syntax
while(100) {}     // Loop exactly 100 times (Y Language unique)
while() {}        // Infinite loop

// Vectors / matrices
[1 2; 3 4]        // space-separated elements, ; separates rows

// Buffer slicing
buf[0~4]          // Extract bytes 0~4 from buf
```

### Type Conversion Reference
| Operation | Result | Example |
|-----------|--------|---------|
| `str + buf` | str | `"Hello" + '\x57\x6F\x72\x6C\x64'` → `"HelloWorld"` |
| `buf + str` | buf | `'11 22' + "AB"` → `'11 22 41 42'` |
| `str + int` | str | `"val=" + 123` → `"val=123"` |
| `buf + int` | buf | `'00' + 255` → `'00 FF'` |
| `buf + buf` | buf | Concatenation |
| `str + str` | str | Concatenation |

---

## L2 · Platform Built-in API

### Debugging
| Function | Description |
|----------|-------------|
| `logset(level, flagfile/null)` | level: 0=debug/1=info/2=warn/3=error/4=none; flagfile:1→file `zhiye1M_0.log` |
| `trace(name, ...)` | Output to console `[type][len]"val"` |
| `out(name, ...)` | Output to return value (invisible unless captured) |
| `printf(format, args...)` | Formatted output (C printf style) |

### File Operations
| Function | Description |
|----------|-------------|
| `inf(path)` | Read file → buf |
| `outf(path, data)` | Write data to file |
| `len(data)` | Get byte length of data |
| `fill(val, len)` | Generate fill data of specified length |
| `pack(format, val)` | Data packing (big/little endian, specified byte count) |
| `range(rng, bits)` | Synthesize buf data from range |

### Time
| Function | Description |
|----------|-------------|
| `time()` | Get Unix timestamp (seconds) |
| `time("ntp_server")` | Get time from NTP server |
| `localtime(ts)` | Timestamp → `"2025-10-14 01:02:51"` |
| `mktime("2025-10-14 01:02:51")` | Time string → timestamp |
| `getms()` | Get system milliseconds |
| `sleep(s)` | Sleep seconds (decimals supported, e.g. `0.01`) |

### Networking
| Function | Description |
|----------|-------------|
| `urlparse("https://...")` | Parse URL → host, port, type |
| `connect(host, port)` | TCP connect → fd |
| `send(fd, data)` | Send data |
| `recv(fd, len)` | Receive data |
| `disconnect(fd)` | Disconnect |

### Threading & Timers
| Function | Description |
|----------|-------------|
| `createth({...})` | Create thread (code block in braces) |
| `killth(fd)` | Terminate thread |
| `starttimer({...}, delay, interval)` | Create timer |

### Dynamic Loading
| Function | Description |
|----------|-------------|
| `loadlib("dll/xxx.dll")` | Dynamically load DLL extension |
| `loadlib("xxx")` | Load (auto-appends .dll, searches dll/ directory) |
| `unload("xxx")` | Unload a loaded library |

### Reflection & Dynamic Execution
| Function | Description |
|----------|-------------|
| `getnode("name")` | Reflect to get platform internal node |
| `runstr("code")` | Execute Y Language code string |
| `runsc{...}` | Execute Y Language code block |
| `jsonvm{...}` | JSON data processing VM |

### Environment
| Function | Description |
|----------|-------------|
| `getenv("NAME")` | Get system environment variable |

---

## L3 · External DLL Libraries

> Each DLL is a **library**. Within each library, interfaces are grouped into **groups** (call prefix), and each group contains specific **interfaces**. Libraries will keep expanding — see L6 for the full catalog.

### L3.1 · ui.dll — Window/Image (Library→Group "ui", 14 interfaces)

**Image Processing**:
| Interface | Parameters | Returns | Description |
|-----------|------------|---------|-------------|
| `ui.gmap(path)` | File path | mapdata | Read image (supports bmp/png/jpg/tga/webp) |
| `ui.smap(path, data)` | Path + image data | int (file size) | Save image |
| `ui.dmap(data)` | Image data | bool | Popup display of image |
| `ui.resizemap(data, w, h/null)` | Image + w + h | mapdata | Resize (h=null means same as w) |

**Window Operations**:
| Interface | Parameters | Returns | Description |
|-----------|------------|---------|-------------|
| `ui.open(w, h, name)` | Width + height + title | fd | Create window |
| `ui.close(fd)` | fd | bool | Close window |
| `ui.fill(fd, color, flag)` | fd + color + flag | bool | Fill window (color=0xRRGGBB or 0xaaRRGGBB, flag controls alpha) |
| `ui.push(fd, mapdata, left, top)` | fd + image + origin | bool | Draw image onto window |
| `ui.backup(fd)` / `ui.backup(fd, x, y, w, h)` | fd + region | bool/mapdata | Backup window |
| `ui.recovery(fd)` / `ui.recovery(fd, x, y, w, h)` | fd + region | bool | Restore window |
| `ui.save(fd, file)` | fd + filename | bool | Save window to image file |
| `ui.cursor(fd, idx)` | fd + cursor ID | bool | Set cursor (32649=hand/32512=arrow) |
| `ui.get()` | — | UI_FuncBase | Get graphics low-level interface (for C/C++) |
| `ui.pull(fd)` | fd | UI_FuncSyn | Get window control info (for C/C++) |
| `ui.setdpi(dpi)` | dpi value | bool | Set DPI scaling |

### L3.2 · crypt.dll — Encryption (Library→Group "tls", 27 interfaces)

**Data Conversion**:
| Interface | Description | Example |
|-----------|-------------|---------|
| `tls.bts(buf)` | hex→str (doubles length) | `tls.bts('\xA1\xB2')` → `"A1B2"` |
| `tls.stb(str)` | str→hex (halves length) | `tls.stb("A1B2")` → `'\xA1\xB2'` |
| `tls.b64e(data)` | Base64 encode | — |
| `tls.b64d(str)` | Base64 decode | — |
| `tls.rand()` | Random int | `tls.rand()` → int |
| `tls.rand(n)` | Random n-byte buf | `tls.rand(16)` → buf[16] |

**Hash Algorithms**:
| Interface | Description |
|-----------|-------------|
| `tls.md(flag, data)` | Hash: flag=md5/sha1/sha256/sm3 |
| `tls.mdc(flag, key, data)` | HMAC: flag=md5/sha1/sha256/sm3 |

**Symmetric Encryption**:
| Interface | Description |
|-----------|-------------|
| `tls.ecbe(flag, key, data)` | ECB encrypt: flag=des/aes/sm4 |
| `tls.ecbd(flag, key, data)` | ECB decrypt |
| `tls.cbce(flag, key, data, iv/null)` | CBC encrypt (iv defaults to all 0x00) |
| `tls.cbcd(flag, key, data, iv/null)` | CBC decrypt |

**Asymmetric Encryption**:
| Interface | Description |
|-----------|-------------|
| `tls.pk_get(key/"", flag)` | Generate keypair: flag=sm2/rsa:1024/rsa:2048 |
| `tls.pk_inkey(key)` | Import key (pem/cer format) |
| `tls.pk_free(ctx)` | Free key context |
| `tls.pk_outpub(ctx)` | Export public key (ASN1) |
| `tls.pk_outpri(ctx)` | Export private key (ASN1) |
| `tls.pk_enc(ctx, data, pad)` | Public-key encrypt (pad=v15/v21) |
| `tls.pk_dec(ctx, data, pad)` | Private-key decrypt |
| `tls.pk_sign(ctx, data, hash_pad)` | Sign (e.g. "sha1v15"/"sha256v21") |
| `tls.pk_verify(ctx, data, sign, hash_pad)` | Verify signature |
| `tls.pk_rsapub(ctx, data)` | RSA raw public operation |
| `tls.pk_rsapri(ctx, data)` | RSA raw private operation |

**Certificates**:
| Interface | Description |
|-----------|-------------|
| `tls.gp10(key, subname, hashlen/null)` | Generate PKCS#10 CSR |
| `tls.x509v(cer, cerlast)` | Verify certificate chain |
| `tls.x509cw(sn, md, p10, isskey, rootca)` | Issue certificate |
| `tls.x509w(...)` | Write certificate |
| `tls.getasn(...)` | ASN.1 parsing |

### L3.3 · uart.dll — Serial Port (Library→Group "uart", 7 interfaces)

| Interface | Parameters | Returns | Description |
|-----------|------------|---------|-------------|
| `uart.open(com, baud, flag)` | Port + baud rate + flags | fd | Open serial port |
| `uart.send(fd, data)` | fd + data | bool | Send data |
| `uart.recv(fd)` | fd | buf | Receive data |
| `uart.close(fd)` | fd | bool | Close serial port |
| `uart.crecv(fd)` | fd | — | Start threaded receive |
| `uart.scan(tag)` | tag | str set | Scan available ports |
| `uart.getapi()` | — | C interface | Get low-level C interface |

### L3.4 · tts.dll — Speech Synthesis (Library→Group "tts", 3 interfaces)

| Interface | Parameters | Returns | Description |
|-----------|------------|---------|-------------|
| `tts.set(vol, rate)` | Volume + rate | bool | Set parameters |
| `tts.pay(text, block)` | Text + blocking flag | bool | Speak (block=1 for blocking) |
| `tts.swav(hz, ms, vol)` | Frequency + duration + volume | wavbuf | Generate WAV audio |

### L3.5 · mark.dll — Image Annotation (Library→Group "mk", 5 interfaces)

Depends on ui + freetype + TextFlow.

| Interface | Description |
|-----------|-------------|
| `mk.loadgui(ui.get())` | Load UI low-level interface |
| `mk.loadfont(ft.getapi(), path)` | Load font |
| `mk.setfont(h, color)` | Set font size and color |
| `mk.perimg(imgs, tags)` | Image annotation processing |
| `mk.unify(imgs, tags, w, h)` | Unify annotation format |

### L3.6 · TextFlow.dll — Text + JSON (Library→Groups "str"/"json", 10 interfaces)

**str group**:
| Interface | Description |
|-----------|-------------|
| `str.betweenb(data, start, end)` | Extract content between two markers |
| `str.replace(data, old, new)` | String replacement |
| `str.find(data, pattern)` | String search |
| `str.convert(data, fmt)` | Format conversion |
| `str.outc(data)` | Output characters |

**json group**:
| Interface | Description |
|-----------|-------------|
| `json.press(buf, key)` | Extract JSON field (use `.` for nesting) |
| `json.load(buf)` | Load JSON → fd |
| `json.get(fd, key)` | Get field from fd |
| `json.free(fd)` | Free JSON handle |

---

## L4 · C/C++ DLL Development

### Standard Template (see examples/dll_nlu3/ThirdLib/ThirdFunc.cpp)

```cpp
#include "ThirdBasics.h"     // ★ Must be first
#include "sz_api_ext.h"

//======= DLL Init/Deinit (optional) ==========================
void sz_lib_attach(void* p) {
    printf("DLL Init...\n");  // LOG not yet initialized at this point
}
void sz_lib_detach(void* p) {
    printf("DLL DeInit\n");
}

//======= Function Implementation =============================
// Signature: int FuncName(EnvP pENV, ExpUnit*& pInOutPar)
// Returns: positive = number of returned params, 0 = no return, negative = error code
// Error code -5: input parameters don't meet function requirements

static int EXP_Func(EnvP pENV, ExpUnit*& pInOutPar) {
    // 1. Parse parameters (linked list, sequential read)
    ExpUnit* p = sz_parsing_unit(pInOutPar);
    if (p == NULL) return -5;

    // 2. Type checking
    if (p->type == TYPE_NUM) {
        intptr_t val = SZ_GET_INT_VALUE(p);  // Convert to int regardless of int/num
    } else if (p->type == TYPE_DATA) {
        DataBufStr* dbs = p->pBfS;           // Get data buffer
        LG("data[%d]:%s", dbs->Len, dbs->pStr);
    }

    // 3. Execute logic ...

    // 4. Return results
    sz_set_unit_bool(pInOutPar, true);       // Return bool
    // Or:
    sz_set_unit_int(pInOutPar, 100);         // Return int
    // Or:
    sz_set_unit_num(pInOutPar, 3.14);        // Return num
    // Or:
    DataBufStr* out = sz_get_dbs("hello", 5);
    sz_take_unit_dbs(pInOutPar, DATA_STR, out);  // Return str/buf (dbs is taken, no free needed)

    return 1;  // Return 1 parameter
}

//======= Function Table ======================================
static const SzFuncTable gFuncTbl = {
    FUNC_TYPE_I, 1,
    (1 << 16) | (0 << 8) | (1),   // version
    "ArrayName",                   // Called via "ArrayName.FuncName" in Y Language
    {
        {"Func1", EXP_Func},
        {"Func2", EXP_Func2},
        {NULL, NULL},             // ★ Required terminator
    }
};

//======= DLL Export ==========================================
// Developer info (JSON format: ID/Company/Author/Phone)
const char sDeveloperInfo[] = "{\"Author\":\"Your Name\"}";

extern const SzImportSystem gTotalInterSet = {
    sDeveloperInfo,
    NULL,                         // Developer certificate
    { &gFuncTbl, NULL }           // Function table list
};
```

### Key Types
| Type | Description |
|------|-------------|
| `u8/u16/u32/u64` | Basic integer types |
| `DataBufStr` | Data buffer (`pBuf` + `Len` + `pStr`) |
| `ExpUnit` | Expression node (type + tval + pBfS/iVal/iNum) |
| `TYPE_NUM` | Numeric type |
| `TYPE_DATA` | Data type (str/buf) |
| `NUM_NUM` | Float subtype |
| `DATA_BUF` | buf subtype |
| `DATA_STR` | str subtype |
| `SzFuncTable` | Function registration table |
| `SzImportSystem` | DLL export interface |

### Key API Functions
| Function | Description |
|----------|-------------|
| `sz_parsing_unit(pInOutPar)` | Parse next parameter (advances linked list) |
| `sz_set_unit_bool/int/num(p, val)` | Set return value |
| `sz_take_unit_dbs(p, type, dbs)` | Set data return value (takes ownership of memory) |
| `sz_get_dbs(pBuf, len)` | Allocate DataBufStr |
| `sz_free_dbs(dbs)` | Free DataBufStr |
| `sz_realloc_dbs(dbs, newLen)` | Reallocate |
| `sz_read_file(path)` | Read file → DataBufStr |
| `SZ_GET_INT_VALUE(p)` | Extract integer value (compatible with int/num) |
| `LG("fmt", ...)` | Debug log (requires logset(0) to enable) |
| `LG_HEX(msg, pBuf, len)` | Hex dump |

### Building
```bash
# 64-bit Release
MSBuild examples/dll_nlu3/ThirdLib.sln /p:Configuration=Release /p:Platform=x64

# 32-bit Release
MSBuild examples/dll_nlu3/ThirdLib.sln /p:Configuration=Release /p:Platform=Win32

# Output: examples/dll_nlu3/x64/Release/gThirdLib.dll
# Copy to: nlu_run_64bit/dll/
```

---

## L5 · Build & Test Workflow

### 5.1 Standard Test Flow
```bash
# 1. Build DLL
MSBuild examples/dll_nlu3/ThirdLib.sln /p:Configuration=Release /p:Platform=x64 /v:quiet

# 2. Copy to runtime
cp examples/dll_nlu3/x64/Release/gThirdLib.dll nlu_run_64bit/dll/

# 3. Write test script Ycode.c
# 4. Run and filter output
cd nlu_run_64bit
timeout 10 ./_zhiye.exe 1234 Ycode.c 2>&1 | \
  sed 's/\x1b\[[0-9;]*m//g' | grep -E '\[ArrayName\]|\[str\]'
```

### 5.2 Test Script Template
```c
logset(0);                          // Enable debug output
loadlib"dll/ui.dll";                // Load dependencies
loadlib"dll/crypt.dll";
loadlib"dll/yourlib.dll";

trace("=== Test Start ===");

// Test cases
ret = YourArrayName.YourFunc(params);
trace("Result: " + ret);

trace("=== Test End ===");
```

### 5.3 Output Filtering
| Target | Filter |
|--------|--------|
| C++ LG log | `grep '\[ArrayName\]'` |
| Y trace output | `grep '\[str\]'` |
| All debug output | `grep -E '\[.*\]'` |
| Raw bytes | `xxd` |

### 5.4 Common Test Snippets
```c
// Test UI
logset(0); loadlib"dll/ui.dll";
fd = ui.open(640, 480, "Test");
ui.fill(fd, 0xFF0000, 2);   // Red
sleep(1);
ui.fill(fd, 0x00FF00, 2);   // Green
sleep(1);
ui.close(fd);

// Test encryption
logset(0); loadlib"dll/crypt.dll";
hash = tls.md("sha256", "hello world");
trace("SHA256: " + tls.bts(hash));

// Test serial port
logset(0); loadlib"dll/uart.dll";
ports = uart.scan(0);
trace("Available ports: " + ports);

// Test TTS
logset(0); loadlib"dll/tts.dll";
tts.set(100, 0);              // Volume 100, normal rate
tts.pay("Hello Zhiye", 1);   // Blocking speech

// Test JSON
logset(0); loadlib"dll/TextFlow.dll";
json = '{"name":"test","val":123}';
name = json.press(json, "name");
trace("name: " + name);

// Test text processing
logset(0); loadlib"dll/TextFlow.dll";
s = "hello[world]end";
inner = str.betweenb(s, "[", "]");
trace("between: " + inner);   // "world"
```

### 5.5 DLL Development One-Click Test
```bash
# Build → Deploy → Test in one command
MSBuild examples/dll_nlu3/ThirdLib.sln /p:Configuration=Release /p:Platform=x64 /v:quiet && \
cp examples/dll_nlu3/x64/Release/gThirdLib.dll nlu_run_64bit/dll/ && \
cd nlu_run_64bit && ./_zhiye.exe 1234 Ycode.c
```

---

## L6 · Full DLL Library Catalog (Continuously Expanding)

> The following libraries are supported by the Zhiye platform. More libraries will be added through ongoing source development.

### Built-in DLLs in This Project (nlurh)
| DLL | ArrayName | Interfaces | 32-bit | 64-bit | Function |
|-----|-----------|------------|--------|--------|----------|
| ui.dll | ui | 14 | ✅ | ✅ | Window creation + image processing (bmp/png/jpg/tga/webp) |
| crypt.dll | tls | 27 | ✅ | ✅ | Encryption/hash/sign/certs (md5/sha1/sha256/sm3/des/aes/sm4/sm2/rsa) |
| uart.dll | uart | 7 | ✅ | ✅ | Serial port communication (COM ports) |
| tts.dll | tts | 3 | ✅ | ✅ | Windows TTS speech synthesis + WAV generation |
| mark.dll | mk | 5 | ✅ | ✅ | Image annotation (requires ui+freetype+TextFlow) |
| TextFlow.dll | str/json | 10 | ✅ | ✅ | Text processing + JSON parsing |
| freetype.dll | ft | — | ✅ | ✅ | FreeType font rendering engine |

### Full Module List (nlu2_all_dll source development repo)
| Module | DLL | ArrayName | Ifaces | Function |
|--------|-----|-----------|--------|----------|
| nlu2_cam | cam.dll | cam | 10 | Camera capture (Media Foundation) |
| nlu2_ui | ui.dll | ui | 14 | Window + image |
| nlu2_gui | gui.dll | — | 15 | GUI enhanced (fonts + drawing) |
| nlu2_lz | lz.dll | lz | 6 | LZMA2+LZ4HC+Zstd triple compression |
| nlu2_zip | zip.dll | zip | 4 | LZMA2+Zstd compress/decompress |
| nlu2_crypt | crypt.dll | tls | 27 | Encryption/hash/certificates |
| nlu2_uart | uart.dll | uart | 7 | Serial port communication |
| nlu2_tts | tts.dll | tts | 3 | Speech synthesis |
| nlu2_mark | mark.dll | mk | 5 | Image annotation |
| nlu2_TextFlow | TextFlow.dll | str/json | 10 | Text processing + JSON |
| nlu2_asr | asr.dll | asr | 8 | Windows online speech recognition (WinRT) |
| nlu2_vosk | vosk.dll | vosk | 7 | Vosk offline speech recognition (local model) |
| nlu2_freetype | freetype.dll | ft | — | FreeType font rendering |
| nlu2_gfs | gfs.dll | — | — | Embedded flash filesystem (wear-leveling + encryption) |
| nlu2_gtls | gtls.dll | — | — | TLS/SSL secure transport (mbedtls) |
| nlu2_filepross | — | — | — | File processing |
| nlu2_stdio | — | — | — | Standard I/O |
| nlu2_I90 | master_dll.dll | — | — | I90 terminal (EMV payment + MOS SDK) |
| nlu2_zmap | — | — | — | Map functionality |
| opencv | — | — | — | OpenCV computer vision |
| ollama | — | — | — | Ollama AI model integration |
| wdftax | — | — | — | Tax module |
