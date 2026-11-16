# Viva Preparation: Mini LEX Tool

## Common questions and short answers

**1. What is LEX?**  
LEX is a lexical-analyzer generator. It converts regular-expression rules into a scanner that identifies tokens.

**2. What is lexical analysis?**  
The first phase of a compiler. It reads the source characters and groups them into tokens, removing whitespace and comments.

**3. What is a lexeme?**  
The actual character sequence in the source, such as `count` or `123`.

**4. What is a token?**  
A category assigned to a lexeme, such as ID, NUMBER or PLUS.

**5. What is a pattern?**  
The rule (regular expression) that describes all lexemes of a token, e.g. `[0-9]+` for NUMBER.

**6. What is the longest-match rule?**  
The scanner selects the rule that consumes the most input characters. Example: `>=` is one GE token, not GT then ASSIGN; `integer` is one ID, not the keyword `int` followed by `eger`.

**7. What happens if two rules match the same length?**  
The rule written first in the `.lex` file wins. That is why `KEYWORD` is placed before `ID`.

**8. What is a regular expression?**  
A notation to describe sets of strings. Our tool uses C++ `std::regex` (ECMAScript syntax) to define token patterns.

**9. What do SKIP rules do?**  
They match text (whitespace, comments) and consume it without producing any token.

**10. What is a lexical error and how do you handle it?**  
Input that matches no rule, e.g. `$`. We report its line and column, skip that character, and continue so that several errors can be found in one run.

**11. How are line and column tracked?**  
After each lexeme we walk its characters: on `\n` we increase the line and reset the column to 1; otherwise we increase the column.

**12. How does your scanner match at the current position only?**  
We use `regex_search` with `match_continuous`, which forces the match to start exactly at the current position.

**13. Why a separate `.lex` file?**  
It separates the specification from the scanner so token rules can change without recompiling the C++ program. This is the same idea as Flex.

**14. How is this different from real Flex?**  
Flex converts the rules into an NFA, then a DFA, and generates C code. Our tool is an interpreter-style scanner built on `std::regex`, with no code generation or actions.

**15. What is the time cost?**  
For each token we test every rule on the remaining input, so it is slower than a DFA-based scanner but fine for small inputs.

**16. How does LEX connect to the rest of a compiler?**  
The token stream produced by the lexer is the input to the parser (YACC-style tool) in the next phase.

## Demo sequence
1. `./mini_lex specs/arithmetic.lex samples/expression.txt` shows the token table.
2. `./mini_lex specs/arithmetic.lex samples/invalid_character.txt` shows lexical errors with positions.
3. `./mini_lex specs/clike.lex samples/program.c.txt` shows keywords vs identifiers, floats, strings, comments and multi-character operators.
4. Explain the longest-match rule and rule priority using `integer` and `>=`.
