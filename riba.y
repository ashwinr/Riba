%{
#include <stdio.h>
#include <string.h>
#include "riba.h"

#define YYSTYPE char*

// External Declarations
extern int yylex(void);

char last_command[12];

// Parser error handler
void yyerror(const char* error)
{
	fprintf(stderr, "%s: %s\n", last_command, error);
}


extern "C" {
int yywrap()
{
	return 1;
}
}

char* strip_quotes(char* arg)
{
	char* q = arg;
	q++; /* Skip the first quote */
	q[strlen(q)-1] = 0; /* Nullify the last quote */
	return q;
}

%}

%token R_NEWLINE R_NUMBER R_BYTES R_QWORD R_COMMA R_SQUOTE R_EQ
			 R_CMD_OPEN R_CMD_CLOSE R_CMD_GET R_CMD_PUT R_CMD_BATCH
			 R_CMD_COMMIT R_CMD_SNAP R_CMD_UNSNAP R_CMD_PRINT R_CMD_HELP
			 R_ARG_KEYS

%%
commands: 
				| commands command R_NEWLINE
        ;

command: 
       | open_command
       | close_command
       | get_command
       | put_command
       | batch_command
       | commit_command
       | snap_command
       | unsnap_command
       | print_command
			 | help_command
       ;

open_command: R_CMD_OPEN R_QWORD { char* arg1 = strip_quotes($2); leveldb_open(arg1); free($2); }

close_command: R_CMD_CLOSE { leveldb_close(); }

get_command: R_CMD_GET ptype { leveldb_get($2); free($2); }

put_command: R_CMD_PUT ptype ptype { leveldb_put($2, $3); free($2); free($3); }

batch_command: R_CMD_BATCH { leveldb_start_batch(); }

commit_command: R_CMD_COMMIT { leveldb_commit_batch(); }

snap_command: R_CMD_SNAP { leveldb_snap(); }

unsnap_command: R_CMD_UNSNAP { leveldb_unsnap(); }

print_command: R_CMD_PRINT R_ARG_KEYS { leveldb_print($2); free($2); }

help_command: R_CMD_HELP { printf("help\n"); }

ptype: R_NUMBER | R_BYTES | R_QWORD

%%
