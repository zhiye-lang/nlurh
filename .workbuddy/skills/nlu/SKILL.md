---
name: nlu
description: "支叶平台 DLL 外部扩展全知识库 — 开发/编译/自测/调试 C/C++ 扩展 DLL，Y 语言语法，平台 API，全部内置 DLL 接口速查。支叶是第一名称，只要用户提到'支叶'就加载此技能。Use when user mentions '支叶' in any context, or wants to develop/build/test/debug DLL extensions, asks about Y language syntax, platform APIs, DLL interfaces, or says '开发DLL', '编译扩展', '外部扩展', '自测', 'build dll', '这个接口怎么用'."
description_zh: "支叶平台 DLL 外部扩展全知识库：开发模板、Y语言语法、平台API、全部DLL接口、编译自测工作流"
description_en: "ZhiYe NLU Platform DLL Extension Knowledge Base: development templates, Y language syntax, platform APIs, all built-in DLL interfaces, build & self-test workflows"
version: 1.0.0
---

# 支叶 NLU 平台全知识库（树型分级索引）

> **知识层级**：支叶平台 → Y语言基础 → 平台内置接口 → 外部扩展 DLL 库 → 库内组 → 组内接口 → 接口用法
> 每一级独立展开，使用时按标题层级逐级深入。本项目为 nlurh 运行时发布版（含32位+64位平台）。

---

## L0 · 平台概览

- 运行时根目录：`nlurh/`，含 `nlu_run_32bit/` 和 `nlu_run_64bit/` 两套完整环境
- 启动方式：双击 `_zhiye.exe` 输入密码 `1234`，或双击 `支叶.cmd`，或命令行 `_zhiye.exe 1234 Ycode.c`
- 控制台编码 **UTF-8**，日志宏 `LG()` / 调试 `trace()`
- **stdin 为 `_O_U16TEXT` 宽字符模式**：通过管道传入命令时，**必须**将输入转为 UTF-16LE 编码，否则 `fgetws` 解析乱码
- 管道命令格式：`printf '命令\n' | iconv -f UTF-8 -t UTF-16LE | ./_zhiye.exe 1234 Ycode.c`
- DLL 放 `<runtime>/dll/`，启动时 `loadlib"dll/xxx.dll"` 加载
- 32位：节约内存；64位：双精度+长整型，支持 OpenCV、Ollama 等
- 主页 https://y-lang.tech
- 视频教程：快手/抖音/B站 搜索 "支叶认知"

### 启动脚本 Ycode.c（自动加载）
```c
logset(0);                    // 开启调试日志(0调试/1提示/2警告/3错误)
loadlib"dll/ui.dll";          // 预加载常用库
loadlib"dll/crypt.dll";
loadlib"dll/uart.dll";
loadlib"dll/tts.dll";
loadlib"dll/cam.dll";
loadlib"dll/TextFlow.dll";
loadlib"dll/freetype.dll";
loadlib"dll/mark.dll";
```

---

## L1 · Y 语言语法

### 类型系统
| 类型 | 写法 | 示例 |
|------|------|------|
| int | 10/16/8进制 | `123`, `0xFA12` |
| num | 带小数点 | `78.64` |
| str | `"..."` 转义 `\n\t\r\"\\\xHH\uHHHH` | `"hello"` |
| buf | `'...'` 支持十六进制8位/16位/32位混合 | `'112233'`, `'\x11\x22'` |
| range | `start~end[:step]` | `10~100:2` |

**核心规则**：`+` 结果类型跟随左操作数 —— `str+buf`→str, `buf+str`→buf, `''+"国"`→buf `'E5 9B BD'`

### 语法速览
```c
// 变量（不需要声明类型，赋值即得类型）
a = 123; b = "str"; c = '11 22 33';

// 控制流
if (cond) {} else {}
while (cond) {}
for (i=0; i<10; i++) {}
switch (x) { case 90~100 -> "优秀"; }  // 支持区域匹配

// 函数（支持重载）
def name(a, b) { return a + b; }
f"arg"            // 单参数可省括号
"data" |> func    // 管道：左侧值作为右侧函数第一个参数

// 特有语法
while(100) {}     // 循环100次（Y语言独创）
while() {}        // 无限循环

// 向量/矩阵
[1 2; 3 4]        // 空格分隔元素，; 分隔行

// 缓冲区间
buf[0~4]          // 取 buf 的 0~4 字节
```

