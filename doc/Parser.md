# Parser Syntax and Rules

## Formal rules for my parser
<program> := <expression>*
<expression> := number | <quoted-expression> | symbol | string | <hash-expr>
                <list>
<quoted-expression> := quote <expression>
<hash-expr> := <boolean> | <literal> | <vector-literal>
<boolean> := hash "t" | "f"
<literal> := hash symbol
<vector-literal> := hash <list>
<list> := left-paren <expression>* right-paren


## The syntax tree

The syntax tree is a structure containing
