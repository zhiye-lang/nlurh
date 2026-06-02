# nlurh — Zhiye Unity Platform Runtime (32-bit + 64-bit)

## Overview

This project is the complete runtime environment for the **Zhiye Unity Platform (ZhiYe NLU)**, including both 32-bit and 64-bit runtimes. Zhiye is a **natural language understanding human-machine interaction kernel** whose core interaction paradigm is called **Y Language** — a minimal, direct human-machine interaction syntax that requires no compilation, with real-time understanding and execution.

- **Homepage**: https://y-lang.tech
- **UI Chat Window**: https://gitee.com/sz-ai/zhiye_chat_ui.git
- **Video Tutorials**: Search "支叶认知" on Kwai/TikTok/Bilibili

## Directory Structure

```
nlurh/
├── nlu_run_32bit/          # 32-bit runtime (memory-efficient)
│   ├── _zhiye.exe          # Zhiye main executable
│   ├── Ycode.c             # Y Language script auto-loaded at startup
│   ├── 支叶.cmd             # Bootstrap script (password → decrypt → launch)
│   ├── dll/                # 32-bit DLL components (do NOT mix with 64-bit)
│   │   ├── ui.dll          # Window + image processing
│   │   ├── crypt.dll       # Encryption/hash/certificates (ArrayName "tls")
│   │   ├── uart.dll        # Serial port communication
│   │   ├── tts.dll         # Text-to-speech synthesis
│   │   ├── mark.dll        # Image annotation
│   │   ├── TextFlow.dll    # Text processing + JSON (ArrayNames "str"/"json")
│   │   └── freetype.dll    # Font rendering engine
│   └── res/                # Resource files (core store, service params, etc.)
├── nlu_run_64bit/          # 64-bit runtime (double precision, OpenCV/Ollama support)
│   └── (same structure as 32-bit)
├── docs/                   # Documentation
│   ├── ZhiyeYl.md          # Y Language complete syntax manual
│   ├── ZhiyeInterfaces.md  # Platform built-in function API reference
│   └── dll/                # DLL component usage guides
│       ├── ui.md           # UI graphics (14 interfaces)
│       ├── crypt.md        # Encryption (27 interfaces)
│       ├── uart.md         # Serial port (7 interfaces)
│       ├── tts.md          # TTS (3 interfaces)
│       ├── mark.md         # Image annotation (5 interfaces)
│       └── TextFlow.md     # Text + JSON (10 interfaces)
└── examples/               # Sample code
    └── dll_nlu3/           # Third-party DLL development example (VS project)
        ├── ThirdFunc.cpp   # Main interface sample code
        ├── ThirdBasics.h   # Base type definitions header
        ├── sz_api_ext.h    # Platform API extension header
        ├── sz_api_ext.cpp  # API extension implementation
        ├── gRunsdk32.lib   # 32-bit SDK import library
        ├── gRunsdk64.lib   # 64-bit SDK import library
        └── README.md       # Build guide
```

## Quick Start

### Launch the Platform
```bash
# Method 1: Double-click
Double-click nlu_run_64bit/_zhiye.exe → enter password "1234"

# Method 2: Script launch
Double-click nlu_run_64bit/支叶.cmd

# Method 3: Command line
cd nlu_run_64bit
_zhiye.exe 1234 Ycode.c
```

### Testing a DLL
```c
logset(0);                    // Enable debug logging
loadlib"dll/ui.dll";          // Load UI library
fd = ui.open(640, 480, "Test Window");
ui.fill(fd, 0xFF0000, 2);    // Red fill
sleep(2);
ui.close(fd);
```

## Developing Custom DLLs

Reference the complete example project in `examples/dll_nlu3/`.

### Core Template

```cpp
#include "ThirdBasics.h"      // ★ Must be included first
#include "sz_api_ext.h"

// Function signature: int FuncName(EnvP pENV, ExpUnit*& pInOutPar)
static int EXP_MyFunc(EnvP pENV, ExpUnit*& pInOutPar) {
    ExpUnit* pUnit = sz_parsing_unit(pInOutPar);   // Get first parameter
    // ... parse and process parameters ...
    sz_set_unit_bool(pInOutPar, true);              // Return result
    return 1;                                       // Number of returned params
}

// Function table
static const SzFuncTable gFuncTbl = {
    FUNC_TYPE_I, 1,
    (1 << 16) | (0 << 8) | (1),   // version
    "ArrayName",                   // Called via "ArrayName.FuncName" in Y Language
    {
        {"FuncName1", EXP_MyFunc},
        {NULL, NULL},             // ★ Required terminator
    }
};
// Note: When ArrayName is NULL, functions can be called directly without prefix

// DLL export
extern const SzImportSystem gTotalInterSet = {
    "{\"Author\":\"Your Name\"}",  // Developer info (JSON)
    NULL,
    { &gFuncTbl, NULL }
};
```

