# Camera Component Library cam.dll Usage Guide

Y Language interface reference

### Basic Interfaces (Group "cam": use `trace cam` to list all sub-interfaces)

```apl
-> trace cam
[cam].sub(ctrl caps show_close show info status close get stop start open list )
```

------

## 1. Basic Capture Interfaces

##### 1.1: Enumerate Camera List

| Interface | cam.list | Enumerate all available cameras              |
| --------- | -------- | -------------------------------------------- |
| Input     | —        | None                                         |
| Returns   | str      | Success: `N#name1\nname2...`; Failure or no camera: false |

```c
str cam.list()
/* demo
cam.list()
Returns: str[19]"1#USB2.0 HD UVC WebCam"

cam.list()
Returns: str[47]"2#Integrated Camera
USB2.0 HD UVC WebCam"
*/
```

##### 1.2: Open Camera

| Interface | cam.open       | Open a specific camera                                   |
| --------- | -------------- | -------------------------------------------------------- |
| Input     | index          | Camera index (0-based)                                   |
| Input     | w              | Requested width (default 640)                            |
| Input     | h              | Requested height (default 480)                           |
| Returns   | int            | Success: camera handle fd; Failure: false                |

```c
int cam.open(index, w/null, h/null)
/* demo
fd = cam.open(0)                     // Open first camera, default 640x480
fd = cam.open(1, 1280, 720)          // Open second camera, 1280x720
fd = cam.open(0, 320)                // Open first camera, 320x320 (h=null equals w)
*/
```

##### 1.3: Start Capture

| Interface | cam.start | Start camera frame capture            |
| --------- | --------- | ------------------------------------- |
| Input     | fd        | Camera handle (returned by cam.open)  |
| Returns   | bool      | Success: true; Failure: false          |

```c
bool cam.start(fd)
/* demo
ok = cam.start(fd)
if (!ok) trace("Camera start failed")
*/
```

##### 1.4: Get Current Frame

| Interface | cam.get | Get current capture frame (DATA_MAP format)                     |
| --------- | ------- | --------------------------------------------------------------- |
| Input     | fd      | Camera handle                                                   |
| Returns   | buf     | Success: DATA_MAP (RGBA image data); Failure: false             |

```c
buf cam.get(fd)
/* demo
fd = cam.open(0, 640, 480)
cam.start(fd)
sleep(0.5)                        // Wait for camera exposure to stabilize
frame = cam.get(fd)               // Get one frame of RGBA data
ui.smap("photo.png", frame)       // Save as image
ui.dmap(frame)                    // Popup display
*/
```

> **DATA_MAP Format**: Data returned by `cam.get()` contains a `DATA_MAP` header (width, height), followed by `w × h × 4` bytes of RGBA pixel data. This format can be passed directly to `ui.push()`, `ui.smap()`, `ui.dmap()` and other UI interfaces without conversion.

##### 1.5: Stop Capture

| Interface | cam.stop | Stop camera capture (keeps camera open)     |
| --------- | -------- | ------------------------------------------- |
| Input     | fd       | Camera handle                               |
| Returns   | bool     | Success: true; Failure: false               |

```c
bool cam.stop(fd)
/* demo
cam.stop(fd)    // Pause capture, camera stays open
cam.start(fd)   // Can resume capture
*/
```

> **stop vs close**: `stop` only stops capture — camera stays open and can be `start`ed again. `close` fully closes and releases resources.

##### 1.6: Close Camera

| Interface | cam.close | Close camera and release all resources      |
| --------- | --------- | ------------------------------------------- |
| Input     | fd        | Camera handle                               |
| Returns   | bool      | Success: true; Failure: false               |

```c
bool cam.close(fd)
/* demo
cam.stop(fd)      // Stop capture first (recommended)
cam.close(fd)     // Then close and release
*/
```

##### 1.7: Query Status