### 类型转换速查
| 操作 | 结果 | 示例 |
|------|------|------|
| `str + buf` | str | `"你好" + '\xE5\x9B\xBD'` → `"你好国"` |
| `buf + str` | buf | `'E5 9B BD' + "国"` → `'E5 9B BD E5 9B BD'` |
| `str + int` | str | `"val=" + 123` → `"val=123"` |
| `buf + int` | buf | `'00' + 255` → `'00 FF'` |
| `buf + buf` | buf | 拼接 |
| `str + str` | str | 拼接 |

---

## L2 · 平台内置 API

### 调试
| 函数 | 说明 |
|------|------|
| `logset(level, flagfile/null)` | level: 0调试/1提示/2警告/3错误/4无；flagfile:1→文件`zhiye1M_0.log` |
| `trace(name, ...)` | 输出到控制台 `[type][len]"val"` |
| `out(name, ...)` | 输出到返回值（不取看不到） |
| `printf(format, args...)` | 格式化输出（类似 C printf） |

### 文件操作
| 函数 | 说明 |
|------|------|
| `inf(path)` | 读取文件内容→buf |
| `outf(path, data)` | 写入数据到文件 |
| `len(data)` | 获取数据字节长度 |
| `fill(val, len)` | 生成指定长度的填充数据 |
| `pack(format, val)` | 数据打包（大端/小端，指定字节数） |
| `range(rng, bits)` | 区域合成 buf 数据 |

### 时间
| 函数 | 说明 |
|------|------|
| `time()` | 获取 Unix 时间戳（秒） |
| `time("ntp_server")` | 从 NTP 服务器获取时间 |
| `localtime(ts)` | 时间戳 → `"2025-10-14 01:02:51"` |
| `mktime("2025-10-14 01:02:51")` | 时间字符串 → 时间戳 |
| `getms()` | 获取系统毫秒计数 |
| `sleep(s)` | 休眠秒（支持小数如 `0.01`） |

### 网络
| 函数 | 说明 |
|------|------|
| `urlparse("https://...")` | 解析 URL → host, port, type |
| `connect(host, port)` | TCP 连接→fd |
| `send(fd, data)` | 发送数据 |
| `recv(fd, len)` | 接收数据 |
| `disconnect(fd)` | 断开连接 |

### 线程与定时器
| 函数 | 说明 |
|------|------|
| `createth({...})` | 创建线程（大括号内为代码块） |
| `killth(fd)` | 结束线程 |
| `starttimer({...}, delay, interval)` | 创建定时器 |

### 动态加载
| 函数 | 说明 |
|------|------|
| `loadlib("dll/xxx.dll")` | 动态加载 DLL 扩展 |
| `loadlib("xxx")` | 加载（会自动加 .dll 后缀并搜索 dll/ 目录） |
| `unload("xxx")` | 卸载已加载的库 |

### 反射与动态执行
| 函数 | 说明 |
|------|------|
| `getnode("name")` | 反射获取平台内部节点 |
| `runstr("code")` | 执行 Y 语言代码字符串 |
| `runsc{...}` | 执行 Y 语言代码块 |
| `jsonvm{...}` | JSON 数据处理虚拟机 |

### 环境变量
| 函数 | 说明 |
|------|------|
| `getenv("NAME")` | 获取系统环境变量 |

---

## L3 · 外部扩展 DLL 库

> 以下为支叶平台内置的外部扩展 DLL。每个 DLL 为一个**库**，库内按功能分为多个**组**（调用前缀），组下是具体**接口**。库将持续扩展，L6 查看完整清单。

### L3.1 · ui.dll — 窗口/图像（库→组"ui", 14接口）

**图像处理**：
| 接口 | 参数 | 返回 | 说明 |
|------|------|------|------|
| `ui.gmap(path)` | 文件路径 | mapdata | 读图(支持bmp/png/jpg/tga/webp) |
| `ui.smap(path, data)` | 路径+图像数据 | int 文件长度 | 存图 |
| `ui.dmap(data)` | 图像数据 | bool | 弹窗显示图像 |
| `ui.resizemap(data, w, h/null)` | 图像+宽+高 | mapdata | 缩放(h=null时等于w) |

