<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/examples/dll_nlu3/README.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>

# Zhiye-Y Language: Third-Party Function Integration (C/C++ Dynamic Library Method)

#### Project Directory Structure

```lua
dll_nlu3/           -- Third-party extension development example
├── runtime/           #          
│   ├── sz_runtime32.lib / sz_runtime64.lib # Zhiye runtime bridge lib
│   └── sz_runtime.h   # Header for the above libs 
├── sz_api_ext.cpp / sz_api_ext.h   # sz_runtime.h C/C++ convenience wrapper
├── ThirdFunc.cpp      # C/C++ interface development example (modify this to write your own functions)
├── ThirdLib.vcxproj   # VS project configuration (platform toolset)
├── ThirdLib.sln       # VS solution file (SDK V141: compatible with V141 and above VS SDK)
└── README.md          # Build guide and example instructions

```

#### Library Compilation (Windows) Instructions
1. First, install the VS compilation environment on your computer (the example was created with VS2017).
2. Double-click to open `ThirdLib.sln` (the VS project file).
3. Modify `ThirdLib/ThirdFunc.cpp` (main interface functions, function correspondence table, author information).
4. Select: Build → Rebuild `gThirdLib`. If compilation is error-free, the `gThirdLib.dll` file will be output (this file is the dynamic library file we need).

#### Usage Instructions
1. Copy `gThirdLib.dll` to the directory where the `_zhiye.exe` file is located. Double-click to run the Zhiye main program `_zhiye.exe`. Load the DLL library using the built-in function `loadlib` (input: `loadlib("gThirdLib.dll")`). If there are no errors, the functions from `gThirdLib.dll` are now linked to Zhiye.
2. Enter the `"group_name.function_name"` from `ThirdFunc.c` along with the required parameters to execute the corresponding function. If the library is no longer needed, you can unload it using the built-in function `unload` (input `unload("gThirdLib.dll")`).
[File Content End]