<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/dll/uart_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>
------

# UART Component Library uart.dll Usage Guide

Z-Language Interface Specification



#### 1、Basic operation interface for serial communication (ArrayName "uart" : using the out uart command can display all subset interfaces)

```jade
---<out uart 
[uart].sub(close crecv recv send open scan getapi )
```

------

### 1.1: Open Serial Port

| Interface  | `uart.open`                  | Opens a serial port                                          |
| ---------- | ---------------------------- | ------------------------------------------------------------ |
| Parameters | `commName` `baudrate` `flag` | Port name (e.g., COM1) Baud rate Optional mode flag: - `"s"`: String mode - Default: Binary buffer mode |
| Returns    | Handle                       | Success: Port handle Failure: `false`                        |

```c++
hd uart.open(commName, baudrate, flag) // Example: fd = uart.open("com3", 9600, "s")  
```



### 1.2: Send Data

| Interface  | `uart.send` | Transmits data                              |
| ---------- | ----------- | ------------------------------------------- |
| Parameters | `hd` `data` | Port handle Data to send (string or buffer) |
| Returns    | `int`       | Success: Bytes transmitted Failure: `false` |

```c++
int uart.send(hd, data) // Example: uart.send(fd, "\x02testdata123456789\x03")  
```



### 1.3: Receive Data

| Interface | `uart.recv` | Receives data                           |
| --------- | ----------- | --------------------------------------- |
| Parameter | `hd`        | Port handle                             |
| Returns   | `buf`       | Success: Received data Failure: `false` |

```c++
buf uart.recv(hd) // Example: data = uart.recv(fd)  
```



### 1.4: Close Port

| Interface | `uart.close` | Closes port                      |
| --------- | ------------ | -------------------------------- |
| Parameter | `hd`         | Port handle                      |
| Returns   | `bool`       | Success: `true` Failure: `false` |

```c++
bool uart.close(hd) // Example: uart.close(fd)  
```



### 1.5: Threaded Receive

| Interface | `uart.crecv` | Enables background reception     |
| --------- | ------------ | -------------------------------- |
| Parameter | `hd`         | Port handle                      |
| Returns   | `bool`       | Success: `true` Failure: `false` |

```c++
bool uart.crecv(hd) // Starts background thread that prints received data to console  
```



### 1.6: Scan Ports

| Interface | `uart.scan` | Discovers available ports                     |
| --------- | ----------- | --------------------------------------------- |
| Parameter | `tag`       | Optional filter pattern                       |
| Returns   | `str{}`     | Success: Array of port names Failure: `false` |

```c++
strname uart.scan(tag) // Examples:<br>ports = uart.scan() // All ports<br>fd = uart.open(uscan("MI_????")) // Filtered ports  
```



### 1.7: Get C/C++ Interface

| Interface | `uart.getapi` | Exports native C/C++ API |
| --------- | ------------- | ------------------------ |
| Returns   | `bufApi`      | Pointer to API structure |

```
bufApi uart.getapi() // Example: uart.getapi()  
//----------- Structure Definition ----------------------  
typedef struct _HD_UART *PT_UART;  

typedef struct {  
    int (*scan)(char*, int, const char*);   // Scan ports  
    PT_UART (*open)(const char*, DWORD);     // Open port  
    int (*send)(PT_UART, u8*, int);          // Send data  
    int (*recv)(PT_UART, u8*, int, int);     // Receive data  
    void (*close)(PT_UART);                  // Close port  
} FT_UART_API;  

static FT_UART_API func_uart_api = {NULL};  

bool import_uart_api(void* pBuf, int size) {  
    if(size == sizeof(func_uart_api)) {  
        memcpy(&func_uart_api, pBuf, size);  
        return true;  
    }  
    return false;  
}  
```

**C Implementation Reference**:
 [Natural Language Understanding NLU - Third-party Integration](https://github.com/kebo521/HCI_NLU/dll_nlu3)