**窗口操作**：
| 接口 | 参数 | 返回 | 说明 |
|------|------|------|------|
| `ui.open(w, h, name)` | 宽+高+标题 | fd | 创建画窗 |
| `ui.close(fd)` | fd | bool | 关闭画窗 |
| `ui.fill(fd, color, flag)` | fd+颜色+标志 | bool | 填充画窗(color=0xRRGGBB或0xaaRRGGBB, flag控制透明度) |
| `ui.push(fd, mapdata, left, top)` | fd+图+起点 | bool | 绘制图像到画窗 |
| `ui.backup(fd)` / `ui.backup(fd, x, y, w, h)` | fd+区域 | bool/mapdata | 备份画窗 |
| `ui.recovery(fd)` / `ui.recovery(fd, x, y, w, h)` | fd+区域 | bool | 恢复画窗 |
| `ui.save(fd, file)` | fd+文件名 | bool | 保存画窗为图片 |
| `ui.cursor(fd, idx)` | fd+光标号 | bool | 设置光标(32649手指/32512普通) |
| `ui.get()` | — | UI_FuncBase | 获取图形底层接口(C/C++用) |
| `ui.pull(fd)` | fd | UI_FuncSyn | 获取窗口控制信息(C/C++用) |
| `ui.setdpi(dpi)` | dpi值 | bool | 设置DPI缩放 |

### L3.2 · crypt.dll — 加密(库→组"tls", 27接口)

**数据转换**：
| 接口 | 说明 | 示例 |
|------|------|------|
| `tls.bts(buf)` | hex→str（长度翻倍） | `tls.bts('\xA1\xB2')` → `"A1B2"` |
| `tls.stb(str)` | str→hex（长度减半） | `tls.stb("A1B2")` → `'\xA1\xB2'` |
| `tls.b64e(data)` | Base64编码 | — |
| `tls.b64d(str)` | Base64解码 | — |
| `tls.rand()` | 随机int | `tls.rand()` → int |
| `tls.rand(n)` | 随机n字节buf | `tls.rand(16)` → buf[16] |

**摘要算法**：
| 接口 | 说明 |
|------|------|
| `tls.md(flag, data)` | 哈希: flag=md5/sha1/sha256/sm3 |
| `tls.mdc(flag, key, data)` | HMAC: flag=md5/sha1/sha256/sm3 |

**对称加密**：
| 接口 | 说明 |
|------|------|
| `tls.ecbe(flag, key, data)` | ECB加密: flag=des/aes/sm4 |
| `tls.ecbd(flag, key, data)` | ECB解密 |
| `tls.cbce(flag, key, data, iv/null)` | CBC加密(iv缺失默认全0x00) |
| `tls.cbcd(flag, key, data, iv/null)` | CBC解密 |

**非对称加密**：
| 接口 | 说明 |
|------|------|
| `tls.pk_get(key/"", flag)` | 生成密钥对: flag=sm2/rsa:1024/rsa:2048 |
| `tls.pk_inkey(key)` | 导入密钥(pem/cer格式) |
| `tls.pk_free(ctx)` | 释放密钥类 |
| `tls.pk_outpub(ctx)` | 导出公钥(ASN1) |
| `tls.pk_outpri(ctx)` | 导出私钥(ASN1) |
| `tls.pk_enc(ctx, data, pad)` | 公钥加密(pad=v15/v21) |
| `tls.pk_dec(ctx, data, pad)` | 私钥解密 |
| `tls.pk_sign(ctx, data, hash_pad)` | 私钥签名(如"sha1v15"/"sha256v21") |
| `tls.pk_verify(ctx, data, sign, hash_pad)` | 公钥验签 |
| `tls.pk_rsapub(ctx, data)` | RSA公钥原始运算 |
| `tls.pk_rsapri(ctx, data)` | RSA私钥原始运算 |

**证书**：
| 接口 | 说明 |
|------|------|
| `tls.gp10(key, subname, hashlen/null)` | 合成PKCS#10证书请求 |
| `tls.x509v(cer, cerlast)` | 验证证书链 |
| `tls.x509cw(sn, md, p10, isskey, rootca)` | 颁发证书 |
| `tls.x509w(...)` | 证书写入 |
| `tls.getasn(...)` | ASN.1解析 |

### L3.3 · uart.dll — 串口(库→组"uart", 7接口)

