<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/dll/ui_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>
# UI Component Library ui.dll Usage Guide

Y-Language Interface Specification

### 1: Basic Interfaces (Group "ui": use `out ui` to list all sub-interfaces)

```jade
-> out ui
[ui].sub(unlockwd wdfill wdline wdpoint lockwd resizemap smap gmap setdpi close cursor recovery backup get save pull fill push open )
```

------

##### 1.1: Get Image Data

| Interface | `ui.gmap`  | Retrieves image data                                              |
| --------- | ---------- | ---------------------------------------------------------------- |
| Parameter | `pathName` | File path (supports: bmp, png, jpg, tga, webp)                   |
| Returns   | `mapdata`  | Success: image data; Failure: `false`                            |

```c
buf ui.gmap(pathName) // mapdata = ui.gmap("test.png")
```

##### 1.2: Save Image Data

| Interface   | `ui.smap`            | Saves image data to file                                 |
| ----------- | -------------------- | --------------------------------------------------------- |
| Parameters  | `pathName` `mapdata` | File path Image data                                      |
| Returns     | `int`                | Success: file size in bytes; Failure: `false`             |

```c
int ui.smap(pathName, mapdata) // smap("test2.png", mapdata)
```

##### 1.3: Display Image Data

| Interface | `ui.dmap` | Creates a window on screen to display image data    |
| --------- | --------- | -------------------------------------------------- |
| Parameter | `mapdata` | Image data                                         |
| Returns   | `bool`    | Success: `true`; Failure: `false`                  |

```c
bool ui.dmap(mapdata) // dmap(mapdata)  or  dmap(gmap("test.png"))
```

##### 1.4: Resize Image Data

| Interface   | `ui.resizemap` | Resizes image data                                              |
| ----------- | -------------- | -------------------------------------------------------------- |
| Parameters  | `mapdata`      | Source image data                                              |
| Parameter   | `int`          | Target width                                                   |
| Parameter   | `int/null`     | Target height (when absent, equals width to make a square)     |
| Returns     | `mapdata`      | Success: new image data; Failure: `false`                      |

```c
mapdata ui.resizemap(mapdata, width, height/null)
// m = ui.resizemap(gmap("test.png"), 800, 600)  → resize to 800x600
// m = ui.resizemap(gmap("test.png"), 800)       → resize to 800x800 (square)
```

------

### 2: Advanced Interfaces (Group "ui")

*^(Adds multi-window support, scroll wheel, cursor customization)*

##### 2.1: Create Window

| Interface   | `ui.open`      | Creates and displays a window                       |
| ----------- | -------------- | --------------------------------------------------- |
| Parameters  | `w` `h` `name` | Window width, Window height, Window title          |
| Returns     | `fd`           | Success: window handle; Failure: `false`            |

```c
bool ui.open(w, h, name) // fd = ui.open(640, 480, "test")
```

##### 2.2: Close Window

| Interface | `ui.close` | Closes a window opened by `ui.open`             |
| --------- | ---------- | ----------------------------------------------- |
| Parameter | `fd`       | Window handle                                   |
| Returns   | `bool`     | Success: `true`; Failure: `false`               |

```c
bool ui.close(fd) // ui.close(fd)
```

##### 2.3: Fill Window

| Interface   | `ui.fill`            | Fills the window with a color                                                   |
| ----------- | -------------------- | -------------------------------------------------------------------------------- |
| Parameters  | `fd` `colour` `flag` | Window handle; Color in 0xRRGGBB hex (or 0xaaRRGGBB where aa=alpha, 00=opaque); Blending flag |
| Returns     | `bool`               | Success: `true`; Failure: `false`                                               |

> Blending flag (only effective when aa>0):
> - absent: fill only, no display
> - `1`: additive blend, + (256 - aa) / 256
> - `-1`: subtractive blend
> - `2` / `-2`: as above plus display push

```c
bool ui.fill(fd, colour, flag)
// Examples:
// ui.fill(fd, 0xFF0000, 2)        → full red window
// ui.fill(fd, 0x800000FF, 2)      → add 50% blue to window
```

##### 2.4: Draw to Window

