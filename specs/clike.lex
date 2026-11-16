# Mini LEX specification for a small C-like language
# Format: TOKEN token_name regular_expression   |   SKIP name regular_expression
# Rule order matters only for ties (same length): earlier rule wins.
SKIP WHITESPACE [ \t\r\n]+
SKIP COMMENT //[^\n]*
TOKEN KEYWORD (int|float|if|else|while|return)\b
TOKEN FLOAT [0-9]+\.[0-9]+
TOKEN NUMBER [0-9]+
TOKEN ID [A-Za-z_][A-Za-z0-9_]*
TOKEN STRING "[^"\n]*"
TOKEN EQ ==
TOKEN NEQ !=
TOKEN LE <=
TOKEN GE >=
TOKEN LT <
TOKEN GT >
TOKEN ASSIGN =
TOKEN PLUS \+
TOKEN MINUS -
TOKEN MULTIPLY \*
TOKEN DIVIDE /
TOKEN LPAREN \(
TOKEN RPAREN \)
TOKEN LBRACE \{
TOKEN RBRACE \}
TOKEN SEMICOLON ;
TOKEN COMMA ,
