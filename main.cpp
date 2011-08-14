#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

#include "lex.yy.h"

int yyparse();

int main(int argc, char** argv)
{
  char* line_buf;

  // Disable any default autocomplete
  rl_bind_key('\t', rl_insert);

  // If processing from stdin, evaluate continously
  while((line_buf=readline(">>> ")) != NULL)
  {
    if(line_buf[0] == 0) continue;
    // Add a newline to the end of the buffer
    std::string yyline(line_buf);
    yyline += "\n";

    YY_BUFFER_STATE yybs = yy_scan_string(yyline.c_str());
    yy_switch_to_buffer(yybs);
    yyparse();
    yy_delete_buffer(yybs);
    add_history(line_buf);
  }

  // Unnecessary
  return 0;
}