| Interface   | `ui.push`                  | Draws image data to the window and displays it      |
| ----------- | -------------------------- | -------------------------------------------------- |
| Parameters  | `fd` `mapdata` `left, top` | Window handle; Image data; Start coordinates (default 0,0 when absent) |
| Returns     | `bool`                     | Success: `true`; Failure: `false`                  |

```c
bool ui.push(fd, mapdata, left, top)
// Examples:
// ui.push(fd, mapdata)         → draw at (0,0)
// ui.push(fd, mapdata, 20, 10) → draw at (20,10)
```

##### 2.5: Backup Window

| Interface   | `ui.backup`    | Backs up the current window image data                                      |
| ----------- | -------------- | ---------------------------------------------------------------------------- |
| Parameters  | `fd` `x,y,w,h` | Window handle; Region (x,y,width,height) — omit to backup the whole window to internal buffer |
| Returns     | `mapdata/bool` | With region params: image data; Without: `true`; Failure: `false`           |

```go
bool ui.backup(fd)  \  mapdata ui.backup(fd, x, y, w, h)
// ui.backup(fd)                       → backup whole window to internal buffer
// md = ui.backup(fd, 20, 10, 80, 100)  → get mapdata of 80x100 region at (20,10)
// md can be passed to ui.push() or ui.smap() to save as image
```

##### 2.6: Restore Window

| Interface   | `ui.recovery`  | Restores previously backed-up data to the window and displays it   |
| ----------- | -------------- | ------------------------------------------------------------------ |
| Parameters  | `fd` `x,y,w,h` | Window handle; Restore region — omit to restore the whole window  |
| Returns     | `bool`         | Success: `true`; Failure: `false`                                  |

```c#
bool ui.recovery(fd)  \  bool ui.recovery(fd, x, y, w, h)
// (call ui.backup(fd) first before ui.recovery)
// ui.recovery(fd)                       → restore whole window
// ui.recovery(fd, 20, 10, 80, 100)      → restore 80x100 region at (20,10)
```

##### 2.7: Save Window

| Interface   | `ui.save`      | Saves current window data to an image file |
| ----------- | -------------- | ------------------------------------------ |
| Parameters  | `fd` `mapname` | Window handle; Image filename              |
| Returns     | `bool`         | Success: `1`; Failure: other               |

```c
bool ui.save(fd, mapname)
// ui.save(fd, "mapwd.png")  → save fd window data to mapwd.png
```

##### 2.8: Set Cursor Style

| Interface   | `ui.cursor`  | Sets cursor style                                            |
| ----------- | ------------ | ------------------------------------------------------------ |
| Parameters  | `fd` `index` | Window handle; Cursor ID (32649=hand pointer, 32512=arrow)  |
| Returns     | `bool`       | Success: `true`; Failure: `false`                           |

```c
bool ui.cursor(fd, index)
// ui.cursor(fd, 32649)  → hand pointer
// ui.cursor(fd, 32512)  → arrow pointer
```

##### 2.9: Get Window Core API

| Interface | `ui.get`      | Gets the low-level window API (creates window directly in C without going through Y Language, unlike `ui.open`) |
| --------- | ------------- | ------------------------------------------------------------------------------------------------- |
| Returns   | `UI_FuncBase` | Internal API structure                                                                            |

```
UI_FuncBase ui.get()
// For direct C/C++ development; see "C Program Reference Definitions 2" for UI_FuncBase definition
```

##### 2.10: Get Window Control Handle

| Interface | `ui.pull`        | Gets the window control handle so other libraries can control the window directly via C |
| --------- | ---------------- | --------------------------------------------------------------------------------------- |
| Parameter | `fd`             | Window handle                                                                          |
| Returns   | `UI_FuncSynPlus` | Success: control structure (see "C Program Reference Definitions 2"); Failure: `false` |

```c
UI_FuncSynPlus ui.pull(fd)
// Example: in Zhiye chat window, input: getuibase(pullwd());
```

