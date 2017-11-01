/* CommandLineParse.lex
 *
 * This scanner performs scanning of arithmetic expressions
 * to a postfixed expression and passes to the parser for calculation.

 * x = 1 + (2+3)*4;    (type in an expression)
 * ... 
 * ;      
 */

%option noyywrap case-insensitive
%option outfile="CmdLineScanner.cpp"


%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Macro_Defs.h"
#include "CmdLineParserUtility.h"
#include "CmdLineParser.hpp"

					
#define YY_USER_ACTION yylloc->columns (yyleng);

typedef Cmd::CmdLineParser::token token;

char _CommandLine[1024];

/*
	The following definition substitutes the lexer default input
	(from a file) by a string. (G.Shi, 2017/7/7)
*/
#define YY_INPUT(buf,result,max_size) \
	if ( YY_CURRENT_BUFFER_LVALUE->yy_is_interactive ) \
	{ \
		int ch, n; \
		for ( n = 0; \
			n < max_size && \
			(ch = _CommandLine[n]) != 10 && ch != 0; \
			++n ) \
			buf[n] = (char) ch; \
		if ( ch == 10 ) \
			buf[n++] = (char) ch; \
		result = n; \
		_CommandLine[0] = 0; \
	} 

// Redefine so that it returns a token (the default return was 0)
#define yyterminate() 	return token::END

%}

LF			[\r]*[\n]
delim     	[ \t\n]
WS        	{delim}+
alpha    	[a-zA-Z]
letter    	[a-zA-Z]
DIGIT     	[0-9]
IDENTIFIER	{letter}({letter}|{DIGIT})*
/* An identifier could be a variable for assignment, a function name,
   an unknown function name, and other command names such as plot, etc. */
alpha_dig	{alpha}|{DIGIT}
alpha_num	{alpha}|{DIGIT}|[.]

INTEGER		{DIGIT}+
/* nonnegative integers */
FLOAT		({DIGIT}*\.{DIGIT}+)|({DIGIT}+\.{DIGIT}*)
/* .12 or 0.12 are ok */

SIGN		"-"|"+"
SCN		[Ee]({SIGN}?){INTEGER}  
/* scientific notation: 1e-10 */	

FUNC1		"abs"|"fabs"|"ceil"|"floor"
FUNC2		"asin"|"acos"|"atan"|"sin"|"cos"|"tan"|"ctan"
FUNC3		"sinh"|"cosh"|"tanh"
FUNC4		"exp"|"ln"|"log"|"log10"|"sqrt" 
FUNC5		"ones"|"round"

FUNC		{FUNC1}|{FUNC2}|{FUNC3}|{FUNC4}|{FUNC5}

/* Command macros */
CMD_PLOT		plot
CMD_OPEN		open
CMD_FIGURE		figure
CMD_CLOSE		close
CMD_CLEAR 		"clear"
CMD_ALL 		"all"

%%

({INTEGER}{SCN}?)|({FLOAT}{SCN}?) {
		yylval->dval = atof(yytext); 
		/*
		cout << LINE_INFO;
		printf("Scanned a number (text) <%s>", yytext);
		printf("\nScanned a number <%g>", yylval->dval);
		*/
		return (token::NUM);
		}

"pi"|"PI"|"Pi"	{
		yylval->name = strdup(yytext);
		return (token::CONST);
		}

{FUNC}	{
		/* Put this part before the {VARIABLE} part.*/
		/* printf("\n[lex] Got a function %s", yytext); fflush(stdout); */
		yylval->name = strdup(yytext);
		return (token::FUNC);
		}

{CMD_PLOT} {
		yylval->name = strdup(yytext);
		return (token::PLOTOP);
		}

{CMD_FIGURE} {
		yylval->name = strdup(yytext);
		return (token::FIGUREOP);
		}

"clear"|"print"|"all" {
		cout << LINE_INFO;
		printf("\n[lex] Scanned string \"%s\" ", yytext);  fflush(stdout);
		yylval->name = strdup(yytext);
		return (token::STRING_S);
		}

{IDENTIFIER} {
#ifdef TRACE_
		cout << LINE_INFO;
		printf("\n[lex] Scanned an ID: %s ", yytext);  fflush(stdout);
#endif
		yylval->name = strdup(yytext); 
		return (token::ID);
		}
		
"+"|"-" {
		/*
		cout << LINE_INFO << "Scanned add (+,-)" << endl;
		*/
		yylval->ch = yytext[0]; // Need to pass over + or -
		return (token::ADDOP);
		}
		
"*"|"/" {
		/*
		cout << LINE_INFO << "Scanned mult (*,/)" << endl;
		*/
		yylval->ch = yytext[0]; // Need to pass over * or /
		return (token::MULTOP);
		}
		
";" 	{
		yylval->ch = yytext[0]; 
		return (token::EOL);
		}
		
[,=:()] {
		/*
		cout << LINE_INFO;
		printf("[lex] Scanned char '%c' ", yytext[0]);
		*/
		yylval->ch = yytext[0]; 
		return Cmd::CmdLineParser::token_type(yytext[0]); 
		}
		
{WS}    {}

%%

#if 0
"(" 	{
		cout << LINE_INFO;
		printf("[lex] Scanned char '%c' ", yytext[0]);
		return (token::PARENTHESIS_L);
		}
		
")" 	{
		cout << LINE_INFO;
		printf("[lex] Scanned char '%c' ", yytext[0]);
		return (token::PARENTHESIS_R);
		}
#endif



/*
main()
{
	Cmdlex();
}
*/