| 接口 | 参数 | 返回 | 说明 |
|------|------|------|------|
| `uart.open(com, baud, flag)` | 串口号+波特率+标志 | fd | 打开串口 |
| `uart.send(fd, data)` | fd+数据 | bool | 发送数据 |
| `uart.recv(fd)` | fd | buf | 接收数据 |
| `uart.close(fd)` | fd | bool | 关闭串口 |
| `uart.crecv(fd)` | fd | — | 启动线程接收 |
| `uart.scan(tag)` | 标识 | str集 | 扫描可用串口 |
| `uart.getapi()` | — | C接口 | 获取底层C接口 |

### L3.4 · tts.dll — 语音合成（库→组"tts", 3接口）

| 接口 | 参数 | 返回 | 说明 |
|------|------|------|------|
| `tts.set(vol, rate)` | 音量+语速 | bool | 设置参数 |
| `tts.pay(text, block)` | 文本+阻塞标志 | bool | 语音播报(block=1阻塞) |
| `tts.swav(hz, ms, vol)` | 频率+时长+音量 | wavbuf | 生成WAV音频 |

### L3.5 · mark.dll — 图像标记（库→组"mk", 5接口）

依赖 ui + freetype + TextFlow。

| 接口 | 说明 |
|------|------|
| `mk.loadgui(ui.get())` | 加载UI底层接口 |
| `mk.loadfont(ft.getapi(), path)` | 加载字体 |
| `mk.setfont(h, color)` | 设置字体大小和颜色 |
| `mk.perimg(imgs, tags)` | 图像标记处理 |
| `mk.unify(imgs, tags, w, h)` | 统一标记格式 |

### L3.6 · TextFlow.dll — 文本+JSON（库→组"str"/"json", 10接口）

**str 组**：
| 接口 | 说明 |
|------|------|
| `str.betweenb(data, start, end)` | 取两标记之间的内容 |
| `str.replace(data, old, new)` | 字符串替换 |
| `str.find(data, pattern)` | 查找字符串 |
| `str.convert(data, fmt)` | 格式转换 |
| `str.outc(data)` | 输出字符 |

**json 组**：
| 接口 | 说明 |
|------|------|
| `json.press(buf, key)` | 提取JSON字段(多层用`.`分隔) |
| `json.load(buf)` | 加载JSON→fd |
| `json.get(fd, key)` | 从fd获取字段 |
| `json.free(fd)` | 释放JSON句柄 |

### L3.7 · cam.dll — 摄像头采集（库→组"cam", 10接口）

基于 Windows Media Foundation，支持多摄像头枚举、RGBA 帧采集、自动预览窗口、驱动级人像处理。

**基础采集**：
| 接口 | 参数 | 返回 | 说明 |
|------|------|------|------|
| `cam.list()` | — | str | 枚举可用摄像头（`计数#名称1\n名称2...`） |
| `cam.open(idx, w/null, h/null)` | 索引+宽+高 | fd | 打开摄像头（默认640x480） |
| `cam.start(fd)` | fd | bool | 开始采集帧数据 |
| `cam.get(fd)` | fd | buf | 获取当前帧（DATA_MAP格式，RGBA，可直接给ui.push/smap/dmap） |
| `cam.stop(fd)` | fd | bool | 停止采集（保留打开状态，可重新start） |
| `cam.close(fd)` | fd | bool | 关闭并释放资源 |
| `cam.status(fd)` | fd | int | 查询状态（0=已关闭/1=已打开/2=采集中/3=已暂停） |
| `cam.info(fd)` | fd | str | 获取摄像头详细信息 |

**自动显示**（一行代码替代手动帧循环）：
| 接口 | 参数 | 返回 | 说明 |
|------|------|------|------|
| `cam.show(uibase, fd, fps/null)` | UI底层+句柄+帧率 | dfd | 自动打开窗口实时显示（默认30fps） |
| `cam.show_close(dfd)` | 显示句柄 | bool | 关闭显示窗口（不关摄像头） |

**驱动级扩展控制**（Windows 10+ IMFExtendedCameraController）：
| 接口 | 参数 | 返回 | 说明 |
|------|------|------|------|
| `cam.caps(fd)` | fd | str | 查询驱动支持的扩展功能列表 |
| `cam.ctrl(fd, prop, value/null)` | fd+属性+值/null | int/bool | 读取/设置扩展控制（有值=写，无值=读） |