| Interface | cam.status | Query current camera status                 |
| --------- | ---------- | ------------------------------------------- |
| Input     | fd         | Camera handle                               |
| Returns   | int        | 0=closed, 1=opened, 2=capturing, 3=paused   |

```c
int cam.status(fd)
/* demo
st = cam.status(fd)
switch(st) {
    case 0 -> trace("Camera closed")
    case 1 -> trace("Camera opened")
    case 2 -> trace("Camera capturing")
    case 3 -> trace("Camera paused")
}
*/
```

##### 1.8: Get Camera Info

| Interface | cam.info | Get detailed camera information             |
| --------- | -------- | ------------------------------------------- |
| Input     | fd       | Camera handle                               |
| Returns   | str      | Success: description string; Failure: false |

```c
str cam.info(fd)
/* demo
trace(cam.info(fd))
// Output: [str][30]"Camera fd=1, 640x480, status=2"
*/
```

## 2. Advanced: Auto Display (Group "cam")

`cam.show` and `cam.show_close` provide **one-click camera open + window creation + live display**, replacing manual frame-loop boilerplate with a single line.

##### 2.1: Auto Display Camera

| Interface  | cam.show  | Auto open camera + create window + live frame display        |
| ---------- | --------- | ------------------------------------------------------------ |
| Input      | uibase    | UI low-level interface (obtained via `ui.get()`)             |
| Input      | fd        | Camera handle (pointer returned by `cam.open`)               |
| Input      | fps       | Refresh rate (default 30, range 1~120, optional)             |
| Returns    | int       | Success: display handle dfd; Failure: false                  |

```c
int cam.show(uibase, fd, fps/null)
/* demo
fb = ui.get()                          // Get UI low-level interface
fd = cam.open(0, 640, 480)             // Open camera first
cam.ctrl(fd, "background_blur", 1)     // Configure extension control before display
dfd = cam.show(fb, fd)                 // Pass in handle, default 30fps
dfd = cam.show(fb, fd, 60)             // 60fps

sleep(30)
cam.show_close(dfd)                    // Close display (camera stays open)
cam.close(fd)                          // Close camera separately
*/
```

> **Design**: `cam.show` no longer opens the camera internally — it receives the handle from `cam.open`. This allows driver-level extension control via `cam.ctrl` before display. Clear separation of concerns: `cam.open/close` manages camera lifecycle, `cam.show/show_close` manages window display.

##### 2.2: Close Auto Display

| Interface | cam.show_close | Close auto display (window + thread only, **camera stays open**) |
| --------- | -------------- | ---------------------------------------------------------------- |
| Input     | dfd            | Display handle (returned by cam.show)                            |
| Returns   | bool           | Success: true; Failure: false                                    |

```c
bool cam.show_close(dfd)
/* demo
fd = cam.open(0); dfd = cam.show(ui.get(), fd)
sleep(60)
cam.show_close(dfd)     // Close window, camera remains open
cam.stop(fd)            // Can continue operating the camera
cam.close(fd)           // Finally close the camera
*/
```

## 3. Extension Control: Driver-Level Portrait Processing (cam.caps / cam.ctrl)

Through the Windows 10+ `IMFExtendedCameraController` interface, directly control **camera driver built-in** portrait processing features such as background blur, face detection, eye correction, etc. No additional image processing libraries needed — done entirely at the driver level.

> **Prerequisite**: Camera must be opened (`cam.open` — `cam.start` is not required).
> **Support**: Depends on the camera driver; query with `cam.caps(fd)`.

##### 3.1: Query Supported Features

| Interface | cam.caps | Query driver-supported extension features                      |
| --------- | -------- | -------------------------------------------------------------- |
| Input     | fd       | Camera handle                                                  |
| Returns   | str      | Comma-separated feature names; empty string "" if none; false on failure |

```c
str cam.caps(fd)
/* demo
fd = cam.open(0)
trace(cam.caps(fd))
// Possible output: "background_blur, eye_correction, video_hdr"
*/
```