```c
//===================== C Program Reference Definitions 2 =============================
// First download the "Third-party Function Access" code, add the following content, 
// compile the dll, then load with loadlib in Zhiye.
typedef struct _WindowBase WindowBase;
//================ Keyboard & Mouse Callback Function Definitions ========================
typedef int (*FUNC_MouseLdown)(bitmap_t*, int x, int y);       // Left click at x,y in window
typedef int (*FUNC_MouseRdown)(bitmap_t*, int x, int y);       // Right click at x,y in window
typedef int (*FUNC_MouseMove)(bitmap_t*, int type, int x, int y); // type=0 just move, type=1 move with left button, type=2 with right
typedef int (*FUNC_MouseWheel)(bitmap_t*, int updown);         // positive=scroll down, negative=scroll up
typedef int (*FUNC_Keyboard)(bitmap_t*, int down, int key, int timems); // down=true means pressed
#define WM_WD_REFRESH  'P' // Return this from callbacks when canvas content was modified and needs refresh.
//======================== Graphics Data Structure Definitions ==========================
typedef struct {
    BYTE b;
    BYTE g;
    BYTE r;
    BYTE a;
} rgba_t;

typedef union {
    rgba_t  t;
    DWORD   rgba;
} bitmap_trgb;

typedef struct {
    DWORD w;
    DWORD h;
    bitmap_trgb* data;
} bitmap_t;

//==================  pullwindow Return Structure Definition =========================
typedef struct
{
    void* fd;
    pWindowBase (*LockWindow)(HWND hWnd); // UI operation lock to avoid sync issues. Returns NULL if the window has been released.
    bitmap_t ui;
    void (*push)(pWindowBase pWd);          // Push to display.
    void (*loadKeyMouse)(pWindowBase pWd, FUNC_Keyboard pKeybd, FUNC_MouseLdown pLdow, FUNC_MouseRdown pRdown, FUNC_MouseWheel pWheel, FUNC_MouseMove pMove); // Keyboard and mouse callback
    void (*UnLockWindow)(pWindowBase pWd);  // Unlock
} UI_FuncSyn;

// In your code, add the window operation parameter tSysBaseUi.
UI_FuncSynPlus tSysBaseUi = { 0 };

//===== Download "NLU - Third-party Function Access" https://gitee.com/kebo521/nlu3
void Keyboard(bitmap_t* pWd, int down, int key, int timems)
{
    if (down) { // A key is pressed
        // key, timems
    }
    else {
        if (key == 0) {
            // Window closed; bitmap_t ui has been released. Do not write to ui.data afterwards.
        }
    }
}
//---- If mouse functions are required, define the following (select as needed) -----------------
void MouseLdown(bitmap_t *pWd, int x, int y) // Left click at x,y in window
{
    // Handle left click.
}
void MouseRdown(bitmap_t *pWd, int x, int y) // Right click at x,y in window
{
    // Handle right click.
}
void MouseMove(bitmap_t *pWd, int type, int x, int y) // type=0: just moving; type=1: with left; type=2: with right
{
    bitmap_t& wd = *pWd; // pWd is never NULL; use reference in C++ for speed
    // Handle mouse movement.
}
void MouseWheel(bitmap_t *pWd, int updown)    // positive=down, negative=up
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
    tSysBaseUi.push = pSyn->push;        // Call tSysBaseUi.push(pWd) to push display refresh.
    //------ Keyboard or mouse functions require callback reverse-loading ---------------
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
    tSysBaseUi.push(pWd);           // Push refresh.
    tSysBaseUi.UnLockWindow(pWd);   // Unlock window.
*/
//------------- Place FunPullUiBase into the gFuncTbl reference table -----------------
const DEF_FUNC_Tbl gFuncTbl =
{
    FUNC_TYPE_I,    1,
    1,
    {
        ......
        "getuibase",    FunPullUiBase,
        NULL,           NULL,    // End
    }
};

//==================== Underlying Canvas Window Interface (Open) ================================================
typedef struct
{
    void* (*CreateWd)(int, int, const char*); // CreateWindow(WORD width, WORD height, const char* pName);
    int (*PullUi)(void*, UI_FuncSyn*);        // (void* fd, UI_FuncSyn* pUiSyn)
    int (*CloseWd)(void*);                    // CloseWindow(void* fd);
} UI_FuncBase;

loadwdapi(getwd()); // Third-party library implements loadwdapi to accept UI_FuncBase data. Same as: pullwd method.

```

After compiling, load with `loadlib` in Zhiye: first create a window `fd = ui.open(640, 480, "test")`, then call `getuibase(ui.pull(fd))` to obtain the UI control parameters.

