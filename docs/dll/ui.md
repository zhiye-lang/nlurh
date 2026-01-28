<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/dll/ui_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>

# UI Component Library ui.dll Usage Guide

Z-Language Interface Specification

------

### 1: Basic Interfaces(ArrayName"ui": using the out ui command can display all subset interfaces))

```jade
---<out ui
[ui].sub(smap gmap setdpi close cursor recovery backup get save pull fill push open )
```

------



##### 1.1: Get Image Data

| Interface | `ui.gmap`  | Retrieves image data                 |
| --------- | ---------- | ------------------------------------ |
| Parameter | `pathName` | File path                            |
| Returns   | `mapdata`  | Success: image data Failure: `false` |

```c++
buf ui.gmap(pathName) // Example: mapdata = ui.gmap("test.png")  
```

##### 1.2: Save Image Data

| Interface  | `ui.smap`            | Saves image data to file                  |
| ---------- | -------------------- | ----------------------------------------- |
| Parameters | `pathName` `mapdata` | File path Image data                      |
| Returns    | `int`                | Success: saved file size Failure: `false` |

```c++
int ui.smap(pathName, mapdata) // Example: smap("test2.png", mapdata)  
```

##### 1.3: Display Image Data

| Interface | `ui.dmap` | Displays image in a window       |
| --------- | --------- | -------------------------------- |
| Parameter | `mapdata` | Image data                       |
| Returns   | `bool`    | Success: `true` Failure: `false` |

```c++
bool ui.dmap(mapdata) // Example: ui.dmap(mapdata) OR ui.dmap(ui.gmap("test.png"))  
```

------

### 2: Advanced Interfaces(ArrayName"ui")

*^(Supports multi-window, scroll wheel, cursor customization)*

##### 2.1: Create Window

| Interface  | `ui.open`      | Creates display window                  |
| ---------- | -------------- | --------------------------------------- |
| Parameters | `w` `h` `name` | Window width Window height Window title |
| Returns    | `fd`           | Success: window handle Failure: `false` |

```c++
bool ui.open(w, h, name) // Example: fd = ui.open(640, 480, "test")  
```

##### 2.2: Close Window

| Interface | `closewd` | Closes window                    |
| --------- | --------- | -------------------------------- |
| Parameter | `fd`      | Window handle                    |
| Returns   | `bool`    | Success: `true` Failure: `false` |

```c#
bool ui.close(fd) // Example: ui.close(fd)  
```

##### 2.3: Fill Window

| Interface  | `ui.fill`            | Fills window with color                                      |
| ---------- | -------------------- | ------------------------------------------------------------ |
| Parameters | `fd` `colour` `flag` | Window handle Color (0xRRGGBB format) Blending mode: ¬- `flag=1`: Additive ¬- `flag=-1`: Subtractive ¬- `flag=2/±2`: Alpha blending |
| Returns    | `bool`               | Success: `true` Failure: `false`                             |

```c++
bool ui.fill(fd, colour, flag) // Examples: ui.fill(fd, 0xFF0000, 2) // Red window ui.fill(fd, 0x800000FF, 2) // Add 50% blue  
```

##### 2.4: Draw to Window

| Interface  | `ui.push`                  | Draws image to window                                     |
| ---------- | -------------------------- | --------------------------------------------------------- |
| Parameters | `fd` `mapdata` `left, top` | Window handle Image data Start coordinates (default: 0,0) |
| Returns    | `bool`                     | Success: `true` Failure: `false`                          |

```c++
bool ui.push(fd, mapdata, left, top) // Examples: ui.push(fd, mapdata) // Draw at (0,0) ui.push(fd, mapdata, 20, 10) // Draw at (20,10)  
```

##### 2.5: Backup Window

| Interface  | `ui.backup`    | Backs up window region                                       |
| ---------- | -------------- | ------------------------------------------------------------ |
| Parameters | `fd` `x,y,w,h` | Window handle Region coordinates (optional)                  |
| Returns    | `mapdata/bool` | With region: image data Without region: `true` Failure: `false` |

```c++
// Full window backup: ui.backup(fd) 
// Partial backup: md = ui.backup(fd, 20, 10, 80, 100)  
```

##### 2.6: Restore Window

| Interface  | `ui.recovery`  | Restores backup data                    |
| ---------- | -------------- | --------------------------------------- |
| Parameters | `fd` `x,y,w,h` | Window handle Restore region (optional) |
| Returns    | `bool`         | Success: `true` Failure: `false`        |

