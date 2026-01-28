# Y Language

> Open-source deterministic scripting language for human-AI interaction protocols  
> Part of the Zhiye Platform project · Free to use, modify, and distribute

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

Y Language is a **fully open-source** scripting language designed for:
- Deterministic human-AI interaction protocols
- NLU pipeline configuration
- Teachable interface definition
- Cross-application workflow scripting

✨ **100% open source** · Build from source or use pre-built binaries  
✨ **MIT licensed** · Free for commercial and personal use  
✨ **Community driven** · Contributions welcome!

## Quick Start
```bash
# Clone the entire project (includes runtime source code)
git clone https://github.com/zhiye-lang/nlurh.git
cd nlurh

# Build on Windows (requires Visual Studio 2019+)
.\build.bat

# Run a Y Language script
.\bin\zhiye.exe examples/hello_world.y
```
## Project Structure
```
nlurh/
├── src/                   # Runtime source code (C++)
│   ├── core/              # Y Language interpreter core
│   ├── runtime/           # Platform runtime implementation
│   └── main.cpp           # Entry point (_zhiye.exe source)
├── examples/              # Y Language script examples (.y files)
│   ├── hello_world.y
│   ├── Ycode.y            # Core interaction example
│   └── nlu_demo.y
├── include/               # Public headers for embedding
│   ├── ylang.h
│   └── ThirdBasics.h
├── scripts/               # Build & run helpers
│   └── build.bat          # Windows build script
├── docs/                  # Documentation (en/zh)
├── LICENSE                # MIT License (copyright notice required)
├── README.md              # You are here
└── TRADEMARK.md           # Trademark usage guidelines
```

## **License**

Distributed under the **[MIT License](https://www.qianwen.com/chat/LICENSE)**.
✅ **You may**:

- Use freely in commercial/personal projects
- Modify and distribute derivatives
- Embed in your applications

⚠️ **Requirement**:

> **Preserve the copyright notice** in all copies or substantial portions:
> `Copyright (c) 2024 Changsha Sizhi Information Technology Co., Ltd.`

## **Trademark Note**

"Zhiye" (支叶) and "Y Language" are trademarks of Changsha Sizhi Information Technology Co., Ltd.
This project is open source, but **trademark usage has guidelines**:
→ See [TRADEMARK.md](https://www.qianwen.com/chat/TRADEMARK.md) for respectful usage (e.g., "Built with Y Language" is welcome; naming your project "Y Language Pro" is not).

## **Contributing**

We welcome contributions!
🐞 Report bugs · 💡 Suggest features · 🌍 Improve documentation
→ Read [CONTRIBUTING.md](https://www.qianwen.com/chat/CONTRIBUTING.md) to get started

------

© 2024 Changsha Sizhi Information Technology Co., Ltd.
[Full License](https://www.qianwen.com/chat/LICENSE) | [Trademark Policy](https://www.qianwen.com/chat/TRADEMARK.md) | [中文文档](https://www.qianwen.com/chat/README_zh.md)
