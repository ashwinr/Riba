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

^delete { return R_CMD_DELETE; }

^batch$ { return R_CMD_BATCH; }

^commit$ { return R_CMD_COMMIT; }

^snap$ { return R_CMD_SNAP; }

^unsnap$ { return R_CMD_UNSNAP; }

^print$ { return R_CMD_PRINT; }

^compact$ { return R_CMD_COMPACT; }

^count$ { return R_CMD_COUNT; }

^help$ { return R_CMD_HELP; }

^about$ { return R_CMD_ABOUT; }

^exit$ { return R_CMD_EXIT; }

\'[^']+\' { yylval = strdup(yytext+1); yylval[strlen(yylval)-1] = 0; return R_QWORD; }

[0-9]+ { yylval = strdup(yytext); return R_NUMBER; }

0x[0-9a-fA-F]+ { yylval = strdup(yytext); return R_BYTES; }

. /* Ignore everything else */ ;

%%