##### 3.2: Read/Set Extension Control

| Interface | cam.ctrl | Read or set driver-level extension feature                     |
| --------- | -------- | -------------------------------------------------------------- |
| Input     | fd       | Camera handle                                                  |
| Input     | prop     | Property name (string)                                         |
| Input     | value/null | Set value (present=write mode, absent=read mode)            |
| Returns   | int/bool | Read mode: current value (-1=not supported); Write mode: bool  |

```c
// Read mode: query current state
int/bool cam.ctrl(fd, prop)

// Write mode: toggle feature on/off
bool cam.ctrl(fd, prop, value)
```

##### 3.3: Supported Extension Properties

| Property | R/W | Value | Description |
|----------|-----|-------|-------------|
| `"background_blur"` | R/W | 0=off, 1=blur, 2=replace | Portrait background blur/replace (most common) |
| `"face_detection"` | R/W | 0=off, 1=on | Driver-level face detection (deprecated in Win11, read-only) |
| `"eye_correction"` | R/W | 0=off, 1=on | Eye correction (appears to look at camera during video calls) |
| `"video_hdr"` | R/W | 0=off, 1=on | Video HDR (improves backlit/high-contrast scenes) |
| `"video_stabilization"` | R/W | 0=off, 1=on | Video stabilization |
| `"roi_isp"` | R/W | 0=off, 1=on | Face ROI exposure optimization (ISP meters based on face) |

```c
/* demo: Complete usage flow */
fd = cam.open(0)

// Query supported features
caps = cam.caps(fd)
trace("Supported: " + caps)
// → Supported: background_blur, eye_correction, video_hdr

// Enable background blur
cam.ctrl(fd, "background_blur", 1)

// Enable eye correction
cam.ctrl(fd, "eye_correction", 1)

// Query current state
blur = cam.ctrl(fd, "background_blur")     // → 1
eye  = cam.ctrl(fd, "eye_correction")      // → 1

// Start capture (driver automatically applies above settings)
cam.start(fd)
frame = cam.get(fd)    // Frame data already processed by driver
```

##### 3.4: Driver Portrait Blur Example

```c
logset(0)
loadlib"dll/ui.dll"
loadlib"dll/cam.dll"

fd = cam.open(0, 640, 480)

// Query support
caps = cam.caps(fd)
trace("Camera extension features: " + caps)

// Enable background blur
if (cam.ctrl(fd, "background_blur", 1)) {
    trace("Background blur enabled")
} else {
    trace("Camera does not support background blur")
}

// Also enable eye correction (if supported)
cam.ctrl(fd, "eye_correction", 1)

// Capture and display
cam.start(fd)
dfd = cam.show(ui.get(), fd)     // Pass in configured handle
sleep(30)
cam.show_close(dfd)              // Close window only
cam.close(fd)                    // Close camera
```

## 4. Complete Examples

##### 4.1: Basic Capture → Save Photo

```c
logset(0)
loadlib"dll/ui.dll"
loadlib"dll/cam.dll"

// Enumerate cameras
list = cam.list()
trace("Camera list: " + list)

// Open and capture one frame
fd = cam.open(0, 640, 480)
if (!fd) { trace("Failed to open camera"); return }
cam.start(fd)
sleep(0.5)                          // Wait for exposure stabilization

frame = cam.get(fd)
if (frame) {
    ui.smap("photo.png", frame)     // Save photo
    trace("Photo saved")
}

cam.stop(fd)
cam.close(fd)
```

##### 4.2: Auto Display (Configure First, Then Display)

```c
logset(0)
loadlib"dll/ui.dll"
loadlib"dll/cam.dll"

fb = ui.get()
fd = cam.open(0, 640, 480)         // Open camera first
cam.ctrl(fd, "background_blur", 1)  // Enable background blur

dfd = cam.show(fb, fd)              // Pass in handle, create live display window
sleep(60)                           // Display for 60 seconds
cam.show_close(dfd)                 // Close window, camera stays open
cam.close(fd)                       // Close camera
```

