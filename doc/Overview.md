# Lexer Rules

## Comments
All text after a ';', and before a '\n', shall be ignored.
All text after a "#|" and before a "|#" shall be ignored.

## Tokens

### Single Character Tokens
'(', ')', '#', '''
These characters shall be parsed as their own tokens.

### Symbol Tokens
Any token starting with a non-numerical value shall be parsed as a
symbol token.

### Number Token
Any token starting with a numerical value shall be parsed as a
number token.

### String Token
Any token starting with '"', and ending with '"', shall be parsed as
a string token.

### Boolean Token
"#t" and "#f" will be parsed as boolean tokens.

#### Escape Sequences
\\" shall be parsed as ", while \\\\" shall be parsed as \\".