```c++
// Full restore: ui.recovery(fd)
// Partial restore: ui.recovery(fd, 20, 10, 80, 100)  
```

##### 2.7: Save Window

| Interface  | `ui.save`      | Saves window to image file  |
| ---------- | -------------- | --------------------------- |
| Parameters | `fd` `mapname` | Window handle Filename      |
| Returns    | `bool`         | Success: `1` Failure: other |

```c++
bool ui.save(fd, mapname) // Example: ui.save(fd, "mapwd.png")  
```

##### 2.8: Set Cursor Style

| Interface  | `ui.cursor`  | Sets cursor style                                            |
| ---------- | ------------ | ------------------------------------------------------------ |
| Parameters | `fd` `index` | Window handle Cursor ID: ¬- 32649: Hand pointer ¬- 32512: Arrow pointer |
| Returns    | `bool`       | Success: `true` Failure: `false`                             |

```c++
bool ui.cursor(fd, index) 
// Examples: ui.cursor(fd, 32649) 
// Hand cursor ui.cursor(fd, 32512) // Arrow cursor  
```

##### 2.9: Get Window Core API

| Interface | `ui.get`      | Gets low-level window API |
| --------- | ------------- | ------------------------- |
| Returns   | `UI_FuncBase` | Core API structure        |

```c++
UI_FuncBase ui.get() // For direct C/C++ development  
```

##### 2.10: Get Window Control Handle

| Interface | `ui.pull`        | Gets window control handle               |
| --------- | ---------------- | ---------------------------------------- |
| Parameter | `fd`             | Window handle                            |
| Returns   | `UI_FuncSynPlus` | Control structure (see definition below) |

```c++
UI_FuncSynPlus ui.pull(fd) // Usage in Zhiye: getuibase(ui.pull())  
```

------

### Data Structures & Implementation

