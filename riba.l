%{

#define YYSTYPE char*

#include <string.h>
#include "riba.tab.h"
#include "riba.h"

extern YYSTYPE yylval;

%}

%%

[ \t]+ ;

[\n] { return R_NEWLINE; }

"," { return R_COMMA; }

"=" { return R_EQ; }

^open { return R_CMD_OPEN; }

^close$ { return R_CMD_CLOSE; }

^get { return R_CMD_GET; }

^put { return R_CMD_PUT; }

^batch$ { return R_CMD_BATCH; }

^commit$ { return R_CMD_COMMIT; }

^snap$ { return R_CMD_SNAP; }

^unsnap$ { return R_CMD_UNSNAP; }

^print { return R_CMD_PRINT; }

^help { return R_CMD_HELP; }

keys { yylval = strdup(yytext); return R_ARG_KEYS; }

\'[^']+\' { yylval = strdup(yytext); return R_QWORD; }

[0-9]+ { yylval = strdup(yytext); return R_NUMBER; }

0x[0-9a-fA-F]+ { yylval = strdup(yytext); return R_BYTES; }

. /* Ignore everything else */ ;

%%
