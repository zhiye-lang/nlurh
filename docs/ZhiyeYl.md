<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/%E6%94%AF%E5%8F%B6-Y%E8%AF%AD%E8%A8%80.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>
----
# Zhiye-Y Language (Interaction Syntax) Basics

## I. Y Language Fundamentals

### 1. Numeric Types

Supports floating-point and integer values (decimal, hexadecimal, octal).

| Type           | Format        | Examples                                                |
| -------------- | ------------- | ------------------------------------------------------- |
| Float (`num`)  | With decimal  | `78.64`, `-123.96`, `100.0`                             |
| Integer (`int`) | Plain number | `123`, `-464`, `0xFA12` (hex), `0766` (octal)           |

### 2. String Type

**Double-quoted string `"..."`** — fully compatible with C:

```c
"abc\"123\""           // String: abc"123"
"\x31\x42\x53\r\n"     // Hex escapes + CRLF
"Hello\nWorld\t!"      // Standard escape characters
```

**Supported escape characters:** `\n`, `\t`, `\r`, `\"`, `\\`, `\xHH`

### 3. Byte Buffer Type (Y Language Original)

**Single-quoted byte buffer `'...'`** — stores binary data; supports mixed input formats.

| Format              | Example                      | Description                            |
| ------------------- | ---------------------------- | -------------------------------------- |
| Continuous hex      | `'112233'`                   | 3 bytes: 0x11, 0x22, 0x33              |
| Space-separated hex | `'11 22 33'`                 | Same, space-separated for readability  |
| Escape format       | `'\x11\x22\x33'`             | C-style hex escapes                    |
| 16-bit Unicode      | `'\u3132\u42AA'`             | Little-endian: 32 31 AA 42             |
| 32-bit Unicode      | `'\U11223344'`               | Little-endian: 44 33 22 11             |
| Mixed format        | `'0x11,0x2233,0x44556677'`   | Width auto-detected                    |

**Embedded string rules:**

```c
'"{"run":"max","data":7}"'     // Standalone embed: starts with '", ends with "', treated as a raw string — no escaping
'123242"abc"'                 // Non-standalone embed: string portion is converted to 8-bit hex
```

### 4. Range Type

Represents numeric intervals:

```y
a = 10~100          // Range: 10 to 100; 64-bit platform supports step, e.g. 10~100:2 (10 to 100, step 2)
```

## II. Variables and Parameters

### 1. Variable Definition

No type declaration required; types are inferred at assignment (similar to Python):

```y
a = 123              // Integer
b = "hello"          // String
c = '112233'         // Byte buffer
d = 10~100           // Range
```

### 2. Scope

- Supports global and local parameters
- Variables defined inside a function are local
- Variables defined at the top level are global

### 3. Parameter Subscript Notation `[..]`

`[..]` is used to index or slice a parameter — the core subscript syntax in Y Language's parameter system:

**Index access `buf[n]`**: Retrieves the `(n+1)`-th sub-element (0-based), consistent with mainstream languages:

```c
subdata[3]       // The 4th sub-parameter of subdata
buf[0]           // The 1st element of buf
```

**Range access `buf[s~e]`**: Retrieves all content between positions `s` and `e` (inclusive, 0-based) — a Y Language extension:

```c
buf[0~4]         // Elements 1 through 5 of buf (inclusive)
```

> **Note:** `[..]` is a modifier attached directly after the parameter name, not a standalone operator. It acts on the parameter's content to extract a specified subset of data.

## III. Expressions and Operators

### 1. Numeric Operators

| Category        | Operators                        |
| --------------- | -------------------------------- |
| Arithmetic      | `+` `-` `*` `/` `%` `.*` `./`   |
| Compound assign | `+=` `-=` `*=` `/=` `%=`         |
| Bitwise         | `&` `|` `^`                      |
| Logical         | `&&` `||`                        |
| Comparison      | `>` `>=` `<` `<=` `==`           |
| Assignment      | `=`                              |

### 2. String Operators

| Operation              | Description       | Example                  |
| ---------------------- | ----------------- | ------------------------ |
| `+`                    | Concatenation     | `"Hello" + " World"`     |
| `==`                   | Equality          | `"abc" == "abc"`         |
| `>`, `<`, `>=`, `<=`   | Lexicographic     | `"abb" > "aab"`          |
| `=`                    | Assignment        | `str = "new"`            |

### 3. Cross-Type Operator Rules

