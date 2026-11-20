/*pgutil.cpp*/

/**
  * @brief program graph utility functions
  *
  * @note SOLUTION
  *
  * @author original template by Prof. Joe Hummel
  * @author ?
  * @author ?
  * @author ?  
  * @note Northwestern University
  */

#include <iostream>

using std::cout;
using std::endl;

#include "pgutil.h"


/**
 * @brief breaks the program graph at the given stmt
 *
 * Breaks the links between the cur statement and the one 
 * that follows; in the case of while loops and if stmts,
 * there are multiple links that must be broken. Returns 
 * a struct that contains copies of all the broken links.
 *
 * NOTE: not all the links in the struct will be valid. For
 * example, if cur denotes a while loop, then only struct.next
 * and struct.loop_body will be valid --- ignore the other
 * links.
 *
 * @param cur denotes stmt where to break program graph
 * @return struct PG_LINKS that contains saved copies of broken links
 */
struct PG_LINKS break_pg(struct STMT* cur)
{
  struct PG_LINKS links;

  links.next = nullptr;
  links.loop_body = nullptr;
  links.true_path = nullptr;
  links.false_path = nullptr;

  switch(cur->stmt_type) {
    case 0:
      links.next=cur->types.assignment->next_stmt;
      cur->types.assignment->next_stmt=nullptr;
      break;
    case 1:
      links.next=cur->types.function_call->next_stmt;
      cur->types.function_call->next_stmt=nullptr;
      break;
    case 2:
      links.true_path=cur->types.if_then_else->true_path;
      links.false_path=cur->types.if_then_else->false_path;
      links.next=cur->types.if_then_else->next_stmt;
      cur->types.if_then_else->next_stmt=nullptr;
      cur->types.if_then_else->false_path=nullptr;
      cur->types.if_then_else->true_path=nullptr;
      break;
  
    case 3:
      links.next=cur->types.while_loop->next_stmt;
      links.loop_body=cur->types.while_loop->loop_body;
      cur->types.while_loop->next_stmt=nullptr;
      cur->types.while_loop->loop_body=nullptr;
      break;
    case 4:
    
      links.next=cur->types.pass->next_stmt;
      cur->types.pass->next_stmt=nullptr;
      break;
  }
  //
  // TODO:
  //
  
  return links;
}



/**
 * @brief repairs the program graph at the given stmt
 *
 * This function is the counterpart to break_pg by repairing
 * the program graph using the saved links in the given 
 * struct. This function only works if it is called to repair
 * the program graph at the SAME STATEMENT where it was 
 * broken. In other words, cur in this case must denote the 
 * SAME STMT cur in the call to break_pg.
 *
 * NOTE: not all the links in the struct are valid. For
 * example, if cur denotes a while loop, then only struct.next
 * and struct.loop_body will be valid --- ignore the other
 * links.
 *
 * @param cur denotes stmt where to repair program graph
 * @param struct PG_LINKS containing saved copies of broken links
 */
void repair_pg(struct STMT* cur, struct PG_LINKS links)
{
   switch(cur->stmt_type) {
    case 0:
      cur->types.assignment->next_stmt=links.next;
      break;
    case 1:
      cur->types.function_call->next_stmt=links.next;
      break;
    case 2:
      cur->types.if_then_else->next_stmt=links.next;
      cur->types.if_then_else->false_path=links.false_path;
      cur->types.if_then_else->true_path=links.true_path;
      break;
  
    case 3:
      cur->types.while_loop->next_stmt=links.next;
      cur->types.while_loop->loop_body=links.loop_body;
      break;
    case 4:
      cur->types.pass->next_stmt=links.next;
      break;
  }
  //
  // TODO:
  //
  
  return;
}


#ifdef UNIT_TEST
//////////////////////////////////////////////////////////////////////////
// 
// UNIT TESTS for above functions:
//
//////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

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

