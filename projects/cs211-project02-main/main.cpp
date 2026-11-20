/*main.cpp*/

/**
  * @brief simple debugger for nuPython programs.
  *
  * To run:
  *
  *   ./a.out filename.py
  *
  * @author original template by Prof. Joe Hummel
  * @author ?
  * @author ?
  * @author ?  
  * @note Northwestern University
  */

#include <iostream>
#include <string>
#include <map>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::stoi;
using std::map;

#include "pgutil.h"

//
// Since we are working in C++, we have to tell the
// C++ compiler that the functions in the provided
// nuPython code are C functions:
//
extern "C" {
  #include "tokenqueue.h"
  #include "parser.h"
  #include "programgraph.h"
  #include "ram.h"
  #include "execute.h"
}


/**
  * @brief help command
  *
  * @return void
  */
void help_command()
{
  cout << "Available commands:" << endl;
  cout << "h -> help with list of commands" << endl;
  cout << "q -> quit the debugger" << endl;
  cout << "b n -> set breakpoint at line n" << endl;
  cout << "rb n -> remove breakpoint at line n" << endl;
  cout << "cb -> clear all breakpoints" << endl;
  cout << "lb -> list all breakpoints" << endl;
  cout << "pg -> print entire program graph" << endl;
  cout << "r -> run the program from the current stmt" << endl;
  cout << "s -> step by executing the current stmt" << endl;
  cout << "sm -> show memory contents" << endl;
  cout << "w -> what line will execute next?" << endl;
}


/**
  * @brief main
  *
  * usage: ./a.out filename.py
  * 
  * If a filename is given, the file is opened and serves as
  * input to the debugger.
  *
  * @param argc # of arguments to program
  * @param argv Array of actual arguments
  * @return 0 Success
  */
void breakpt(map<int, int>& breakpoints, int newpt){
  if(breakpoints.contains(newpt)==true){
    cout<<"breakpoint exists"<<endl;
  }
  else{
    breakpoints[newpt]=0;
    cout<<"breakpoint set"<<endl;
  }
}
void removebp(map<int, int>& breakpoints, int deletept){
  if(!breakpoints.contains(deletept)){
cout<<"no such breakpoint"<<endl;
}else{
breakpoints.erase(deletept);
cout<<"breakpoint removed"<<endl;
}
}
void clearbp(map<int, int>& breakpoints){
  breakpoints.clear();
  cout<<"all breakpoints removed\n";
}
void listbp(map<int, int>& breakpoints){
  if(breakpoints.size()==0){
    cout<<"breakpoints: none"<<endl;
    return;
  }
  bool first=true;
  cout<<"breakpoints:";
  for(const std::pair<const int, int>& i: breakpoints){
    if(!first) cout<<",";
    cout<<i.first;
    first=false;
  }
  cout<<endl;
}
void where(STMT* cur){
  if(cur==nullptr){
    cout<<"execution has completed"<<endl;
    return;
  }
 struct PG_LINKS placeholder=break_pg(cur);
  programgraph_print(cur);
  repair_pg(cur, placeholder);
}



int step(STMT*& cur, RAM* memory, map<int, int>& breakpoints){
  if(cur==nullptr){
    cout<<"execution has completed"<<endl;
    return 1;
  }
  //breakpoint stuff
  if(breakpoints.contains(cur->line)){
    if(breakpoints[cur->line]==0){
      cout<<"breakpoint hit at line "<<cur->line<<endl;
      breakpoints[cur->line]=1;
      return 1;
    }
  }
  struct PG_LINKS placeholder=break_pg(cur);
  if(cur->stmt_type==STMT_IF_THEN_ELSE){
    struct RAM_VALUE* result=execute_expr(cur, memory, cur->types.if_then_else->condition);
    repair_pg(cur, placeholder);
    if(result==nullptr)return 1;
    if(result->types.i==0){
      cur=placeholder.false_path;
    }else{
      cur=placeholder.true_path;
    }
    ram_free_value(result);
  }else if(cur->stmt_type==STMT_WHILE_LOOP){
    struct RAM_VALUE* result=execute_expr(cur, memory, cur->types.while_loop->condition);
    repair_pg(cur, placeholder);
    if(result==nullptr)return 1;
    if(result->types.i==0){
      cur=placeholder.next;
    }else{
      cur=placeholder.loop_body;
    }
    ram_free_value(result);
  }else{
    struct ExecuteResult result=execute(cur, memory);
    repair_pg(cur, placeholder);
    if(result.Success){
      cur=placeholder.next;
    }
    else return 1;
  }
  if(cur==nullptr){
    cout<<"program completed"<<endl;
    return 1;
  }
  return 0;
}
void run(STMT*& cur, RAM* memory, map<int, int>& breakpoints){
  if(cur==nullptr){
    cout<<"execution has completed"<<endl;
    return;
  }
  int cont=0;
  while(cont==0) cont=step(cur, memory, breakpoints);
}

int main(int argc, char* argv[])
{
  map<int, int> breakpoints;
  FILE* input = nullptr;

  cout << "** Simple gdb for nuPython **" << endl;

  if (argc < 2) {
    //
    // missing filename:
    //
    cout << "usage: ./a.out filename.py" << endl;
    return 0;
  }

  //
  // assume 2nd arg is a nuPython file, try to open:
  //
  char* filename = argv[1];

  input = fopen(filename, "r");

  if (input == nullptr) // unable to open:
  {
    cout << "**ERROR: unable to open input file "
         << "'" << filename << "' for input."
         << endl;

    return 0;
  }
 struct TokenQueue* tokens = parser_parse(input);

if (tokens == NULL)
{
printf("**parsing failed...\n");
fclose(input);
return 0;
}

STMT* program = programgraph_build(tokens);
STMT* cur = program;


RAM* memory = ram_init();

  // command loop:
  //
  string cmd;

  while (true) {

    cout << endl;
    cout << "Enter a command, h for help >" << endl;
    cin >> cmd;

    if (cmd == "q") {
      //
      // quit:
      //
      break;
    }
    else if (cmd == "h") { 
      help_command();
    }
    else if (cmd == "pg") 
    {
      programgraph_print(program);
    }
    else if (cmd == "sm") 
    {
      ram_print(memory);
    }
    else if (cmd == "b")
    {
      int key;
      cin>>key;
      breakpt(breakpoints, key); 
    } 
    else if (cmd == "rb")
    {
     int key;
        cin>>key;
        removebp(breakpoints, key); 
    } 
   else if (cmd == "cb")
    {
       clearbp(breakpoints); 
    } 
    else if (cmd == "lb")
    {
      listbp(breakpoints);
    } 
     else if (cmd == "w")
    {
      where(cur);
    } 
    else if (cmd == "s")
    {
       string b;
      std::getline(cin, b);
      int i=step(cur, memory, breakpoints);
    } 
    else if(cmd=="r"){
       string b;
      std::getline(cin, b);
      run(cur, memory, breakpoints);
    }
    else {
      cout << "unknown command" << endl;
    }

  }//while

  //
  // done: free data structures, close file, and return:
  //
  cout << endl;
  cout << "** Done **" << endl;

  programgraph_destroy(program);
  tokenqueue_destroy(tokens);
  fclose(input);
  ram_destroy(memory);
  return 0;
}
