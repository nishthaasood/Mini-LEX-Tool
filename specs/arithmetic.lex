# Mini LEX specification
# Format: TOKEN token_name regular_expression
# SKIP is used for whitespace and comments.
SKIP WHITESPACE [ \t\r\n]+
TOKEN NUMBER [0-9]+
TOKEN ID [A-Za-z_][A-Za-z0-9_]*
TOKEN PLUS \+
TOKEN MINUS -
TOKEN MULTIPLY \*
TOKEN DIVIDE /
TOKEN LPAREN \(
TOKEN RPAREN \)
TOKEN ASSIGN =
TOKEN SEMICOLON ;
