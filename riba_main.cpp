#include <iostream>

int yyparse ();

int main(int argc, char** argv)
{
  // If processing from stdin, keep on evaluating
  // Catch and output errors
  while(1)
  {
    yyparse();
  }

  // Unnecessary
  return 0;
}