TEST(break_repair_pg, assignment)
{
  cout << "**starting" << endl;

  FILE* file;
  char* filename = "__test.py";

  file = fopen(filename, "w");
  fprintf(file, "x = 0\n");
  fprintf(file, "y = 1\n");
  fclose(file);

  file = fopen(filename, "r");
  struct TokenQueue* tokens = parser_parse(file);
  struct STMT* program = programgraph_build(tokens);
  fclose(file);

  struct STMT* stmt2 = program->types.assignment->next_stmt;

  //
  // okay let's break PG at the first stmt:
  //
  cout << "**breaking..." << endl;

  struct PG_LINKS links = break_pg(program);

  ASSERT_EQ(program->types.assignment->next_stmt, nullptr);
  ASSERT_EQ(links.next, stmt2);

  //
  // now let's repair it:
  //
  cout << "**repairing..." << endl;

  repair_pg(program, links);

  ASSERT_EQ(program->types.assignment->next_stmt, stmt2);

  //
  // done!
  //
  tokenqueue_destroy(tokens);
  programgraph_destroy(program);

  cout << "**passed!" << endl;
}  

TEST(break_repair_pg, pass)
{
  cout << "**starting" << endl;

  FILE* file;
  char* filename = "__test.py";

  file = fopen(filename, "w");
  fprintf(file, "pass\n");
  fprintf(file, "y = 1\n");
  fclose(file);

  file = fopen(filename, "r");
  struct TokenQueue* tokens = parser_parse(file);
  struct STMT* program = programgraph_build(tokens);
  fclose(file);

  struct STMT* stmt2 = program->types.pass->next_stmt;

  //
  // okay let's break PG at the first stmt:
  //
  cout << "**breaking..." << endl;

  struct PG_LINKS links = break_pg(program);

  ASSERT_EQ(program->types.pass->next_stmt, nullptr);
  ASSERT_EQ(links.next, stmt2);

  //
  // now let's repair it:
  //
  cout << "**repairing..." << endl;

  repair_pg(program, links);

  ASSERT_EQ(program->types.pass->next_stmt, stmt2);

  //
  // done!
  //
  tokenqueue_destroy(tokens);
  programgraph_destroy(program);

  cout << "**passed!" << endl;
}  

//test that break works on function_call type
TEST(break_repair_pg, function_call) {
  cout << "**starting" << endl;

  FILE* file;
  char* filename = "__test.py";

  //writes python program in a new file
  file = fopen(filename, "w");
  fprintf(file, "print(\"hello guys what's up\")\n");
  fprintf(file, "y = 1\n");
  fclose(file);

  //has NUPython read from the newly created file
  file = fopen(filename, "r");
  struct TokenQueue* tokens = parser_parse(file);
  struct STMT* program = programgraph_build(tokens);
  fclose(file);
  
  //finds the next STMT that the function call points to
  struct STMT* stmt2 = program->types.function_call->next_stmt;

    // okay let's break PG at the first stmt:
  //
  cout << "**breaking..." << endl;

  //links is a struct that contains pointer to next stmt
  struct PG_LINKS links = break_pg(program);

  //should be nullptr after being broken
  ASSERT_EQ(program->types.function_call->next_stmt, nullptr);
  //links.next should hold the old next stmt
  ASSERT_EQ(links.next, stmt2);

  //
  // now let's repair it:
  //
  cout << "**repairing..." << endl;

  repair_pg(program, links);

  ASSERT_EQ(program->types.function_call->next_stmt, stmt2);

  //
  // done!
  //
  tokenqueue_destroy(tokens);
  programgraph_destroy(program);

  cout << "**passed!" << endl;
}



