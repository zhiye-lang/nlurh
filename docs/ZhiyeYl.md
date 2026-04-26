<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/%E6%94%AF%E5%8F%B6-Y%E8%AF%AD%E8%A8%80.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>
# Zhiye Y-Language Basics

## Overview

Zhiye is a programming language platform developed for natural language understanding, while retaining machine language (Y-language) capabilities and providing integration interfaces open to C/C++.

**Core Design Philosophy:** Make machine language easier to understand, and make AI easier to generate.


## I. Y-Language Fundamental Features

### 1. Numeric Types

Supports floating-point and integer values (decimal, hexadecimal, octal).

| Type            | Format             | Examples                                      |
| --------------- | ------------------ | --------------------------------------------- |
| Float (`num`)   | With decimal point | `78.64`, `-123.96`, `100.0`                   |
| Integer (`int`) | Plain number       | `123`, `-464`, `0xFA12` (hex), `0766` (octal) |

### 2. String Type

**Double-quoted strings `"..."`** - Fully compatible with C language

```c
"abc\"123\""           // String: abc"123"
"\x31\x42\x53\r\n"     // Hex escape + CR/LF
"Hello\nWorld\t!"      // Standard escape characters
```

**Supported escape characters:** `\n`, `\t`, `\r`, `\"`, `\\`, `\xHH`

### 3. Data Buffer Type (Y-Language Original)

**Single-quoted data buffers `'...'`** - Used for storing binary data, supports mixed input formats.

| Format         | Example                    | Description                       |
| -------------- | -------------------------- | --------------------------------- |
| Continuous hex | `'112233'`                 | 3 bytes: 0x11, 0x22, 0x33         |
| Spaced hex     | `'11 22 33'`               | Same as above, spaces for clarity |
| Escape format  | `'\x11\x22\x33'`           | C-style hexadecimal               |
| 16-bit Unicode | `'\u3132\u42AA'`           | Little-endian: 32 31 AA 42        |
| 32-bit Unicode | `'\U11223344'`             | Little-endian: 44 33 22 11        |
| Mixed format   | `'0x11,0x2233,0x44556677'` | Auto-detect bit width             |

**Nested string rules:**

```c
'"{"run":"max","data":7}"'     // Independent nesting: starts with '" and ends with "', treated as a whole string without escaping
'123242"abc"'                 // Non-independent nesting: string portion converted to 8-bit hex
```

### 4. Range Type

Used to represent numeric intervals:

```y
a = 10~100          // Range: 10 to 100. 64-bit platform supports step parameter: 10~100:2 (10 to 100, step 2)
```


## II. Variables and Parameters

### 1. Variable Definition

No type declaration needed; type is determined automatically upon assignment (similar to Python):

```y
a = 123              // Numeric type
b = "hello"          // String type
c = '112233'         // Data buffer type
d = 10~100           // Range type
```

### 2. Scope

- Supports global and local parameters
- Variables defined inside functions are local variables
- Variables defined at the top level are global variables

### 3. Parameter Modifier `[..]`

`[..]` is used for subscript or range access on parameters. It is a core modifier syntax of the Y-language parameter system:

**Index access `buf[n]`**: Accesses the `n+1`-th sub-element (0-based indexing), consistent with mainstream programming languages:

```c
subdata[3]       // Access the 4th sub-parameter of subdata
buf[0]           // Access the 1st element of buf
```

**Range access `buf[s~e]`**: Accesses all content from position `s` to position `e` (inclusive, 0-based indexing). This is an extended Y-language syntax:

```c
buf[0~4]         // Access all content from the 1st to the 5th element of buf
```

> **Note:** `[..]` is a parameter modifier that follows immediately after the parameter name. It is not a standalone operator. It acts on the parameter's own content to extract data from a specified range.


## III. Expressions and Operators

### 1. Numeric Operations

