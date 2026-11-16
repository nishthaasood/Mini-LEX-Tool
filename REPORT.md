# MICRO PROJECT REPORT
## MINI LEX TOOL USING C++

**Group:** 9  
**Group Members:** Gulshan Sharma, Parth Mukheria, Janhvi, Himanshi Garg, Nishtha Sood, Aadhya Sharma  
**Project Title:** Design LEX Tool Using C/C++/JAVA  
**Implementation Language:** C++17

## 1. Abstract
This project implements a simplified LEX-style lexical analyzer generator in C++. The tool reads token definitions written as regular expressions in a `.lex` specification file and scans a source text to produce a stream of tokens. It applies the longest-match rule, supports skipped patterns such as whitespace and comments, and reports lexical errors with line and column positions.

## 2. Objectives
- To understand lexical analysis, the first phase of a compiler.
- To implement regular-expression-based token recognition.
- To apply the longest-match rule and rule-priority tie-breaking.
- To separate the token specification from the scanner so rules can change without recompiling.
- To detect and report lexical errors with their position.

## 3. Tools and Technologies
- C++17
- Standard Template Library (`vector`, `map`, `string`)
- `std::regex` (ECMAScript syntax)
- GCC / MinGW g++ compiler
- Visual Studio Code or any C++ IDE

## 4. System Design
### 4.1 Specification reader
Each line of the `.lex` file has the form `TOKEN NAME REGEX` or `SKIP NAME REGEX`. Each regular expression is compiled once and stored with its name, type and priority (its order in the file). Comment lines start with `#`. Invalid rules or invalid regular expressions are reported with the line number.

### 4.2 Scanner
The scanner keeps the current position, line and column. At each position it tests every rule with continuous matching (the match must begin exactly at the current position) and keeps the rule with the longest match. `TOKEN` rules emit a token (type, lexeme, line, column); `SKIP` rules consume the input silently. If no rule matches, a lexical error is recorded, one character is skipped, and scanning continues.

### 4.3 Output
The tool prints a token table, the total token count, a per-type summary, and the list of lexical errors.

## 5. Algorithm
1. Load and compile all rules from the `.lex` file.
2. Set `pos = 0`, `line = 1`, `column = 1`.
3. While `pos` is less than the source length:
   1. Match every rule against the remaining input, anchored at `pos`.
   2. Choose the rule with the longest match (earlier rule wins on a tie).
   3. If none matches, report an error at `line:column`, advance by one character.
   4. Otherwise create a token (unless the rule is `SKIP`), update `line`/`column` over the lexeme, and advance by its length.
4. Print the tokens and any errors.

## 6. Sample Specification
```text
SKIP  WHITESPACE [ \t\r\n]+
TOKEN NUMBER     [0-9]+
TOKEN ID         [A-Za-z_][A-Za-z0-9_]*
TOKEN PLUS       \+
TOKEN MINUS      -
TOKEN MULTIPLY   \*
TOKEN DIVIDE     /
TOKEN LPAREN     \(
TOKEN RPAREN     \)
```
A second specification, `specs/clike.lex`, covers keywords, floats, strings, relational operators and `//` comments.

## 7. Sample Runs
**Valid input:** `25 + 30 * (4 + 2)`  
Tokens: NUMBER, PLUS, NUMBER, MULTIPLY, LPAREN, NUMBER, PLUS, NUMBER, RPAREN (9 tokens, no errors).

**Invalid input:** `25 + 30 $ (4 + 2) @ x`  
The scanner reports `unexpected character '$'` at 1:9 and `'@'` at 1:19, and still returns the remaining valid tokens.

**C-like program** (`samples/program.c.txt`): keywords such as `int`/`while` are recognised as KEYWORD, while `integer` is an ID because the longest match wins; `>=` and `!=` are single tokens; `3.14` is a FLOAT; the comment line is skipped.

## 8. Result
The tool correctly converts source text into tokens from a user-supplied specification, demonstrates longest-match and rule-priority behaviour, and reports lexical errors with accurate positions without stopping at the first error.

## 9. Limitations and Future Scope
- Build an NFA/DFA from regular expressions instead of relying on `std::regex`.
- Generate a standalone scanner source file, as the real Flex does.
- Support user-defined actions and start conditions.
- Improve speed by avoiding repeated copying of the remaining input.
- Connect the token stream to a parser (YACC-style tool) as a later extension.

## 10. Conclusion
The project gives practical exposure to the lexical-analysis phase of a compiler. Implementing a specification-driven scanner in C++ demonstrates regular expressions, longest-match tokenisation, whitespace/comment handling and error reporting.
