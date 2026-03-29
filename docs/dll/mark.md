<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/dll/mark_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>


# Mark Component Library mark.dll Usage Guide

> **Note:** mark requires a canvas to render graphics, so it depends on the UI library. To display text on the canvas, it also requires the freetype library. When running `unify` or `perimg`, it relies on `str.find` from `TextFlow.dll` to locate file paths.



### 1: Basic Interfaces (Array name `"mk"`: use the `out mk` command to display all subset interfaces)

```jade
-> out mk
[mk].sub(unify perimg setfont loadfont loadgui )
```

------



##### 1.1: Load UI Backend

| Interface | `mk.loadgui`   | Imports UI library functionality              |
| --------- | -------------- | --------------------------------------------- |
| Parameter | `pointer ui`   | Data obtained from `ui.get()`                 |
| Returns   | `bool`         | Success: `true`  Failure: `false`             |

```c
bool mk.loadgui(pt_ui) // mk.loadgui(ui.get());
```



##### 1.2: Load Font Backend

| Interface | `mk.loadfont`      | Imports freetype library functionality        |
| --------- | ------------------ | --------------------------------------------- |
| Parameter | `pointer freetype` | Data obtained from `ft.getapi()`              |
| Parameter | `string`           | Path to the font file                         |
| Returns   | `bool`             | Success: `true`  Failure: `false`             |

```c
bool mk.loadfont(pt_ft, path) 
// mk.loadfont(ft.getapi(),"C:\\Windows\\Fonts\\msyh.ttc");
// mk.loadfont(ft.getapi(),'"C:\Windows\Fonts\msyh.ttc"'); // same as above
```



##### 1.3: Set Font

| Interface | `mk.setfont` | Set character height and foreground color                                         |
| --------- | ------------ | --------------------------------------------------------------------------------- |
| Parameter | `int`        | Character height (width scales proportionally)                                   |
| Parameter | `int`        | Foreground color `0xAARRGGBB` (AA: `0x00` = opaque; `0xFF` = fully transparent). Background is always transparent. |
| Returns   | `bool`       | Success: `true`  Failure: `false`                                                 |

```c
bool mk.setfont(height, color) // mk.setfont(16, 0x00FF00)  // height 16, green opaque
```



##### 1.4: Label Images One by One

| Interface | `mk.perimg`   | Labels images one at a time (no scaling; opens a new window for each image)       |
| --------- | ------------- | --------------------------------------------------------------------------------- |
| Parameter | `mappath_sub` | Image path set (can be retrieved with `str.find` from TextFlow.dll)               |
| Parameter | `tagpath_sub` | Annotation file path set (same as above) or annotation file directory             |
| Returns   | `bool`        | Success: `true`  Failure: `false`                                                 |

```c
bool mk.perimg(path_sub, path_sub) 
/* mk.perimg(str.find("fireImg","*.png;*.webp;*.jpg"), find("fireTag","*.txt"))
   Note: str.find("fireImg","*.png;*.webp;*.jpg")  retrieves png/webp/jpg files under the fireImg directory
   Note: str.find("fireTag","*.txt")               retrieves all .txt files under the fireTag directory
*/    
```



##### 1.5: Process All Images in a Single Window

| Interface | `mk.unify`    | Processes all images in one window; each image is auto-scaled to fill the window. |
| --------- | ------------- | --------------------------------------------------------------------------------- |
| Parameter | `mappath_sub` | Image path set                                                                    |
| Parameter | `tagpath_sub` | Annotation file path set or annotation file directory                             |
| Parameter | `width`       | Window width                                                                      |
| Parameter | `height/null` | Window height; if omitted, height equals width                                    |
| Returns   | `bool`        | Success: `true`  Failure: `false`                                                 |

```c
mapdata mk.unify(path_sub, path_sub, width, height/null) 
/* mk.unify(str.find("fireImg","*.png;*.webp;*.jpg"), find("fireTag","*.txt"), 800)
   Note: str.find usage is the same as above. Window size: 800×800.
         The image filename and annotation filename must match.
*/  
```

```c
// Demo:
mk.loadgui(ui.get());
mk.loadfont(ft.getapi(), "C:\\Windows\\Fonts\\msyh.ttc");
mk.setfont(16, 0x00FF00);
mk.unify(str.find('"data\fire"', "*.png;*.webp;*.jpg"), '"data\tfire"', 800);
```
