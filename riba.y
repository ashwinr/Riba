%{
#include <stdio.h>
#include <string.h>
#include "riba.h"

#define YYSTYPE char*

// External Declarations
extern int yylex(void);

// Parser error handler
void yyerror(const char* error)
{
	fprintf(stderr, "%s\n", error);
}


extern "C" {
int yywrap()
{
	return 1;
}
}

%}

%token R_NEWLINE R_NUMBER R_BYTES R_QWORD R_COMMA R_SQUOTE R_EQ
			 R_CMD_OPEN R_CMD_CLOSE R_CMD_GET R_CMD_PUT R_CMD_DELETE
			 R_CMD_BATCH R_CMD_COMMIT R_CMD_SNAP R_CMD_UNSNAP R_CMD_PRINT
			 R_CMD_COMPACT R_CMD_COUNT R_CMD_HELP R_CMD_ABOUT R_CMD_EXIT

%%
commands: 
				| commands command R_NEWLINE
        ;

command: 
       | open_command
       | close_command
       | get_command
       | put_command
       | delete_command
       | batch_command
       | commit_command
       | snap_command
       | unsnap_command
       | print_command
       | compact_command
       | count_command
       | help_command
       | about_command
       | exit_command
       ;

open_command: R_CMD_OPEN R_QWORD { leveldb_open($2); free($2); }

close_command: R_CMD_CLOSE { leveldb_close(); }

get_command: R_CMD_GET ptype { leveldb_get($2); free($2); }

put_command: R_CMD_PUT ptype ptype { leveldb_put($2, $3); free($2); free($3); }

delete_command: R_CMD_DELETE ptype { leveldb_delete($2); free($2); }

batch_command: R_CMD_BATCH { leveldb_start_batch(); }

commit_command: R_CMD_COMMIT { leveldb_commit_batch(); }

snap_command: R_CMD_SNAP { leveldb_snap(); }

unsnap_command: R_CMD_UNSNAP { leveldb_unsnap(); }

print_command: R_CMD_PRINT { leveldb_print(); }

compact_command: R_CMD_COMPACT { leveldb_compact(); }

count_command: R_CMD_COUNT { leveldb_count(); }

help_command: R_CMD_HELP { leveldb_help(); }

about_command: R_CMD_ABOUT { leveldb_about(); }

exit_command: R_CMD_EXIT { leveldb_exit(); }

ptype: R_NUMBER | R_BYTES | R_QWORD

%%