##### 4.3: Manual Frame Control (Real-time Processing)

```c
logset(0)
loadlib"dll/ui.dll"
loadlib"dll/cam.dll"

// Create window
fb = ui.get()
fd = ui.open(640, 480, "Live Feed")

// Open camera
cfd = cam.open(0, 640, 480)
cam.start(cfd)

while (100) {                       // Display 100 frames
    frame = cam.get(cfd)
    if (frame) {
        // Process frame here (face detection, image filtering, etc.)
        ui.push(fd, frame, 0, 0)
    }
    sleep(0.033)                    // ~30fps
}

cam.stop(cfd)
cam.close(cfd)
ui.close(fd)
```

##### 4.4: Multi-Camera Simultaneous Capture

```c
logset(0)
loadlib"dll/ui.dll"
loadlib"dll/cam.dll"

list = cam.list()
trace("Cameras found: " + list)

fd0 = cam.open(0, 640, 480)
cam.start(fd0)

fd1 = cam.open(1, 640, 480)
cam.start(fd1)

// Create two windows
fb = ui.get()
wd0 = ui.open(640, 480, "Camera 1")
wd1 = ui.open(640, 480, "Camera 2")

while (100) {
    f0 = cam.get(fd0)
    if (f0) ui.push(wd0, f0, 0, 0)

    f1 = cam.get(fd1)
    if (f1) ui.push(wd1, f1, 0, 0)

    sleep(0.033)
}

cam.close(fd0)
cam.close(fd1)
ui.close(wd0)
ui.close(wd1)
```

## 5. State Machine

```
                    cam.open()
    [CLOSED] ──────────────────→ [OPENED]
       ↑                           │
       │                    cam.start()
       │                           ↓
       │                      [RUNNING] ──── cam.stop() ──→ [PAUSED]
       │                           │                          │
       │                           │                  cam.start()
       │                           │                          ↓
       │                           │                      [RUNNING]
       │                    cam.stop() / cam.close()
       │                           │
       └──── cam.close() ──────────┘
```

| State Value | State Name | Description |
|-------------|------------|-------------|
| 0 | CAM_CLOSED | Closed, camera not opened |
| 1 | CAM_OPENED | Opened but not capturing |
| 2 | CAM_RUNNING | Currently capturing |
| 3 | CAM_PAUSED | Capture paused |

> Camera handle fd shows CAM_CLOSED when `cam.status()` returns 0 (including non-existent fds).

## 6. C/C++ Program Reference

```c
//==================== DATA_MAP Structure Definition (from ext_comm.h) ==============
typedef struct {
    uint16_t w, h;       // Image width and height
    uint32_t rgbA[];     // RGB pixel data, one uint32 per pixel (RGBA packed)
} DATA_MAP;

// Usage example:
DATA_MAP* pMap = (DATA_MAP*)pBfS->pBuf;
int width  = pMap->w;
int height = pMap->h;
uint32_t* pixels = pMap->rgbA;  // Each uint32 = one pixel (RGBA)
// Pixel components: (pixel >> 16) & 0xFF = R, (pixel >> 8) & 0xFF = G, pixel & 0xFF = B, (pixel >> 24) & 0xFF = A
```

> **UI Integration**: The DATA_MAP format returned by `cam.get()` is fully compatible with `ui.push` / `ui.smap` / `ui.dmap` — a rare case of **cross-module direct interoperability** at the Y Language level.

---

After compiling the above functions, load with `loadlib` in Zhiye: first enumerate with `cam.list()`, then `cam.open` to open, `cam.start` to begin capture, `cam.get` to retrieve frames, and finally `cam.stop` → `cam.close`, or use `cam.show` for a one-click solution.