The `+` operator in Y Language supports operations across different data types. **The result type follows the left operand:**

| Left Operand | Right Operand | Op  | Result Type | Description                                  |
| ------------ | ------------- | --- | ----------- | -------------------------------------------- |
| str          | str           | `+` | str         | String concatenation                         |
| str          | buf           | `+` | str         | buf converted to string, then concatenated   |
| buf          | str           | `+` | buf         | String converted to bytes, then appended     |
| buf          | buf           | `+` | buf         | Byte buffer concatenation                    |

**UTF-8 Encoding Trick:** The type-coercion behavior of cross-type `+` enables convenient encoding inspection:

```c
'' + "国"       // Returns buf — shows UTF-8 encoding of "国": buf[3]' E5 9B BD'
"" + 'E5 9B BD' // Returns str — decodes UTF-8 bytes back to character: str[1]"国"
```

> **Core rule:** The result type of `+` always follows the left operand — `str + buf` → str, `buf + str` → buf.

### 4. Numeric Type Promotion Rules

For `+`, `-`, `*`, `/` on numeric types, the result type follows these rules (only all-integer operations produce an integer result):

| Left | Right | Result | Example        | Value      |
| ---- | ----- | ------ | -------------- | ---------- |
| int  | int   | int    | `10 / 3`       | `3`        |
| int  | num   | num    | `10 / 3.0`     | `3.333333` |
| num  | int   | num    | `10.0 / 3`     | `3.333333` |
| num  | num   | num    | `10.0 / 3.0`   | `3.333333` |

> **Note:** Only `int / int` truncates the decimal; all other combinations preserve it.

### 5. Composite Data Operations (Arrays/Matrices)

Y Language has built-in vector and matrix types for efficient numerical computation.

- **Vector:** Space-separated values, e.g. `[1 2 3]`
- **Matrix:** Rows separated by `;` or `,`, e.g. `[1 2; 3 4]` or `[1 2, 3 4]`

They support the full set of arithmetic operators (`+`, `-`, `*`, `/`, `.*`, `./`), where `*` is matrix multiplication and `.*` is element-wise multiplication.

```bat
[11 22 33]*5                ->  [int:55 int:110 int:165 ]
[11 22 33]+[44 55 66]       ->  [int:55 int:77 int:99 ]
[1.1 2.2 3.3]*[4.4 5.5 6.6] -> [num:4.84 num:12.1 num:21.78 ]
[1 2]/[3 4]                 ->  [int:0 int:0 ]
[1.0 2.0]/[3 4]             ->  [num:0.333333 num:0.5 ]
// Matrix multiplication (max 255x255)
a=[1 2; 3 4]; b=[5 6; 7 8];
c=a*b;
out c  ->  [c]sub(int:19 int:22  ,  int:43 int:50 )
```

### 6. Pipe Operator `|>`

`|>` passes the value on the left as an argument to the function on the right, supporting chained calls — data flow direction matches reading direction:

```julia
// Nested style: read from inside out
localtime(time("ntp2.aliyun.com"))

// Pipe style: read left to right — clearer logic
"ntp2.aliyun.com" |> time |> localtime
```

> **Note:** When the right-hand function takes exactly one argument, parentheses may be omitted (consistent with the function call shorthand rule). For multi-argument functions, supply extra parameters explicitly, e.g. `val |> process(extra_param)`.

## IV. Control Flow

Syntax is similar to C / Java / JavaScript:

### 1. Conditional

```c
if(condition) {
    // executes when true
} else {
    // executes when false
}
```

### 2. Loops

```c
// Standard while loop (same as C/C++, Java, JavaScript, C#)
while(condition) {
    // body (supports: break, continue, return)
}

// Unconditional loop (Y Language original)
while() {
    a++;  // Infinite loop (supports: break, continue, return)
}

// Fixed-count loop (Y Language original)
while(100) {
    // Executes exactly 100 times (supports: break, continue, return)
}
// Note: In Y Language, while(1) executes the body once. Use while() for an infinite loop.

// for loop (same as C/C++, Java, JavaScript, C#)
for (init; condition; increment) {
    // body (supports: break, continue, return)
}
```

### 3. Switch / Branch

Two switch styles are supported. `case` accepts multiple values and variables; commas may be omitted.

**Traditional style (colon + break):**