| Category            | Operators                     |
| ------------------- | ----------------------------- |
| Arithmetic          | `+` `-` `*` `/` `%` `.*` `./` |
| Compound assignment | `+=` `-=` `*=` `/=` `%=`      |
| Bitwise             | `&` `|` `^`                   |
| Logical             | `&&` `||`                     |
| Comparison          | `>` `>=` `<` `<=` `==`        |
| Assignment          | `=`                           |

### 2. String Operations

| Operation            | Description              | Example              |
| -------------------- | ------------------------ | -------------------- |
| `+`                  | String concatenation     | `"Hello" + " World"` |
| `==`                 | Equality comparison      | `"abc" == "abc"`     |
| `>`, `<`, `>=`, `<=` | Lexicographic comparison | `"abb" > "aab"`      |
| `=`                  | Assignment               | `str = "new"`        |

### 3. Cross-Type Operation Rules

The `+` operator in Y-language supports operations between different data types. **The result type is determined by the left operand:**

| Left Type | Right Type | Operation | Result Type | Description                                         |
| --------- | ---------- | --------- | ----------- | --------------------------------------------------- |
| str       | str        | `+`       | str         | String concatenation                                |
| str       | buf        | `+`       | str         | buf converted to readable string, then concatenated |
| buf       | str        | `+`       | buf         | String appended to end of data buffer               |
| buf       | buf        | `+`       | buf         | Data buffer concatenation                           |

**Encoding inspection tip (UTF-8 encoded platform)**: Using the `str + buf` type conversion rule, you can easily inspect character encodings:

```c
'' + "国"       // Returns buf, showing the UTF-8 encoding of "国": buf[3]' E5 9B BD'
"" + 'E5 9B BD' // Returns str, converting UTF-8 encoding back to character: str[1]"国"
```

> **Core rule:** The result data type of `+` follows the left operand — `str + buf` yields str, `buf + str` yields buf.

### 5. Composite Data (Arrays / Matrices)

Y Language has built-in vector and matrix types for efficient numerical computation.

- **Vector:** Space-separated values, e.g. `[1 2 3]`
- **Matrix:** Rows separated by `;` or `,`, e.g. `[1 2; 3 4]` or `[1 2, 3 4]`

Supports the full set of arithmetic operators (`+`, `-`, `*`, `/`, `.*`, `./`), where `*` is matrix multiplication and `.*` is element-wise multiplication.

```bat
[11 22 33]*5              ->  [int:55 int:110 int:165 ]
[11 22 33]+[44 55 66]     ->  [int:55 int:77 int:99 ]
[1.1 2.2 3.3]*[4.4 5.5 6.6] -> [num:4.84 num:12.1 num:21.78 ]
[1 2]/[3 4]               ->  [int:0 int:0 ]
[1.0 2.0]/[3 4]           ->  [num:0.333333 num:0.5 ]
// Matrix multiplication
a=[1 2; 3 4]; b=[5 6; 7 8];
c=a*b;
out c  ->  [c]sub(int:19 int:22  ,  int:43 int:50 )
```


## IV. Control Flow Statements

Syntax is similar to C/Java/JavaScript:

### 1. Conditional Statements

```c
if(condition) {
    // Execute when condition is true
} else {
    // Execute when condition is false
}
```

### 2. Loop Statements

```c
// while Standard loop(Same as C/C++, Java, JavaScript, and C#)
while(condition) {
    // Loop body(supports: break, continue, return)
}

// while unconditional loop (Y-language original)
while() {
    a++;  // Infinite loop (supports: break, continue, return)。
}

// Constant-count loop (Y-language original)
while(100) {
    // Loops 100 times(supports: break, continue, return)
}
// Note: Unlike other languages, while(1) is NOT an infinite loop — it executes only once

// for loop(Same as C/C++, Java, JavaScript, and C#)
for (init; condition; increment) {
    // Loop body(supports: break, continue, return)
}
```

### 3. Switch Statements

