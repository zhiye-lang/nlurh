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
│   ├── dll/            # Component library development docs (corresponds to dll folder)
│   ├── ZhiyeYl.md    # Y Language basic introduction
│   └── ZhiyeInterfaces.md   # Zhiye Unity platform interface documentation
└── examples/           -- Sample code projects
    └── dll_nlu3/       # Third-party interface development example
        ├── gRunsdk32.lib    # 32-bit SDK import library (for linking at compile time)
        ├── gRunsdk64.lib    # 64-bit SDK import library (for linking at compile time)
        ├── ThirdBasics.h    # Zhiye third-party access basic interface header (defines interface functions/data structures)
	├── sz_api_ext.h/cpp   # Zhiye third-party Basic SDK Encapsulation
        ├── ThirdFunc.cpp    # Zhiye third-party access C++ sample code
        ├── ThirdLib.vcxproj # VS project configuration (platform toolset)
        ├── ThirdLib.sln     # VS solution file (SDK V141: compatible with V141 and above VS SDK)
        └── README.md        # Build guide and example instructions
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

Third-party library integration sample code is no longer provided separately — it is now stored directly in the main directory: examples → dll_nlu3.


### Claude Code AI Assistant (Skill Tool)

This project includes a built-in **DLL External Extension Skill** for Claude Code — download and use AI-assisted development right away.

**Setup**:
1. Install [Claude Code](https://docs.anthropic.com/en/docs/claude-code/overview) (Anthropic's official AI coding assistant)
2. Open a terminal in this project directory
3. Run `claude` and type `/nlu` to load the full knowledge base

**How to Use**:

| Method | Action | Example |
|--------|--------|---------|
| Slash command | Type `/nlu` | Load the DLL extension knowledge base, AI guides you step by step |
| Natural question | Ask directly | "What interfaces does ui.dll have?", "How do I build a serial port DLL?" |
| Build assist | Let AI compile for you | "Build dll_nlu3 for 64-bit" |

**Skill Capabilities**:
- Y Language syntax reference and examples
- All built-in DLL interface lookup (ui / crypt / uart / tts / mark / TextFlow)
- C/C++ DLL development template generation
- Build → Deploy → Test one-click workflow
- Platform API guidance (file I/O, networking, threading, timers, etc.)

**How It Works**: Claude Code automatically reads `CLAUDE.md` on startup for project context, and auto-discovers skill files in the `.claude/skills/` directory. No configuration needed — it just works out of the box.


### WorkBuddy AI Assistant (Skill Tool)

This project also includes a built-in **DLL External Extension Skill** for WorkBuddy, the Tencent AI desktop agent. Available for both domestic and international users.

> **Prerequisite**: No matter which tool you use, you **must open the `nlurh/` folder** as the project root for the skill files to be auto-discovered.

**Using WorkBuddy (International Edition)**:

1. Install [WorkBuddy](https://www.workbuddy.ai) (international) or [WorkBuddy CN](https://www.codebuddy.cn) (domestic)
2. Open the `nlurh/` folder as a project in WorkBuddy
3. Just mention **Zhiye** (or 支叶) — the skill loads automatically

| Usage | Example |
|-------|---------|
| Casual mention | "Build a serial port tool with Zhiye", "How does Zhiye handle encryption?" |
| Interface lookup | "What interfaces does ui.dll have?", "How to use crypt.dll SM2 signing?" |
| Build assist | "Build dll_nlu3 for 64-bit" |

**Skill Capabilities**:
- Y Language syntax reference and examples
- All built-in DLL interface lookup (ui / crypt / uart / tts / mark / TextFlow)
- C/C++ DLL development template generation
- Build → Deploy → Test one-click workflow
- Platform API guidance (file I/O, networking, threading, timers, etc.)


