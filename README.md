<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/README_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>
------

### Zhiye.Fusion



#### Project Directory Structure Description:

```lua
nlu_mt/                 -- Project Root Directory (Zhiye.Fusion)
├── nlu_run_32bit/      -- 32-bit Runtime Environment
│   ├── dll/            # 32-bit specific component libraries (Must not be mixed with 64-bit)
│   ├── res/           # Resource files (Including core repository, service parameters, etc...)
│   ├── _zhiye.exe   # Zhiye main program (Automatically loads the core repository at runtime) 
│   ├── Ycode.c     # Y language runtime code
│   └── 支叶.cmd   # Zhiye program bootstrap script 
├── nlu_run_64bit/      -- 64-bit Runtime Environment (Structure same as 32-bit)
│   ├── dll/            # 64-bit specific component libraries
│   ├── res/            # Resource files
│   ├── _zhiye.exe  # Zhiye main program
│   ├── Ycode.c     # Y language runtime code
│   └── 支叶.cmd  # Zhiye program bootstrap script
├── docs/           -- development documentation
│   ├── dll/            # External library interface description
│   ├── ZhiyeYl.md     # Zhiye Y-Language Basics
│   └── ZhiyeInterfaces.md  # Zhiye Built-in Interfaces
└── examples/           -- Example code projects
    └── dll_nlu3/       # Third-party interface development example
        ├── gRunsdk32.lib    # 32-bit development interface library (Linked during compilation)
        ├── gRunsdk64.lib    # 64-bit development interface library (Linked during compilation)
        ├── ThirdBasics.h    # Zhiye third-party integration basic interface header file ()
		├── sz_api_ext.h/cpp   # Zhiye third-party Basic SDK Encapsulation
        ├── ThirdFunc.cpp    # Zhiye third-party integration interface C++ example code
        ├── ThirdLib.vcxproj # VS project configuration (Platform Toolset)
        ├── ThirdLib.sln     # VS project file (SDK V143)
        └── README.md        # Compilation guide and example description
```

#### Main Program Name

    Zhiye: 32-bit runtime environment nlu_run_32bit (memory efficient); 64-bit runtime environment nlu_run_64bit (supports double-precision and long integer data, integration with platforms like openCV, ollama, etc.); third-party C/C++ library example project dll_nlu3.

#### Introduction
    At the beginning of its operation, the Zhiye program is like a seed. It takes root and sprouts through interaction with humans, continuously learning knowledge and ideas.
    It can handle various intellectual tasks for humans (this ability needs to be taught by the user or shared with others). When paired with a server, it can master infinite knowledge.
    In the future, it will become an essential super assistant for each of us.

#### Operating Environment
    Data centers, large servers, personal computers, mobile phones, automotive computers, intelligent robots.

#### Usage Instructions
1.  Double-click to run the main program "_zhiye.exe", enter the password "1234", and then input Chinese content (you can directly define runtime functions, or inject functions via C/C++ programs: for more usage tutorials, follow nlu_mt/wikis).

#### C++ Library Integration Instructions
1.  Load via the internal `loadlib` function and unload with `unload` (Example code: examples->dll_nlu3).

#### Special Notes
1. Zhiye can accept education in common human languages, as well as definitions via machine language (either through scripts or C/C++ programs).

2. All C/C++ functions and script functions in Zhiye can be executed by referencing their function names. Usage is relatively free-form; corresponding parameters are placed in parentheses following the name. Parentheses can be omitted when there is only one parameter. All functions support overloading.

3. During execution, all parameters and variables can be used directly (no need to define types; types are inferred upon assignment, similar to Python usage).

4. Zhiye possesses continuous learning capabilities. At the start of operation, it is like a blank slate with few rules. It forms rules through interaction with us and understands us better the more it is used.

5. During the public beta, all skills and knowledge acquired by Zhiye belong to the user.

6. Zhiye lacks the ability to discern behavior. Users are solely responsible for any consequences arising from teaching it to perform illegal actions.

   

Main version address: https://gitee.com/kebo521/nlu_main.git (Better compatibility in development)
The MT version of Zhiye is designed to be compatible with more Windows systems (with better runtime compatibility)