Supports two styles of `switch` syntax (case parameters support variables; commas can be omitted for multiple values):

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
    case "aab":
    case "aac":
        result = 1;
        break;
    case tag:
        result = 2;
        break;
    default:
        result = 3;
}
```

**Modern style (arrow + auto return):**

```c
switch(x) {
    case 1 2 -> "small";
    case 3 4 5 -> "medium";
    default -> "large";
}
```

**Range matching:**

```c
switch(score) {
    case 90~100 -> "Excellent";
    case 60~89 -> "Pass";
    default -> "Fail";
}
```


## V. Function Definition

### 1. Y Functions (Y-Language Implementation)

```go
def max(a, b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}
```

**Call examples:**

```y
max(3, 4)        // Returns 4
max("abb", "aab") // Returns "abb" (string comparison)
```

### 2. Type-I Functions (Simple C/C++ Implementation)

- Simple to implement, open to all programming enthusiasts
- Suitable for encapsulating basic functionality

### 3. Type-II Functions (Advanced C/C++ Implementation)

- Complex to implement, requires deep development expertise
- Only available to partners
- Suitable for high-performance, low-level operations


## VI. Simplified Function Call Rules

When a function has **only one parameter**, parentheses can be omitted:

| Standard syntax   | Simplified syntax |
| ----------------- | ----------------- |
| `inf("pathname")` | `inf"pathname"`   |
| `deconnect(fd)`   | `deconnect fd`    |
| `len("hello")`    | `len"hello"`      |


## VII. Cross-Language Invocation

### Invocation Rules

```apl
┌─────────┐     ┌───────────┐  
│ Y-lang   │ ←→  │ Type-I/II │ 
│ Functions│ ←→  │ (C/C++)   │ 
└─────────┘     └───────────┘  
      ↑               ↑  
      └───────────────┘
       All modules can call each other
```

- Y-language can directly call Type-I and Type-II functions
- C/C++ code can call Y-language functions through platform interfaces
- Supports cross-module invocation regardless of implementation language


## VIII. Data Sources

| Source                | Description                            |
| --------------------- | -------------------------------------- |
| File import           | Supports reading data from local files |
| Remote reception      | Supports network data reception        |
| Third-party libraries | Supports extension modules             |


## IX. Nested Execution

Unlimited nesting can be achieved through the `runstr` function:

```y
// Call runstr within a Y-language program
runstr(inf"program.txt");
// Inside program.txt, you can still...
...
runstr(inf"sub_program.txt"); 
...
// Call sub_program.txt
```

- Supports building large-scale projects
- Supports modular development
- Supports recursive calls


## Appendix A: Operator Precedence (High to Low)

| Precedence | Operators               | Associativity |
| ---------- | ----------------------- | ------------- |
| 1          | `()` function call      | Left→Right    |
| 2          | `*` `/` `%`             | Left→Right    |
| 3          | `+` `-`                 | Left→Right    |
| 4          | `>` `>=` `<` `<=` `==`  | Left→Right    |
| 5          | `&&` `||`               | Left→Right    |
| 6          | `=` `+=` `-=` `*=` `/=` | Right→Left    |

## Appendix B: Data Type Summary

| Type          | Representation                                    | Example           |
| ------------- | ------------------------------------------------- | ----------------- |
| Integer (int) | Octal, hexadecimal, decimal. (Integer storage)    | 0743, 0xABFE, 123 |
| Number (num)  | Numeric value with decimal point. (Float storage) | 123.0, 3.14, -0.5 |
| String        | Double quotes                                     | `"hello"`         |
| Data buffer   | Single quotes                                     | `'112233'`        |
| Range         | Tilde                                             | `10~100`          |

## Appendix C: Quick Example

```y
// Comprehensive example: Score evaluation program
def evaluate(score) {
    return switch(score) {
        case 90~100 -> "Excellent";
        case 75~89 -> "Good";
        case 60~74 -> "Pass";
        default -> "Fail";
    }
}

// Batch processing
scores = [85 92 67 45 78];
for(i=0;i<5;i++) {
    result = evaluate(scores[i]);
    out("Score " + scores[i] + ": " + result);
}
```