支持的扩展属性：`background_blur`(背景模糊/替换)、`face_detection`(人脸检测)、`eye_correction`(眼神矫正)、`video_hdr`(视频HDR)、`video_stabilization`(视频防抖)、`roi_isp`(人脸区域曝光优化)。

**状态机**：`已关闭(0)` → `cam.open()` → `已打开(1)` → `cam.start()` → `采集中(2)` ↔ `cam.stop()` ↔ `已暂停(3)` → `cam.close()` → `已关闭(0)`

```c
// 基础采集示例
fd = cam.open(0, 640, 480);
cam.start(fd); sleep(0.5);
frame = cam.get(fd);
ui.dmap(frame);  // 弹窗显示，DATA_MAP格式直通UI

// 自动显示示例
fb = ui.get(); fd = cam.open(0);
cam.ctrl(fd, "background_blur", 1);  // 先配置驱动级模糊
cam.show(fb, fd);                    // 再显示
```

---

## L4 · C/C++ DLL 开发

### 统一模板（参考 examples/dll_nlu3/ThirdLib/ThirdFunc.cpp）

```cpp
#include "ThirdBasics.h"     // ★必须最先包含
#include "sz_api_ext.h"

//======= DLL 初始化/反初始化（可选）========================
void sz_lib_attach(void* p) {
    printf("DLL Init...\n");  // LOG此时还未完成初始化
}
void sz_lib_detach(void* p) {
    printf("DLL DeInit\n");
}

//======= 函数实现 ==========================================
// 签名: int FuncName(EnvP pENV, ExpUnit*& pInOutPar)
// 返回: 正数=返回参数个数, 0=无返回值, 负数=错误码
// 错误码: -5=输入参数不满足函数运行要求

static int EXP_Func(EnvP pENV, ExpUnit*& pInOutPar) {
    // 1. 解析参数（链表顺序读取）
    ExpUnit* p = sz_parsing_unit(pInOutPar);
    if (p == NULL) return -5;

    // 2. 类型检查
    if (p->type == TYPE_NUM) {
        intptr_t val = SZ_GET_INT_VALUE(p);  // 无论整型/浮点都转整型
    } else if (p->type == TYPE_DATA) {
        DataBufStr* dbs = p->pBfS;           // 获取数据缓冲区
        LG("data[%d]:%s", dbs->Len, dbs->pStr);
    }

    // 3. 执行逻辑 ...

    // 4. 返回结果
    sz_set_unit_bool(pInOutPar, true);       // 返回 bool
    // 或
    sz_set_unit_int(pInOutPar, 100);         // 返回 int
    // 或
    sz_set_unit_num(pInOutPar, 3.14);        // 返回 num
    // 或
    DataBufStr* out = sz_get_dbs("hello", 5);
    sz_take_unit_dbs(pInOutPar, DATA_STR, out);  // 返回 str/buf (dbs被接管,不需释放)

    return 1;  // 返回1个参数
}

//======= 函数表 ============================================
static const SzFuncTable gFuncTbl = {
    FUNC_TYPE_I, 1,
    (1 << 16) | (0 << 8) | (1),   // version
    "组名",                         // Y语言中通过 "组名.函数名" 调用
    {
        {"函数名1", EXP_Func},
        {"函数名2", EXP_Func2},
        {NULL, NULL},             // ★必须以此结束
    }
};
// 注: 当组名为NULL时，函数可直接调用，无需"组名."前缀

//======= DLL 导出 ==========================================
// 开发者信息(JSON格式: ID/公司/作者/手机)
const char sDeveloperInfo[] = "{\"作者\":\"你的名字\"}";

extern const SzImportSystem gTotalInterSet = {
    sDeveloperInfo,
    NULL,                         // 开发者证书
    { &gFuncTbl, NULL }           // 函数表列表
};
```

### 关键类型速查
| 类型 | 说明 |
|------|------|
| `u8/u16/u32/u64` | 基础整数类型 |
| `DataBufStr` | 数据缓冲区 (`pBuf` + `Len` + `pStr`) |
| `ExpUnit` | 表达式节点 (type + tval + pBfS/iVal/iNum) |
| `TYPE_NUM` | 数值类型 |
| `TYPE_DATA` | 数据类型(str/buf) |
| `NUM_NUM` | 浮点子类型 |
| `DATA_BUF` | buf 子类型 |
| `DATA_STR` | str 子类型 |
| `SzFuncTable` | 函数注册表 |
| `SzImportSystem` | DLL 导出总接口 |