```
// ===================== C Program Reference Definitions =====================  

// Bitmap structure  
typedef struct {  
    BYTE b;  
    BYTE g;  
    BYTE r;  
    BYTE a;  
} rgba_t;  

typedef union {  
    rgba_t t;  
    DWORD  rgba;  
} bitmap_trgb;  

typedef struct {  
    DWORD w;  
    DWORD h;  
    bitmap_trgb* data;  
} bitmap_t;  

// Window control structure  
typedef struct {  
    void* fd;  
    WindowBase* (*LockWindow)(void* fd);  
    bitmap_t ui;  
    void (*push)(WindowBase* pWd);  
    void (*loadKeyMouse)(WindowBase* pWd, FUNC_Keyboard pKeybd, ...);  
    void (*UnLockWindow)(WindowBase* pWd);  
} UI_FuncSynPlus;  

// Mouse/Keyboard callback definitions  
typedef int (*FUNC_MouseLdown)(bitmap_t*, int x, int y);     // Left click  
typedef int (*FUNC_MouseRdown)(bitmap_t*, int x, int y);     // Right click  
typedef int (*FUNC_MouseMove)(bitmap_t*, int type, int x, int y);  // Move  
typedef int (*FUNC_MouseWheel)(bitmap_t*, int updown);       // Wheel  
typedef int (*FUNC_Keyboard)(bitmap_t*, int down, int key, int timems); // Key events  
#define WM_WD_REFRESH 'P'  // Refresh required if bitmap modified  
//==================  pullwindow Return Structure Definition ========================
//  RGBQUAD
typedef struct
{
    void* fd;
    pWindowBase (*LockWindow)(HWND hWnd); // UI operation lock to avoid information synchronization issues. Returns NULL if the window has been released and can no longer be operated.
    bitmap_t ui;
    void (*push)(pWindowBase pWd);          // Push to display.
    void (*loadKeyMouse)(pWindowBase pWd, FUNC_Keyboard pKeybd, FUNC_MouseLdown pLdow, FUNC_MouseRdown pRdown, FUNC_MouseWheel pWheel, FUNC_MouseMove pMove); // Keyboard and mouse callback
    void (*UnLockWindow)(pWindowBase pWd);  // Unlock
} UI_FuncSyn;

// Add the drawing window operation parameter tSysBaseUi in the code.
UI_FuncSynPlus tSysBaseUi = { 0 };

//===== Download "Natural Language Understanding NLU - Third-party Function Access" https://gitee.com/kebo521/nlu3
void Keyboard(bitmap_t* pWd, int down, int key, int timems)
{
    if (down) { // A key is pressed
        //key, timems
    }
    else {
        if (key == 0) {
        // Window closed, bitmap_t ui has been released. Do not write data to ui.data afterwards.
        }
    }
}
//---- If mouse functions are required, define the following functions (select as needed) -----------------
void MouseLdown(bitmap_t* pWd, int x, int y) // Left click. x,y are the coordinates on the window.
{
    // Code to handle left mouse click.
}
void MouseRdown(bitmap_t* pWd, int x, int y) // Right click. x,y are the coordinates on the window.
{
    // Code to handle right mouse click.
}
void MouseMove(bitmap_t* pWd, int type, int x, int y) // type = 0: just moving; type=1: moving with left button pressed; type=2: moving with right button pressed.
{
    bitmap_t& wd = *pWd; // pWd will not be NULL. In C++ code, use reference for potential speed improvement.
    // Code to handle mouse movement.
}
void MouseWheel(bitmap_t* pWd, int updown)    // Positive: scroll down; Negative: scroll up.
{
    if (updown > 0) {
    }
    else {
    }
}
//---------- Y Language Reference Function Definition ---------------------------------
int FunPullUiBase(EnvP pENV, EXP_UNIT*& pInOutPar)    // (&UiBase)
{
    EXP_UNIT* pData = BAS_ParsingUnit(pInOutPar);
    if (pData == NULL || pData->type != TYPE_DATA || pData->tval != DATA_BUF) return -5;
    if (pData->pBfS->Len < sizeof(UI_FuncSyn)) {
        LoadBoolInUnit(pInOutPar, false);
        return 1;
    }
    UI_FuncSynPlus* pSyn = (UI_FuncSynPlus*)pData->pBfS->pBuf;
    tSysBaseUi.ui.fd = pSyn->fd;
    tSysBaseUi.LockWindow = pSyn->LockWindow;
    tSysBaseUi.UnLockWindow = pSyn->UnLockWindow;
    tSysBaseUi.ui.w = pSyn->ui.w;
    tSysBaseUi.ui.h = pSyn->ui.h;
    tSysBaseUi.ui.data = pSyn->ui.data;  // Modify display content via tSysBaseUi.ui.data later.
    tSysBaseUi.push = pSyn->push; // Execute: tSysBaseUi.push(pWd) to push display refresh later.
    //------ Keyboard or mouse functions require executing callback reverse loading ---------------
    WindowBase* pWd = tSysBaseUi.LockWindow(tSysBaseUi.fd);
    if (pWd) {
        (*pSyn->loadKeyMouse)(pWd, Keyboard, MouseLdown, MouseRdown, MouseMove);
        tSysBaseUi.UnLockWindow(pWd);
        LoadBoolInUnit(pInOutPar, true);
    }
    else {
        LoadBoolInUnit(pInOutPar, false);
    }
    //-----------------------------------------------------
    return 1;    // Only return one parameter.
}
/* Full window refresh example:
    WindowBase* pWd = tSysBaseUi.LockWindow(tSysBaseUi.fd); // Check and lock window.
    if (pWd == NULL) return -1; // Window no longer exists, return directly.
    register bitmap_trgb* pIn = tSysBaseUi.ui.data;
    register int max = tSysBaseUi.ui.w * tSysBaseUi.ui.h;
    while (max--) {
        pIn->t.r = 255;
        pIn->t.g = 255;
        pIn->t.b = 255;
        //pIn->rgba = 0xFFFFFF;
        pIn++;
    }
    tSysBaseUi.push(pWd); // Push refresh.
    tSysBaseUi.UnLockWindow(pWd); // Unlock window.
*/
//------------- Place FunPullUiBase into the gFuncTbl reference table -----------------
const DEF_FUNC_Tbl gFuncTbl =
{
    FUNC_TYPE_I,    1,
    1,
    {
        ......
        "getuibase",    FunPullUiBase,
        NULL,           NULL,    // End.
    }
};

//==================== Underlying Canvas Window Interface (Open) ================================================
typedef struct
{
    void* (*CreateWd)(int, int, const char*); // CreateWindow(WORD width, WORD height, const char* pName);
    int (*PullUi)(void*, UI_FuncSyn*); // (void* fd, UI_FuncSyn* pUiSyn)
    int (*CloseWd)(void*);    // CloseWindow(void* fd);
} UI_FuncBase;

loadwdapi(getwd()); // Third-party library implements loadwdapi to accept UI_FuncBase data. Same as: pullwd method.
```

**Full Implementation Guide**:
 [Natural Language Understanding NLU - Third-party Integration](https://gitee.com/kebo521/nlu3)

