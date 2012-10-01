#include <iostream>
#include <exception>
#include <readline/readline.h>
#include <readline/history.h>

#include "riba.h"
#include "lex.yy.h"

int yyparse();

int main(int argc, char** argv)
{
  char* line_buf;
  // If an argument is given on the command line, open it
  //  for example use 'riba my_leveldb' to open LevelDB in my_leveldb
  if(argc >= 2)
  {
	leveldb_open(argv[1]);
  }

  // Disable any default autocomplete
  rl_bind_key('\t', rl_insert);

  // If processing from stdin, evaluate continously
  while(1)
  {
    line_buf = readline(">>> ");

    // Encountered an EOF (i.e. C-d), so exit gracefully
    if(line_buf == NULL)
    {
      std::cout << std::endl;
      leveldb_exit();
      continue; // continue in case the user didn't want to exit
    }
    
    if(line_buf[0] == 0) continue;
    // Add a newline to the end of the buffer
    std::string yyline(line_buf);
    yyline += "\n";

    try {
      YY_BUFFER_STATE yybs = yy_scan_string(yyline.c_str());
      yy_switch_to_buffer(yybs);
      yyparse();
      yy_delete_buffer(yybs);
      add_history(line_buf);
    }
    catch(...) {} /* do nothing */
  }

  // Unnecessary
  return 0;
}
