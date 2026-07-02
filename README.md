<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/README_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>

------
# Zhiye.Unity

## Overview

Zhiye is our company's **human-machine interaction core platform** developed for natural language understanding. Its core interaction paradigm is called **Y Language** — a minimal, direct human-machine interaction syntax.

> **Core Philosophy**: Y Language is a **dynamic interaction syntax** that sits between natural language and machine instructions. It is designed for **real-time teaching and interaction** between humans and machines — no pre-compilation required, real-time understanding, real-time execution.

**Traditional Programming Languages vs Y Language (Interaction Syntax)**

| Dimension | Traditional Programming Languages | Y Language |
| :--- | :--- | :--- |
| **Purpose** | Writing fixed-logic programs | Dynamically teaching the machine to understand new instructions |
| **Execution Model** | Compile → Link → Run | **Direct conversational interaction, real-time understanding and execution** |
| **Extensibility** | Modify code, recompile | "Teach" the system new rules through natural language |
| **Core Value** | Precise control of computers | **Natural collaboration between humans and machines, like a conversation** |

---
> **Memory & Continuous Learning**: The Zhiye Unity platform possesses **human-like memory capabilities**. No interaction is isolated — each is built on the accumulated knowledge from past interactions. The platform remembers the instructions, rules, and logic the user has taught it. The more you use it, the better it understands you, achieving true **continuous evolution**.

### Project Directory Structure

```lua
nlurh/           -- Project root (Zhiye.Unity)
├── nlu_run_32bit/      -- 32-bit runtime environment
│   ├── dll/            # 32-bit component libraries (do NOT mix with 64-bit)
│   ├── res/            # Resources (core store, service parameters, etc.)
│   ├── _zhiye.exe      # Zhiye main executable (auto-loads core store at runtime)
│   ├── Ycode.c         # Y Language runtime code (specified as initial execution code for _zhiye.exe in "支叶.cmd")
│   └── 支叶.cmd        # Zhiye bootstrap script (auto-configures password → decrypts core store → launches _zhiye.exe and loads Ycode.c)
├── nlu_run_64bit/      -- 64-bit runtime environment (same structure as 32-bit)
│   ├── dll/            # 64-bit component libraries
│   ├── res/            # Resources
│   ├── _zhiye.exe      # Zhiye main executable
│   ├── Ycode.c         # Y Language runtime code
│   └── 支叶.cmd        # Zhiye bootstrap script
├── docs/               -- Development documentation
│   ├── dll/            # Component library usage guides (corresponds to nlu_run_??bit/dll folder)
│   ├── ZhiyeYl.md    # Y Language basic introduction
│   └── ZhiyeInterfaces.md   # Zhiye Unity platform interface documentation
└── examples/           -- Sample code projects
    └── dll_nlu3       # Third-party C/C++ extension development example (for detailed structure see: examples/dll_nlu3/README.md)
```

### Main Executable Name

    Zhiye (zhiye): 32-bit runtime environment nlu_run_32bit (memory efficient); 64-bit runtime environment nlu_run_64bit (double-precision and long integer data, supports openCV, ollama and other platform integration); third-party C/C++ library example project dll_nlu3.

### Introduction

    Zhiye Unity provides a foundational platform with self-defined rules. It primarily achieves human-machine interaction through Y Language — no pre-compilation needed, compile directly and execute directly. Scripts can also guide the _zhiye.exe main program to pre-load or pre-execute Y Language code. Y Language supports expressions, parameters, functions, equations, and logical expressions. It supports overloading, mapping, unlimited associations, and association analysis. The platform also supports Y Language threads, timers, and more. It supports natural language-based associative self-learning and self-evolution.

### Runtime Environments

    Data centers, large servers, personal computers, mobile phones, automotive computers, smart robots

### Usage Instructions

