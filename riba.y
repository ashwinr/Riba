%{
#include <stdio.h>
#include <string.h>

// External Declarations
extern int yylex(void);

// Parser error handler
void yyerror(const char* error)
{
	fprintf(stderr, "Error: %s\n", error);
}


extern "C" {

int yywrap()
{
	return 1;
}

}
%}

%token R_NEWLINE R_NUMBER R_BYTES R_QWORD R_COMMA R_SQUOTE R_EQ
			 R_CMD_OPEN R_CMD_CLOSE R_CMD_GET R_CMD_PUT R_CMD_BATCH
			 R_CMD_COMMIT R_CMD_SNAP R_CMD_UNSNAP R_CMD_LIST

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
       | list_command
       ;

open_command: R_CMD_OPEN R_QWORD { printf("cmd_open\n"); }

close_command: R_CMD_CLOSE { printf("close\n"); }

get_command: R_CMD_GET ptype { printf("get\n"); }

put_command: R_CMD_PUT ptype ptype { printf("put\n"); }

batch_command: R_CMD_BATCH { printf("batch\n"); }

commit_command: R_CMD_COMMIT { printf("commit\n"); }

snap_command: R_CMD_SNAP { printf("snap\n"); }

unsnap_command: R_CMD_UNSNAP { printf("unsnap\n"); }

list_command: R_CMD_LIST ptype { printf("list\n"); }

ptype: R_NUMBER | R_BYTES | R_QWORD

%%