//test that break works on while_loop type
TEST(break_repair_pg, while_loop) {
  cout << "**starting" << endl;

  FILE* file;
  char* filename = "__test.py";

  //write a for loop in "python" to test
  /*
    i = 0
    while i < 5:
      i+=1
    y = 2
  */

  file = fopen(filename, "w");
  fprintf(file, "i = 0\n");
  fprintf(file, "while i < 5:\n");
  fprintf(file, "{\n");
  fprintf(file, "  i = i + 1\n");
  fprintf(file, "}\n");
  fprintf(file, "y = 2\n");
  fclose(file);

  //has NUPython read from the newly created file
  file = fopen(filename, "r");
  struct TokenQueue* tokens = parser_parse(file);
  struct STMT* program = programgraph_build(tokens);
  fclose(file);

  //get the stmt where while loop starts
  struct STMT* loop = program->types.assignment->next_stmt;
  
  //finds the next STMT that the function call points to
  struct STMT* next_stmt = loop->types.while_loop->next_stmt;
  struct STMT* loop_body = loop->types.while_loop->loop_body;

    // okay let's break PG at the first stmt:
  //
  cout << "**breaking..." << endl;

  //links is a struct that contains pointer to next stmt
  struct PG_LINKS links = break_pg(loop);

  //should be nullptr after being broken
  ASSERT_EQ(loop->types.while_loop->next_stmt, nullptr);
  ASSERT_EQ(loop->types.while_loop->loop_body, nullptr);
  //links.next should hold the old next stmt
  ASSERT_EQ(links.next, next_stmt);
  ASSERT_EQ(links.loop_body, loop_body);

  //
  // now let's repair it:
  //
  cout << "**repairing..." << endl;

  repair_pg(loop, links);

  //after repairing, both of the links should be back to normal
  ASSERT_EQ(loop->types.while_loop->next_stmt, next_stmt);
  ASSERT_EQ(loop->types.while_loop->loop_body, loop_body);

  //
  // done!
  //
  tokenqueue_destroy(tokens);
  programgraph_destroy(program);

  cout << "**passed!" << endl;
}

//test that break works on if_then_else
//test that break works on if/else
TEST(break_repair_pg, if_then_else) {
  cout << "**starting" << endl;

  FILE* file;
  char* filename = "__test.py";

  //writes nuPython program in a new file

  /*
  x = 3
  if x<5:
    y = 1
  else:
    y = 2
  z = 3
  */
  //input a program into new file
  file = fopen(filename, "w");
  fprintf(file, "x = 3\n");
  fprintf(file, "if x < 5:\n");
  fprintf(file, "{\n");
  fprintf(file, "  y = 1\n");
  fprintf(file, "}\n");
  fprintf(file, "else:\n");
  fprintf(file, "{\n");
  fprintf(file, "  y = 2\n");
  fprintf(file, "}\n");
  fprintf(file, "z = 3\n");
  fclose(file);

  //has nuPython read from the newly created file
  file = fopen(filename, "r");
  struct TokenQueue* tokens = parser_parse(file);
  struct STMT* program = programgraph_build(tokens);
  fclose(file);

  //skip past first stmt to go to the actual start of the if stmt
  struct STMT* ifstmt = program->types.assignment->next_stmt;
  ASSERT_EQ(ifstmt->stmt_type, STMT_IF_THEN_ELSE);

  //if then else has 3 paths to keep track of
  struct STMT* true_path  = ifstmt->types.if_then_else->true_path;
  struct STMT* false_path = ifstmt->types.if_then_else->false_path;
  struct STMT* next_stmt  = ifstmt->types.if_then_else->next_stmt;

  cout << "**breaking..." << endl;

  // storing the broken pointers to STMTs here
  struct PG_LINKS links = break_pg(ifstmt);

  //should be nullptr after being broken now
  ASSERT_EQ(ifstmt->types.if_then_else->true_path, nullptr);
  ASSERT_EQ(ifstmt->types.if_then_else->false_path, nullptr);
  ASSERT_EQ(ifstmt->types.if_then_else->next_stmt, nullptr);
  //links structure wshould hold the prvious links
  ASSERT_EQ(links.true_path, true_path);
  ASSERT_EQ(links.false_path, false_path);
  ASSERT_EQ(links.next, next_stmt);

  //x = 3 should still point to the if/else
  ASSERT_EQ(program->types.assignment->next_stmt, ifstmt);

  // now let's repair it:
  cout << "**repairing..." << endl;

  repair_pg(ifstmt, links);


  ASSERT_EQ(ifstmt->types.if_then_else->true_path, true_path);
  ASSERT_EQ(ifstmt->types.if_then_else->false_path, false_path);
  ASSERT_EQ(ifstmt->types.if_then_else->next_stmt, next_stmt);

  tokenqueue_destroy(tokens);
  programgraph_destroy(program);

  cout << "**passed!" << endl;
}


//////////////////////////////////////////////////////////////////////////
// 
// END OF UNIT TESTS
//
//////////////////////////////////////////////////////////////////////////
#endif