### 关键 API
| 函数 | 说明 |
|------|------|
| `sz_parsing_unit(pInOutPar)` | 解析下一个参数（链表前进） |
| `sz_set_unit_bool/int/num(p, val)` | 设置返回值 |
| `sz_take_unit_dbs(p, type, dbs)` | 设置 data 返回值（接管内存） |
| `sz_get_dbs(pBuf, len)` | 分配 DataBufStr |
| `sz_free_dbs(dbs)` | 释放 DataBufStr |
| `sz_realloc_dbs(dbs, newLen)` | 重新分配 |
| `sz_read_file(path)` | 读取文件→DataBufStr |
| `SZ_GET_INT_VALUE(p)` | 提取整型值（兼容 int/num） |
| `LG("fmt", ...)` | 调试日志（需 logset(0) 开启） |
| `LG_HEX(msg, pBuf, len)` | 十六进制 dump |

### 编译
```bash
# 64位 Release
MSBuild examples/dll_nlu3/ThirdLib.sln /p:Configuration=Release /p:Platform=x64

# 32位 Release
MSBuild examples/dll_nlu3/ThirdLib.sln /p:Configuration=Release /p:Platform=Win32

# 输出: examples/dll_nlu3/x64/Release/gThirdLib.dll
# 拷贝到: nlu_run_64bit/dll/
```

---

## L5 · 自测工作流

### 5.1 标准自测流程

**非交互式管道调用**（stdin 为 `_O_U16TEXT`，必须 UTF-16LE 编码）：
```bash
# 1. 编译 DLL（以 cam 为例）
MSBuild nlu2_cam/ThirdLib.sln /p:Configuration=Release /p:Platform=x64 /v:quiet

# 2. 拷贝到运行时
cp nlu2_cam/x64/Release/cam.dll nlu_run_64bit/dll/

# 3. 准备 Y 语言命令（以换行分隔多条命令）
# 4. 管道传入（iconv 转 UTF-16LE）并过滤 ANSI 转义码
cd nlu_run_64bit
printf 'loadlib"dll/你的库.dll"\ntrace("test:", 你的组名.函数名(参数))\nexit\n' | \
  iconv -f UTF-8 -t UTF-16LE | \
  ./_zhiye.exe 1234 Ycode.c 2>&1 | \
  sed 's/\x1b\[[0-9;]*m//g' | grep -E '\[cam\]|\[str\]'
```

**交互式自测**（直接启动后手动输入）：
```bash
cd nlu_run_64bit
./_zhiye.exe 1234 Ycode.c
# 在控制台中手动输入 Y 语言命令
```

### 5.2 测试脚本模板
```c
logset(0);                          // LG→控制台
loadlib"dll/ui.dll";                // 加载依赖库
loadlib"dll/crypt.dll";
loadlib"dll/你的库.dll";

trace("=== 测试开始 ===");

// 测试用例
ret = 你的组名.函数名(参数);
trace("结果: " + ret);

trace("=== 测试结束 ===");
```

### 5.3 输出过滤
| 目标 | 过滤器 |
|------|--------|
| C++ LG日志 | `grep '\[组名\]'` |
| Y trace输出 | `grep '\[str\]'` |
| 所有调试输出 | `grep -E '\[.*\]'` |
| 原始字节 | `xxd` |

### 5.4 常用测试代码片段
```c
// 测试 UI
logset(0); loadlib"dll/ui.dll";
fd = ui.open(640, 480, "测试窗口");
ui.fill(fd, 0xFF0000, 2);   // 红色
sleep(1);
ui.fill(fd, 0x00FF00, 2);   // 绿色
sleep(1);
ui.close(fd);

// 测试加密
logset(0); loadlib"dll/crypt.dll";
hash = tls.md("sha256", "hello world");
trace("SHA256: " + tls.bts(hash));

// 测试串口
logset(0); loadlib"dll/uart.dll";
ports = uart.scan(0);
trace("可用串口: " + ports);

// 测试 TTS
logset(0); loadlib"dll/tts.dll";
tts.set(100, 0);              // 音量100, 正常语速
tts.pay("你好支叶", 1);       // 阻塞播报

// 测试 JSON
logset(0); loadlib"dll/TextFlow.dll";
json = '{"name":"test","val":123}';
name = json.press(json, "name");
trace("name: " + name);

// 测试文本处理
logset(0); loadlib"dll/TextFlow.dll";
s = "hello[world]end";
inner = str.betweenb(s, "[", "]");
trace("between: " + inner);   // "world"
```