1.  Double-click to run the Zhiye main program "_zhiye.exe" and enter the password "1234" to run automatically (or use "支叶.cmd" to bootstrap). Then enter the content to execute. You can define functions directly, or inject functionality through C/C++ programs. For more tutorials, search and follow the official account **"支叶认知"** on **Kwai**, **TikTok**, and **Bilibili**.

### C/C++ Library Integration

1.  Load via the platform's built-in `loadlib` function; release via `unload`. (Library development sample code: examples → dll_nlu3)

### Important Notes

1.  Zhiye can accept human-language-style education as well as machine-language-style definitions (via scripts or C/C++ programs).
2.  All C/C++ functions and script functions in Zhiye can be invoked by their function name. Usage is flexible — place corresponding parameters in parentheses afterward. When there is only one parameter, the parentheses can be omitted. All functions support overloading.
3.  During execution, all parameters and variables can be used directly (no type declaration required — the type is assigned upon value assignment, similar to Python).
4.  **Memory & Continuous Learning**: Zhiye possesses human-like memory capabilities. Each interaction builds upon previously accumulated knowledge. It starts as a blank slate and progressively forms rules through user interaction. The more you use it, the better it understands you, achieving true continuous evolution.
5.  During the public beta, skills and knowledge learned by Zhiye belong to the user.
6.  Zhiye does not have behavior recognition capabilities. Any consequences arising from the user teaching it to perform illegal activities are entirely the user's responsibility.

Homepage: https://y-lang.tech or https://y-lang.org.cn

chat window source code (Tauri-based): https://gitee.com/sz-ai/zhiye_chat_ui.git

### Windows Runtime Environment

For fresh systems without Visual Studio, navigate to `win-base-env/` for environment setup instructions and installation scripts. See [win-base-env/README.md](../win-base-env/README.md).




### Zhiye Convergence — The Bridge Between AI and Everything

Zhiye is a **convergence platform**: one side connects to everything (hardware, sensors, devices), the other side connects directly to AI. **Y Language** is the universal bridge — it carries interactions between AI and Zhiye, and also orchestrates collaboration among Zhiye's internal modules. Y Language bridges everything. Zhiye dramatically lowers the barrier for industries to adopt AI, providing an efficient, infinitely scalable solution for industry AI.

Going further: after understanding Zhiye's third-party extension template, AI can **write C++ DLLs, compile them, test them, and use them — all on its own**, continuously expanding Zhiye's capability boundary. Zhiye grows infinitely alongside AI.

> **Prerequisite**: No matter which AI tool you use, you **must open the `nlurh/` folder** as the project root for the skill files to be auto-discovered.

#### AI Calls Zhiye (Using Existing Capabilities)

| Method | Description | Example |
|--------|-------------|---------|
| Y Language direct | AI writes Y Language code to control Zhiye's existing DLLs | `cam.open(0)` opens camera, `tts.pay("Hello")` speaks |
| Skill lookup | AI uses built-in Skills to quickly query any DLL interface and Y Language usage | "How does ui.dll draw?", "How to use crypt.dll SM2 signing?" |

#### AI Evolves Zhiye (Develop, Test, Use New Capabilities)

| Stage | What AI Does |
|-------|-------------|
| Develop | AI references `examples/dll_nlu3/` template to generate new C++ extension DLL code |
| Build | AI invokes MSBuild to compile → outputs .dll |
| Deploy | AI copies .dll to `nlu_run_64bit/dll/` |
| Load | AI uses `loadlib"dll/newlib.dll"` in Y Language |
| Use | AI calls new functions by group name, closing the business loop |

#### Tool Access

| Tool | Setup | Trigger |
|------|-------|---------|
| Claude Code | Terminal → `nlurh/`, run `claude` | Mention Zhiye or type `/nlu` |
| WorkBuddy | Open `nlurh/` folder with WorkBuddy | Mention Zhiye to auto-load |

> Built-in `.claude/skills/` and `.workbuddy/skills/` Skill files contain the complete knowledge base: Y Language syntax, all DLL interfaces, development templates, and self-test workflows. Works out of the box after cloning.