```c
switch(x) {
    case 1:
    case 2:
        result = "small";
        break;
    case 3 4 5:
        result = "medium";
        break;
    default:
        result = "large";
}
//--------------------------------------------------
tag="aad"
switch(x) {
    case "aab" "aac":
        result = 1;
        break;
    case tag:
        result = 2;
        break;
    default:
        result = 3;
}
```

**Modern style (arrow + implicit return):**

```c
switch(x) {
    case 1 2 -> "small";
    case 3 4 5 -> "medium";
    default -> "large";
}
```

**Range matching (Y Language original):**

```c
switch(score) {
    case 90~100 -> "Excellent";
    case 60~89  -> "Pass";
    default     -> "Fail";
}
```

## V. Function Definitions

### 1. Y Functions (Implemented in Y Language)

```go
def max(a, b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}
```

**Usage:**

```y
max(3, 4)          // Returns 4
max("abb", "aab")  // Returns "abb" (string comparison)
```

### 2. Type-I Functions (Simple C/C++ Implementation)

- Easy to implement; open to all developers
- Suitable for encapsulating basic functionality

### 3. Type-II Functions (Advanced C/C++ Implementation)

- Complex to implement; requires deep development expertise
- Available to partners only
- Suitable for high-performance and low-level operations

## VI. Function Call Shorthand

When a function has **exactly one parameter**, parentheses may be omitted:

| Standard Form       | Shorthand Form   |
| ------------------- | ---------------- |
| `inf("pathname")`   | `inf"pathname"`  |
| `deconnect(fd)`     | `deconnect fd`   |
| `len("hello")`      | `len"hello"`     |

## VII. Cross-Language Calls

### Call Rules

```
┌─────────┐     ┌───────────────────┐
│ Y Func  │ ←→  │ Type-I/II Funcs   │
│         │ ←→  │ (C++ impl)        │
└─────────┘     └───────────────────┘
      ↑                  ↑
      └──────────────────┘
        All modules can call each other
```

- Y Language can directly call Type-I and Type-II functions
- C/C++ code can call Y Language functions via platform interfaces
- Cross-module calls are supported regardless of implementation language

## VIII. Data Sources

| Source           | Description                               |
| ---------------- | ----------------------------------------- |
| File import      | Read data from local files                |
| Remote receive   | Receive data over the network             |
| Third-party libs | Extend functionality via external modules |

## IX. Nested Execution

The `runstr` function enables unlimited nesting:

```y
// Call runstr inside a Y program
runstr(inf"program.txt");
// Inside program.txt, further nesting is possible:
...
runstr(inf"sub_program.txt");
...
// sub_program.txt is executed in turn
```

- Supports large-scale project construction
- Supports modular development
- Supports recursive calls

## Appendix A: Operator Precedence (High → Low)

| Priority | Operators                 | Associativity |
| -------- | ------------------------- | ------------- |
| 1        | `()` function call        | Left → Right  |
| 2        | `*` `/` `%`               | Left → Right  |
| 3        | `+` `-`                   | Left → Right  |
| 4        | `>` `>=` `<` `<=` `==`    | Left → Right  |
| 5        | `&&` `||`                 | Left → Right  |
| 6        | `|>` pipe                 | Left → Right  |
| 7        | `=` `+=` `-=` `*=` `/=`   | Right → Left  |

## Appendix B: Data Types Summary

| Type            | Notation                  | Examples                    |
| --------------- | ------------------------- | --------------------------- |
| Integer (int)   | Octal / hex / decimal     | `0743`, `0xABFE`, `123`     |
| Float (num)     | Number with decimal point | `123.0`, `3.14`, `-0.5`     |
| String          | Double quotes             | `"hello"`                   |
| Byte buffer     | Single quotes             | `'112233'`                  |
| Range           | Tilde                     | `10~100`                    |

## Appendix C: Quick Example

```y
// Comprehensive example: grade evaluation
def evaluate(score) {
    return switch(score) {
        case 90~100 -> "Excellent";
        case 75~89  -> "Good";
        case 60~74  -> "Pass";
        default     -> "Fail";
    }
}

// Batch processing
scores = [85 92 67 45 78];
for(i=0;i<5;i++) {
    result = evaluate(scores[i]);
    out("Score " + scores[i] + ": " + result);
}
```

## Appendix D: Memory & Continuous Learning

The Zhiye Unity platform possesses **human-like memory capabilities**. Each interaction builds on previously accumulated knowledge. The platform remembers past data, user-taught instructions, rules, and logic — the more you use it, the better it understands you, achieving true continuous evolution.
