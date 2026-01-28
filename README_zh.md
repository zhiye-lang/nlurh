[English](README.md)

# Y 语言

> 开源的确定性人机交互脚本语言  
> 支叶平台项目组成部分 · 免费使用、修改、分发

[![许可证: MIT](https://img.shields.io/badge/许可证-MIT-yellow.svg)](LICENSE)
[![欢迎PR](https://img.shields.io/badge/PR-欢迎-brightgreen.svg)](CONTRIBUTING.md)

Y 语言是一个**完全开源**的脚本语言，专为以下场景设计：
- 确定性人机交互协议
- NLU 流水线配置
- 可教式界面定义
- 跨应用工作流脚本

✨ **100% 开源** · 可从源码构建或使用预编译二进制  
✨ **MIT 许可证** · 商业与个人项目均可免费使用  
✨ **社区共建** · 欢迎贡献！

## 快速开始
```bash
# 克隆完整项目（含运行时源码）
git clone https://github.com/zhiye-lang/nlurh.git
cd nlurh

# Windows 构建（需 Visual Studio 2019+）
.\build.bat

# 运行 Y 语言脚本
.\bin\zhiye.exe examples/hello_world.y
```

## 项目结构

```
nlurh/
├── src/                   # 运行时源代码（C++）
│   ├── core/              # Y 语言解释器核心
│   ├── runtime/           # 平台运行时实现
│   └── main.cpp           # 入口文件（_zhiye.exe 源码）
├── examples/              # Y 语言脚本示例（.y 文件）
│   ├── hello_world.y
│   ├── Ycode.y            # 核心交互示例
│   └── nlu_demo.y
├── include/               # 嵌入式开发头文件
│   ├── ylang.h
│   └── ThirdBasics.h
├── scripts/               # 构建与运行辅助脚本
│   └── build.bat          # Windows 构建脚本
├── docs/                  # 文档（中/英）
├── LICENSE                # MIT 许可证（需保留版权信息）
├── README_zh.md           # 您正在阅读
└── TRADEMARK.md           # 商标使用指南
```

## **许可证**

采用 **[MIT 许可证](https://www.qianwen.com/chat/LICENSE)** 授予。
✅ **您可以**：

- 在商业/个人项目中自由使用
- 修改并分发衍生作品
- 嵌入到您的应用中

⚠️ **要求**：

> **在所有副本或重要部分中保留版权声明**：
> `Copyright (c) 2024 长沙思知信息科技有限公司`

## **商标说明**

“支叶”（Zhiye）和“Y 语言”是长沙思知信息科技有限公司的商标。
本项目完全开源，但**商标使用有指南**：
→ 详见 [TRADEMARK.md](https://www.qianwen.com/chat/TRADEMARK.md)（例如：“使用 Y 语言构建” 欢迎；将您的项目命名为“Y 语言专业版”则不可）

## **参与贡献**

我们欢迎贡献！
🐞 报告问题 · 💡 建议功能 · 🌍 改进文档
→ 阅读 [CONTRIBUTING.md](https://www.qianwen.com/chat/CONTRIBUTING.md) 开始参与

------

© 2024 长沙思知信息科技有限公司
[完整许可证](https://www.qianwen.com/chat/LICENSE) | [商标政策](https://www.qianwen.com/chat/TRADEMARK.md) | [English Version](https://www.qianwen.com/chat/README.md)