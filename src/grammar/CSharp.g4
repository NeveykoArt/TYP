grammar CSharp;

program: class EOF;
class: CLASS ID CLB expression CRB;
expression: (expressions)*;
expressions: ( func_def | (assign_statement SEMICOLON) | if_statement | for_statement | print_statement | read_statement);
assign_statement: (ID | var_def) (
				ASSIGN (
					(arg (BINARY_OP arg)?)
					| func_call
				       )
				)?;
literal: TEXT | NUMBER | CHARv | FLOAT_NUMBER;
var_def: VAR ID | VAR SLP SRP ID;
func_def: (VAR | (KEYWORD* VOID) | VOID) ID RLP pars RRP (CLB scope return_statement CRB);
scope: (statement)*;
return_statement: RETURN arg SEMICOLON;
statement: (func_call SEMICOLON)
	| (assign_statement SEMICOLON)
	| if_statement | for_statement | (kw_statement SEMICOLON) | print_statement | read_statement;
func_call: (ID | WRITELN | READLN) RLP args RRP;
args: (arg (COMMA arg)*)?;
arg: ID | literal;
pars: (var_def (COMMA var_def)*)?;
if_statement:
	IF RLP ID (LOGIC_OP arg)? RRP CLB scope CRB (else_statement)?;
else_statement: ELSE CLB scope CRB;
for_statement: FOR RLP assign_statement SEMICOLON for_condition SEMICOLON for_operation RRP CLB scope CRB;
for_condition: ID LOGIC_OP arg;
for_operation: (ID UNARYMATHEXP) | assign_statement;
kw_statement: KEYWORD;
print_statement: WRITELN RLP ID RRP SEMICOLON;
read_statement: READLN RLP ID RRP SEMICOLON;

KEYWORD:
	'abstract'
	| 'as'
	| 'base'
	| 'break'
	| 'case'
	| 'catch'
	| 'checked'
	| 'const'
	| 'continue'
	| 'default'
	| 'delegate'
	| 'do'
	| 'enum'
	| 'event'
	| 'explicit'
	| 'extern'
	| 'false'
	| 'finally'
	| 'fixed'
	| 'foreach'
	| 'goto'
	| 'implicit'
	| 'in'
	| 'interface'
	| 'internal'
	| 'lock'
	| 'namespace'
	| 'new'
	| 'null'
	| 'object'
	| 'operator'
	| 'out'
	| 'override'
	| 'params'
	| 'private'
	| 'protected'
	| 'public'
	| 'readonly'
	| 'ref'
	| 'sealed'
	| 'sizeof'
	| 'stackalloc'
	| 'static'
	| 'struct'
	| 'switch'
	| 'this'
	| 'throw'
	| 'true'
	| 'try'
	| 'typeof'
	| 'unchecked'
	| 'unsafe'
	| 'using'
	| 'virtual'
	| 'volatile'
	| 'while';

CLASS: 'class';
RETURN: 'return';
VOID: 'void';

VAR:
	INT
	| FLOAT
	| DOUBLE
	| DECIMAL
	| SBYTE
	| BYTE
	| SHORT
	| USHORT
	| UINT
	| LONG
	| ULONG
	| STRING
	| CHAR
	| BOOL;

INT: 'int' | 'System.Int32';
FLOAT: 'float' | 'System.Single';
DOUBLE: 'double' | 'System.Double';
DECIMAL: 'decimal' | 'System.Decimal';
SBYTE: 'sbyte' | 'System.SByte';
BYTE: 'byte' | 'System.Byte';
SHORT: 'short' | 'System.Int16';
USHORT: 'ushort' | 'System.UInt16';
UINT: 'uint' | 'System.UInt32';
LONG: 'long' | 'System.Int64';
ULONG: 'ulong' | 'System.UInt64';
STRING: 'string' | 'System.String';
CHAR: 'char';
BOOL: 'bool';

WRITELN: 'WriteLine';
READLN: 'ReadLine';
IF: 'if';
ELSE: 'else';
FOR: 'for';
ID: SYMBOL (SYMBOL | DIGIT | '.' SYMBOL)*;
fragment SYMBOL: [A-Za-z];
fragment DIGIT: [0-9];
UNARYMATHEXP: '++' | '--';
ASSIGN: '=';
BINARY_OP: PLUS | MINUS | PERS | DIVISION | MUL;
fragment MUL: '*';
fragment DIVISION: '/';
fragment PLUS: '+';
fragment MINUS: '-';
fragment PERS: '%';

LOGIC_OP: EQ | LESS | MR | MRQ | LESSQ | NEQ;
fragment EQ: '==';
fragment LESS: '<';
fragment MR: '>';
fragment MRQ: '>=';
fragment LESSQ: '<=';
fragment NEQ: '!=';

SEMICOLON: ';';
RLP: '(';
RRP: ')';
CLB: '{';
SLP: '[';
SRP: ']';
CRB: '}';
DOT: '.';
COLON: ':';
COMMA: ',';
APOSTROPH: '\'';
DQUOTES: '"';
NUMBER: '0' | DIGITNOZERO (DIGIT)*;

TEXT: DQUOTES [0-9A-Za-z!?@#$%^&* ,.]+ DQUOTES;
CHARv: APOSTROPH [0-9A-Za-z!?@#$%^&*.] APOSTROPH;
FLOAT_NUMBER: NUMBER DOT NUMBER;

fragment DIGITNOZERO: [1-9];


WS: [ \r\t\n]+ -> skip;
