<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/%E6%94%AF%E5%8F%B6-Y%E8%AF%AD%E8%A8%80.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>
# Zhiye-Y Language Basics

## Overview

Zhiye is a language platform developed for natural language understanding, which retains machine language (Y language) functionality and provides integration interfaces open to C/C++.

## Y Language Features

### 1. Numeric Syntax

- Supports integers and decimals: `123`, `-464`, `78.64`, `-123.96`, `0xFA12`, `$i:64018`
- Can be used directly upon input

### 2. Data Syntax

#### 2.1. "..." → String (C-style)

- Handles escape characters: `\n`, `\t`, `\"`, `\\`, `\xHH`, etc.
- Identical to C language

#### 2.2. '...' → Data String (Y Language Original)

- Supports multiple styles and mixed styles (supports: 8-bit, 16-bit, 32-bit, 64-bit), e.g.:
  '1122', '11 22', '\x11\x22',
  '\u3132\u42AA',
  '\U11223344',
  '0x11,0x2233,0x44556677... up to 64-bit support'
- Supports nested double-quote strings. **Nesting Rules:**
  - **Independent Nesting** `'"..."'` → Treated as a quoted string, no escaping.
  - **Non-independent Nesting** `'.."..."..'` → The part inside double quotes is interpreted as a string and then converted to 8-bit hexadecimal.

#### 2.3. Examples

```c++
"abc\"123\""        // String abc"123"
"\x31\x42\x53\r\n"  // Supports escape characters (same as C/C++, JAVA)
'112233'            // Data string of 3 bytes: 0x11 0x22 0x33
'11 22 33'          // 3-byte data (same content as above)
'\x11\x22\x33'      // 3-byte data (same content as above)
'0x11,0x22,0x33'    // 3-byte data (same content as above)
'\u3132\u42AA\uFE53' // Data string (little-endian): 32 31 AA 42 53 FE
'\U11223344\U55667788' // Data string (little-endian): 44 33 22 11 88 77 66 55
'""aaa":123;"bbb":"hehe""' // Independent nesting, string "aaa":123;"bbb":"hehe"
'123242"abc"'       // Non-independent nesting, data string 12 32 42 61 62 63
'0x3132,0x42AA,0x11223344,0x1122334455667788'   // 32-bit: data string (little-endian): 32 31 AA 42 44 33 22 11 88 77 66 55 00 00 00 00
'0x3132,0x42AA,0x11223344,0x1122334455667788'   // 64-bit: data string (little-endian): 32 31 AA 42 44 33 22 11 88 77 66 55 44 33 22 11
```

### 3. Parameter Definition

- Supports global and local parameters
- No need to declare type; type is confirmed upon assignment (similar to Python)

```y
a = 123        // Numeric type
b = "hello"    // String type
c = '112233'   // Data type
```

### 4. Expression Operations

#### Numeric Operations

```y
Addition (+) Subtraction (-) Multiplication (*) Division (/) Modulo (%)
Compound operations: +=, -=, /=, *=, %=
Bitwise operations: AND (&) OR (|) XOR (^)
Logical operations: Logical AND (&&) Logical OR (||)
Comparison operations: >, >=, <, <=, ==
Assignment =
```

#### String Operations

```y
Addition (+) Comparison (>, >=, <, <=, ==) Assignment (=)
```

### 5. Conditional Statements

Syntax is the same as C language

```c++
if (condition) { ... } else { ... }
while (condition) { ... } // Supports unconditional loops, e.g.: while() {a++;}
for (init; condition; increment) { ... }
```

### 6. Methods (Functions)

#### Y Functions (Implemented in Y Language)

```go
def max(a, b) {
    if (a > b)
        return a;
    else
        return b;
}
// Call: max(3,4) returns 4
// Call: max("abb","aab") returns "abb"
```

#### Type I Functions (Implemented in C/C++)

- Simple implementation, open to all programming enthusiasts

#### Type II Functions (Implemented in C/C++)

- Complex implementation, requires solid development foundation, only open to partners

## Zhiye Main Program Functional Functions

Refer to the documentation "Zhiye Built-in Interfaces.md"

## Special Syntax Rules

### Simplified Function Calls

When a function has only one parameter, **parentheses can be omitted**:

```c++
// Standard syntax
inf("pathname")
// Simplified syntax
inf"pathname"

// Standard syntax
deconnect(fd)
// Simplified syntax
deconnect fd
```

## Data Sources

- Supports file import
- Supports remote data reception
- Supports third-party library extension functionality

## Nested Execution

The `runfs` function enables infinite nesting. `runfs` can still be called within a Y language program, facilitating the construction of large-scale projects.
[File Content End]