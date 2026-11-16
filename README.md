# Mini LEX Tool (C++17)

A small educational implementation of a LEX-like lexical-analyzer generator, written from scratch in C++17.
The scanner is driven by a `.lex` specification file, so the token rules can be changed without touching the C++ code.

**Micro Project:** Design LEX Tool Using C/C++/JAVA (Group 9)

## Features
- Reads token rules from a `.lex` file using regular expressions (`std::regex`, ECMAScript syntax).
- Longest-prefix (longest-match) rule; ties are resolved by rule order in the spec.
- `SKIP` rules for whitespace and comments (consumed, no token produced).
- Reports lexical errors with line and column, then continues scanning.
- Prints a token table (lexeme, token type, line:column) and a token-count summary.

## Requirements
- C++17 compiler (g++ 8+ recommended). No third-party libraries.

## Compile
Windows (PowerShell):
```powershell
g++ -std=c++17 -O2 -Wall main.cpp -o mini_lex.exe
```
Linux / macOS:
```bash
g++ -std=c++17 -O2 -Wall main.cpp -o mini_lex
```

## Run
```bash
./mini_lex <lex-spec> <source-file>
```
Examples:
```bash
./mini_lex specs/arithmetic.lex samples/expression.txt
./mini_lex specs/arithmetic.lex samples/identifier_expression.txt
./mini_lex specs/arithmetic.lex samples/invalid_character.txt   # shows lexical errors
./mini_lex specs/clike.lex samples/program.c.txt                # keywords, floats, strings, comments
```
Windows users can use `.\mini_lex.exe` instead of `./mini_lex`.

Exit codes: `0` success, `1` bad arguments/file/spec, `2` lexical errors found.

## LEX specification format
Each non-comment line is:
```
TOKEN NAME REGULAR_EXPRESSION
SKIP  NAME REGULAR_EXPRESSION
```
Lines starting with `#` are comments. Example (`specs/arithmetic.lex`):
```
SKIP  WHITESPACE [ \t\r\n]+
TOKEN NUMBER     [0-9]+
TOKEN ID         [A-Za-z_][A-Za-z0-9_]*
TOKEN PLUS       \+
```

## Project layout
```
main.cpp                      LEX tool source
CMakeLists.txt                optional CMake build
specs/arithmetic.lex          expression tokens
specs/clike.lex               small C-like language tokens
samples/                      test inputs (valid and invalid)
REPORT.md                     project report
VIVA.md                       viva questions and answers
```

## Limitations
- Educational subset, not a replacement for Flex.
- Uses `std::regex` directly instead of building its own NFA/DFA.
- No user-defined actions or `yylex()`-style C code generation.
- Re-scans the remaining input for every token (fine for small inputs, not optimized).