------

### 3: Critical-Section Drawing Interfaces (Group "ui")

##### 3.1: Lock Window

| Interface | `ui.lockwd` | Locks the window and maps the canvas         |
| --------- | ----------- | -------------------------------------------- |
| Parameter | `fd`        | Window handle                                |
| Returns   | `hd`        | Success: lock handle; Failure: `false`       |

```c
hd ui.lockwd(fd) // hd = ui.lockwd(fd)
```

##### 3.2: Draw Point

| Interface | `ui.wdpoint` | Draw a point on the canvas (buffered, not yet displayed)        |
| --------- | ------------ | ---------------------------------------------------------------- |
| Parameter | `hd`         | Lock handle                                                      |
| Parameter | `x`          | X coordinate (top-left = 0, left → right)                        |
| Parameter | `y`          | Y coordinate (top → bottom)                                      |
| Parameter | `color`      | Color in 0xAARRGGBB (32-bit), AA = alpha                        |
| Returns   | `null`       | No return value                                                  |

```c
Format 1: ui.wdpoint(hd, x, y, color)
// ui.wdpoint(hd, 20, 30, 0xFF0000)  → red point at (20,30)

Format 2: ui.wdpoint(hd, (x, y, color))
// For Y interfaces that return (x, y, color) in one call, e.g. ui.wdpoint(hd, funXX(sin))
```

##### 3.3: Draw Line

| Interface | `ui.wdline` | Draw a line on the canvas (buffered, not yet displayed)                  |
| --------- | ----------- | ----------------------------------------------------------------------- |
| Parameter | `hd`        | Lock handle                                                             |
| Parameter | `x1~x2`     | X range (from x1 to x2)                                                 |
| Parameter | `y1~y2`     | Y range (from y1 to y2) — draws a line from (x1,y1) to (x2,y2)          |
| Parameter | `color`     | Color in 0xAARRGGBB (32-bit), AA = alpha                                |
| Returns   | `null`      | No return value                                                         |

```c
Format 1: ui.wdline(hd, x1~x2, y1~y2, color)
// ui.wdline(hd, 10~20, 30~60, 0x00FF00)  → green line from (10,30) to (20,60)

Format 2: ui.wdline(hd, (x1~x2, y1~y2, color))
// For Y interfaces that return the params as one tuple, e.g. ui.wdline(hd, funXY(dd))
```

##### 3.4: Fill Region

| Interface | `ui.wdfill`           | Fill a region on the canvas (buffered, not yet displayed)             |
| --------- | --------------------- | --------------------------------------------------------------------- |
| Parameter | `hd`                  | Lock handle                                                           |
| Parameter | `color`               | Color in 0xAARRGGBB, AA = alpha                                       |
| Parameter | `x1~x2/null`          | X range to fill (omit = full width)                                   |
| Parameter | `y1~y2/null`          | Y range to fill (omit = full height)                                  |
| Returns   | `null`                | No return value                                                       |

```c
Format 1: ui.wdfill(hd, color, x1~x2/null, y1~y2/null)
// ui.wdfill(hd, 0xFFFFFF)              → fill whole screen white
// ui.wdfill(hd, 0x0000FF, 20~50)        → blue from x=20 to x=50
// ui.wdfill(hd, 0xFFFF00, 20~50, 70~90) → blue rectangle (20..50, 70..90)

Format 2: ui.wdfill(hd, (color, x1~x2/null, y1~y2/null))
// For Y interfaces that return the params as one tuple, e.g. ui.wdfill(hd, funWH())
```

##### 3.5: Unlock Window (Refresh)

| Interface | `ui.unlockwd` | Pushes the buffered content to the window for display and unlocks |
| --------- | ------------- | ----------------------------------------------------------------- |
| Parameter | `hd`          | Lock handle                                                       |
| Returns   | `null`        | No return value                                                   |

```c
ui.unlockwd(hd)
// Usage:
// hd = ui.lockwd(fd)
// ...draw calls...
// ui.unlockwd(hd)   ← pushes the buffer to the visible window
```

------

**Base Code for C Program Reference Definitions**: [Natural Language Understanding NLU - Third-party Function Access](https://gitee.com/kebo521/nlu_mt/tree/master/dll_nlu3)