### Building
1. Install Visual Studio 2022 (with C++ Desktop Development workload)
2. Open `examples/dll_nlu3/ThirdLib.sln`
3. Select configuration (Debug/Release) and platform (Win32/x64)
4. Build → Rebuild Solution
5. Copy the output DLL to `nlu_run_64bit/dll/` (or 32bit)

### Calling from Y Language
```c
loadlib"dll/yourlib.dll";
YourArrayName.FuncName(param1, param2);
unload("yourlib");
```

## Y Language Quick Reference

### Types
| Type | Syntax | Example |
|------|--------|---------|
| int | decimal/hex | `123`, `0xFA12` |
| num | floating point | `78.64` |
| str | `"..."` | `"hello"` |
| buf | `'...'` hex | `'11 22 33'`, `'\xA1\xB2'` |
| range | `start~end:step` | `10~100:2` |

**Core Rule**: `+` result type follows the left operand — `str+buf`→str, `buf+str`→buf

### Key Syntax
```c
// Pipe: left value becomes the first argument of the right function
"data" |> func

// Single-argument shorthand (parentheses optional)
func"arg"

// Loop 100 times (Y Language unique feature)
while(100) {}

// Switch with range matching
switch(x) { case 90~100 -> "Excellent"; }

// Function definition (supports overloading)
def add(a, b) { return a + b; }
```

## Platform Built-in API Quick Reference

| Category | Function | Description |
|----------|----------|-------------|
| Dynamic Loading | `loadlib("dll/x.dll")` / `unload("x")` | Load/unload DLL |
| File I/O | `inf(f)` / `outf(f,d)` | Read/write files |
| Time | `time()` / `localtime(ts)` / `mktime(s)` | Timestamps/formatting |
| Debug | `logset(level)` / `trace(name,...)` | 0=debug/1=info/2=warn/3=error |
| Network | `connect/send/recv/disconnect` | TCP communication |
| Threading | `createth({...})` / `killth(fd)` | Create/end threads |
| Timers | `starttimer({...}, delay, interval)` | Scheduled tasks |
| Data Ops | `len(d)` / `pack(fmt,v)` / `range(rng,bits)` | Data manipulation |
| JSON | `jsonvm{...}` / `json.press(buf,key)` | JSON processing |
| Reflection | `getnode("name")` / `runstr(code)` | Reflection/dynamic exec |

## DLL Interface Quick Reference

| DLL | ArrayName | Iface Count | Key Functions |
|-----|-----------|-------------|---------------|
| ui.dll | ui | 14 | Window+image: gmap/smap/dmap/resizemap/open/close/fill/push/backup/recovery/save/cursor/get/pull |
| crypt.dll | tls | 27 | Crypto: bts/stb/b64e/b64d/rand/md/mdc/ecbe/ecbd/cbce/cbcd/pk_*/gp10/x509* |
| uart.dll | uart | 7 | Serial: open/send/recv/close/crecv/scan/getapi |
| tts.dll | tts | 3 | Speech: set/pay/swav |
| mark.dll | mk | 5 | Annotation: loadgui/loadfont/setfont/perimg/unify |
| TextFlow.dll | str/json | 10 | Text+JSON: betweenb/replace/find/convert/outc + press/load/get/free |

## Claude Code Skills

This project includes a `.claude/skills/` directory with a dedicated skill for Claude Code:

- **nlu** — DLL External Extension Knowledge Base: development templates, Y Language syntax, platform APIs, all DLL interfaces, build & test workflows

Usage: type `/nlu` in Claude Code, or ask Zhiye-related questions directly.

## Coding Standards

- All source files use **UTF-8** encoding
- `#include "ThirdBasics.h"` must be included first
- Functions must be registered via `SzFuncTable` to be callable from Y Language
- Function tables must end with `{NULL, NULL}`
- Memory management: use `sz_get_dbs()` / `sz_free_dbs()`
- Parse parameters from the `pInOutPar` linked list: `sz_parsing_unit()`
- Write return values via `sz_set_unit_*()` or `sz_take_unit_dbs()`