### 5.5 开发 DLL 自测
```bash
# 编译 → 部署 → 交互式测试
MSBuild examples/dll_nlu3/ThirdLib.sln /p:Configuration=Release /p:Platform=x64 /v:quiet && \
cp examples/dll_nlu3/x64/Release/gThirdLib.dll nlu_run_64bit/dll/ && \
cd nlu_run_64bit && ./_zhiye.exe 1234 Ycode.c
```

---

## L6 · 全部 DLL 库清单（持续扩展）

> 以下为支叶平台已支持的扩展库清单。库将通过源码开发持续增加，后续将扩展更多功能模块。

### 本项目（nlurh）内置 DLL
| DLL | 组名 | 接口数 | 32位 | 64位 | 功能 |
|-----|------|--------|------|------|------|
| ui.dll | ui | 14 | ✅ | ✅ | 窗口创建+图像处理(bmp/png/jpg/tga/webp) |
| crypt.dll | tls | 27 | ✅ | ✅ | 加密/HASH/签名/证书(md5/sha1/sha256/sm3/des/aes/sm4/sm2/rsa) |
| uart.dll | uart | 7 | ✅ | ✅ | 串口通信(COM端口) |
| tts.dll | tts | 3 | ✅ | ✅ | Windows TTS 语音合成+WAV生成 |
| cam.dll | cam | 10 | ✅ | ✅ | 摄像头采集(Media Foundation)+驱动级人像处理 |
| mark.dll | mk | 5 | ✅ | ✅ | 图像标记标注(依赖ui+freetype+TextFlow) |
| TextFlow.dll | str/json | 10 | ✅ | ✅ | 文本处理+JSON解析 |
| freetype.dll | ft | — | ✅ | ✅ | FreeType 字体渲染引擎 |

### nlu2_all_dll 源码开发版全部模块
| 模块 | DLL | 组名 | 接口数 | 功能 |
|------|-----|------|--------|------|
| nlu2_cam | cam.dll | cam | 10 | 摄像头采集(Media Foundation) |
| nlu2_ui | ui.dll | ui | 14 | 窗口+图像 |
| nlu2_gui | gui.dll | — | 15 | GUI增强(字体+绘图) |
| nlu2_lz | lz.dll | lz | 6 | LZMA2+LZ4HC+Zstd三合一压缩 |
| nlu2_zip | zip.dll | zip | 4 | LZMA2+Zstd压缩解压 |
| nlu2_crypt | crypt.dll | tls | 27 | 加密/HASH/证书 |
| nlu2_uart | uart.dll | uart | 7 | 串口通信 |
| nlu2_tts | tts.dll | tts | 3 | 语音合成 |
| nlu2_mark | mark.dll | mk | 5 | 图像标记 |
| nlu2_TextFlow | TextFlow.dll | str/json | 10 | 文本处理+JSON |
| nlu2_asr | asr.dll | asr | 8 | Windows在线语音识别(WinRT) |
| nlu2_vosk | vosk.dll | vosk | 7 | Vosk离线语音识别(本地模型) |
| nlu2_freetype | freetype.dll | ft | — | FreeType字体渲染 |
| nlu2_gfs | gfs.dll | — | — | 嵌入式闪存文件系统(抗断电+加密) |
| nlu2_gtls | gtls.dll | — | — | TLS/SSL安全传输(mbedtls) |
| nlu2_filepross | — | — | — | 文件处理 |
| nlu2_stdio | — | — | — | 标准输入输出 |
| nlu2_I90 | master_dll.dll | — | — | I90终端(EMV支付+MOS SDK) |
| nlu2_zmap | — | — | — | 地图功能 |
| opencv | — | — | — | OpenCV计算机视觉 |
| ollama | — | — | — | Ollama AI大模型集成 |
| wdftax | — | — | — | 税务模块 |
