//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully and add code to build an LLVM program
//**************************************************************

#define EXTERN
#include "cgen.h"
#include <sstream>
#include <string>
#include <limits>

//
extern int cgen_debug;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.  Feel free to add your
// own definitions as you see fit.
//
//////////////////////////////////////////////////////////////////////
EXTERN Symbol
    // required classes
    Object,
    IO, String, Int, Bool, Main,

    // class methods
    cool_abort, type_name, cool_copy, out_string, out_int, in_string, in_int,
    length, concat, substr,

    // class members
    val,

    // special symbols
    No_class,  // symbol that can't be the name of any user-defined class
    No_type,   // If e : No_type, then no code is generated for e.
    SELF_TYPE, // Special code is generated for new SELF_TYPE.
    self,      // self generates code differently than other references

    // extras
    arg, arg2, prim_string, prim_int, prim_bool;

//********************************************************
//
// PREDEFINED FUNCTIONS:
//
// The following functions are already coded, you should
// not need to modify them, although you may if necessary.
//
//********************************************************

//
// Initializing the predefined symbols.
//
static void initialize_constants(void) {
  Object = idtable.add_string("Object");
  IO = idtable.add_string("IO");
  String = idtable.add_string("String");
  Int = idtable.add_string("Int");
  Bool = idtable.add_string("Bool");
  Main = idtable.add_string("Main");

  cool_abort = idtable.add_string("abort");
  type_name = idtable.add_string("type_name");
  cool_copy = idtable.add_string("copy");
  out_string = idtable.add_string("out_string");
  out_int = idtable.add_string("out_int");
  in_string = idtable.add_string("in_string");
  in_int = idtable.add_string("in_int");
  length = idtable.add_string("length");
  concat = idtable.add_string("concat");
  substr = idtable.add_string("substr");

  val = idtable.add_string("val");

  No_class = idtable.add_string("_no_class");
  No_type = idtable.add_string("_no_type");
  SELF_TYPE = idtable.add_string("SELF_TYPE");
  self = idtable.add_string("self");

  arg = idtable.add_string("arg");
  arg2 = idtable.add_string("arg2");
  prim_string = idtable.add_string("sbyte*");
  prim_int = idtable.add_string("int");
  prim_bool = idtable.add_string("bool");
}

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************
void program_class::cgen(ostream &os) {
  initialize_constants();
  class_table = new CgenClassTable(classes, os);
}

// Create definitions for all String constants
void StrTable::code_string_table(ostream &s, CgenClassTable *ct) {
  for (auto entry : tbl) {
    entry->code_def(s, ct);
  }
}

// Create definitions for all Int constants
void IntTable::code_string_table(ostream &s, CgenClassTable *ct) {
  for (auto entry : tbl) {
    entry->code_def(s, ct);
  }
}

//
// Sets up declarations for extra functions needed for code generation
// You should not need to modify this code for MP2.1
//
void CgenClassTable::setup_external_functions() {
  ValuePrinter vp;
  // setup function: external int strcmp(sbyte*, sbyte*)
  op_type i32_type(INT32), i8ptr_type(INT8_PTR), vararg_type(VAR_ARG);
  vector<op_type> strcmp_args;
  strcmp_args.push_back(i8ptr_type);
  strcmp_args.push_back(i8ptr_type);
  vp.declare(*ct_stream, i32_type, "strcmp", strcmp_args);

  // setup function: external int printf(sbyte*, ...)
  vector<op_type> printf_args;
  printf_args.push_back(i8ptr_type);
  printf_args.push_back(vararg_type);
  vp.declare(*ct_stream, i32_type, "printf", printf_args);

  // setup function: external void abort(void)
  op_type void_type(VOID);
  vector<op_type> abort_args;
  vp.declare(*ct_stream, void_type, "abort", abort_args);

  // setup function: external i8* malloc(i32)
  vector<op_type> malloc_args;
  malloc_args.push_back(i32_type);
  vp.declare(*ct_stream, i8ptr_type, "malloc", malloc_args);

#ifdef MP3
  // ADD CODE HERE
  // Setup external functions for built in object class functions
#endif
}

// Creates AST nodes for the basic classes and installs them in the class list
void CgenClassTable::install_basic_classes() {
  // The tree package uses these globals to annotate the classes built below.
  curr_lineno = 0;
  Symbol filename = stringtable.add_string("<basic class>");

  //
  // A few special class names are installed in the lookup table but not
  // the class list. Thus, these classes exist, but are not part of the
  // inheritance hierarchy.

  // No_class serves as the parent of Object and the other special classes.
  Class_ noclasscls = class_(No_class, No_class, nil_Features(), filename);
  install_special_class(new CgenNode(noclasscls, CgenNode::Basic, this));
  delete noclasscls;

#ifdef MP3
  // SELF_TYPE is the self class; it cannot be redefined or inherited.
  Class_ selftypecls = class_(SELF_TYPE, No_class, nil_Features(), filename);
  install_special_class(new CgenNode(selftypecls, CgenNode::Basic, this));
  delete selftypecls;
  //
  // Primitive types masquerading as classes. This is done so we can
  // get the necessary Symbols for the innards of String, Int, and Bool
  //
  Class_ primstringcls =
      class_(prim_string, No_class, nil_Features(), filename);
  install_special_class(new CgenNode(primstringcls, CgenNode::Basic, this));
  delete primstringcls;
#endif
  Class_ primintcls = class_(prim_int, No_class, nil_Features(), filename);
  install_special_class(new CgenNode(primintcls, CgenNode::Basic, this));
  delete primintcls;
  Class_ primboolcls = class_(prim_bool, No_class, nil_Features(), filename);
  install_special_class(new CgenNode(primboolcls, CgenNode::Basic, this));
  delete primboolcls;
  //
  // The Object class has no parent class. Its methods are
  //        cool_abort() : Object   aborts the program
  //        type_name() : Str       returns a string representation of class
  //        name copy() : SELF_TYPE      returns a copy of the object
  //
  // There is no need for method bodies in the basic classes---these
  // are already built in to the runtime system.
  //
  Class_ objcls = class_(
      Object, No_class,
      append_Features(
          append_Features(single_Features(method(cool_abort, nil_Formals(),
                                                 Object, no_expr())),
                          single_Features(method(type_name, nil_Formals(),
                                                 String, no_expr()))),
          single_Features(
              method(cool_copy, nil_Formals(), SELF_TYPE, no_expr()))),
      filename);
  install_class(new CgenNode(objcls, CgenNode::Basic, this));
  delete objcls;

  //
  // The Int class has no methods and only a single attribute, the
  // "val" for the integer.
  //
  Class_ intcls = class_(
      Int, Object, single_Features(attr(val, prim_int, no_expr())), filename);
  install_class(new CgenNode(intcls, CgenNode::Basic, this));
  delete intcls;

  //
  // Bool also has only the "val" slot.
  //
  Class_ boolcls = class_(
      Bool, Object, single_Features(attr(val, prim_bool, no_expr())), filename);
  install_class(new CgenNode(boolcls, CgenNode::Basic, this));
  delete boolcls;

#ifdef MP3
  //
  // The class String has a number of slots and operations:
  //       val                                  the string itself
  //       length() : Int                       length of the string
  //       concat(arg: Str) : Str               string concatenation
  //       substr(arg: Int, arg2: Int): Str     substring
  //
  Class_ stringcls =
      class_(String, Object,
             append_Features(
                 append_Features(
                     append_Features(
                         single_Features(attr(val, prim_string, no_expr())),
                         single_Features(
                             method(length, nil_Formals(), Int, no_expr()))),
                     single_Features(method(concat,
                                            single_Formals(formal(arg, String)),
                                            String, no_expr()))),
                 single_Features(
                     method(substr,
                            append_Formals(single_Formals(formal(arg, Int)),
                                           single_Formals(formal(arg2, Int))),
                            String, no_expr()))),
             filename);
  install_class(new CgenNode(stringcls, CgenNode::Basic, this));
  delete stringcls;
#endif

#ifdef MP3
  //
  // The IO class inherits from Object. Its methods are
  //        out_string(Str) : SELF_TYPE          writes a string to the output
  //        out_int(Int) : SELF_TYPE               "    an int    "  "     "
  //        in_string() : Str                    reads a string from the input
  //        in_int() : Int                         "   an int     "  "     "
  //
  Class_ iocls = class_(
      IO, Object,
      append_Features(
          append_Features(
              append_Features(
                  single_Features(method(out_string,
                                         single_Formals(formal(arg, String)),
                                         SELF_TYPE, no_expr())),
                  single_Features(method(out_int,
                                         single_Formals(formal(arg, Int)),
                                         SELF_TYPE, no_expr()))),
              single_Features(
                  method(in_string, nil_Formals(), String, no_expr()))),
          single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
      filename);
  install_class(new CgenNode(iocls, CgenNode::Basic, this));
  delete iocls;
#endif
}

//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_classes(Classes cs) {
  for (auto cls : cs) {
    install_class(new CgenNode(cls, CgenNode::NotBasic, this));
  }
}

//
// Add this CgenNode to the class list and the lookup table
//
void CgenClassTable::install_class(CgenNode *nd) {
  Symbol name = nd->get_name();

  if (probe(name))
    return;

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd, nds);
  addid(name, nd);
}

//
// Add this CgenNode to the special class list and the lookup table
//
void CgenClassTable::install_special_class(CgenNode *nd) {
  Symbol name = nd->get_name();

  if (probe(name))
    return;

  // The class name is legal, so add it to the list of special classes
  // and the symbol table.
  special_nds = new List<CgenNode>(nd, special_nds);
  addid(name, nd);
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree() {
  for (auto node : nds)
    set_relations(node);
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNode *nd) {
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

// Get the root of the class tree.
CgenNode *CgenClassTable::root() { return probe(Object); }

//////////////////////////////////////////////////////////////////////
//
// Special-case functions used for the method Int Main::main() for
// MP2 only.
//
//////////////////////////////////////////////////////////////////////

#ifndef MP3

CgenNode *CgenClassTable::getMainmain(CgenNode *c) {
  if (c && !c->basic())
    return c; // Found it!

  for (auto child : c->get_children()) {
    if (CgenNode *foundMain = this->getMainmain(child))
      return foundMain; // Propagate it up the recursive calls
  }

  return 0; // Make the recursion continue
}

#endif

//-------------------------------------------------------------------
//
// END OF PREDEFINED FUNCTIONS
//
//-------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// coding string, int, and boolean constants
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type stringEntry.  stringEntry methods are defined both for string
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Create global definitions for constant Cool objects
//
void CgenClassTable::code_constants() {
#ifdef MP3

  // ADD CODE HERE
#endif
}

// generate code to define a global string constant
void StringEntry::code_def(ostream &s, CgenClassTable *ct) {
#ifdef MP3
  // ADD CODE HERE
#endif
}

// generate code to define a global int constant
void IntEntry::code_def(ostream &s, CgenClassTable *ct) {
  // Leave this method blank, since we are not going to use global
  // declarations for int constants.
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//
// CgenClassTable constructor orchestrates all code generation
//
CgenClassTable::CgenClassTable(Classes classes, ostream &s) : nds(0) {
  if (cgen_debug)
    std::cerr << "Building CgenClassTable" << endl;
  ct_stream = &s;
  // Make sure we have a scope, both for classes and for constants
  enterscope();

  // Create an inheritance tree with one CgenNode per class.
  install_basic_classes();
  install_classes(classes);
  build_inheritance_tree();

  // First pass
  setup();

  // Second pass
  code_module();
  // Done with code generation: exit scopes
  exitscope();
}

CgenClassTable::~CgenClassTable() {}

// The code generation first pass.  Define these two functions to traverse
// the tree and setup each CgenNode
void CgenClassTable::setup() {
  setup_external_functions();
  setup_classes(root(), 0);
}

void CgenClassTable::setup_classes(CgenNode *c, int depth) {
  // MAY ADD CODE HERE
  // if you want to give classes more setup information

  c->setup(current_tag++, depth);
  for (auto child : c->get_children())
    setup_classes(child, depth + 1);

  c->set_max_child(current_tag - 1);

  /*
  if (cgen_debug)
          std::cerr << "Class " << c->get_name() << " assigned tag "
                  << c->get_tag() << ", max child " << c->get_max_child()
                  << ", depth " << c->get_depth() << endl;
  */
}

// The code generation second pass. Add code here to traverse the tree and
// emit code for each CgenNode
void CgenClassTable::code_module() {
  code_constants();

#ifndef MP3
  // This must be after code_module() since that emits constants
  // needed by the code() method for expressions
  CgenNode *mainNode = getMainmain(root());
  mainNode->codeGenMainmain();
#endif
  code_main();

#ifdef MP3
  code_classes(root());
#else
#endif
}

#ifdef MP3
void CgenClassTable::code_classes(CgenNode *c) {

  // ADD CODE HERE
}
#endif

//
// Create LLVM entry point. This function will initiate our Cool program
// by generating the code to execute (new Main).main()
//
void CgenClassTable::code_main(){
// Define a function main that has no parameters and returns an i32

// Define an entry basic block

// Call Main_main(). This returns int* for phase 1, Object for phase 2

#ifndef MP3
// Get the address of the string "Main_main() returned %d\n" using
// getelementptr

// Call printf with the string address of "Main_main() returned %d\n"
// and the return value of Main_main() as its arguments

// Insert return 0

#else
// MP 3
#endif

}

///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTable *ct)
    : class__class((const class__class &)*nd), parentnd(0), children(0),
      basic_status(bstatus), class_table(ct), tag(-1) {
  // ADD CODE HERE
}

void CgenNode::add_child(CgenNode *n) {
  children = new List<CgenNode>(n, children);
}

void CgenNode::set_parentnd(CgenNode *p) {
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}

//
// Class setup.  You may need to add parameters to this function so that
// the classtable can provide setup information (such as the class tag
// that should be used by this class).
//
// Things that setup should do:
//  - layout the features of the class
//  - create the types for the class and its vtable
//  - create global definitions used by the class such as the class vtable
//
void CgenNode::setup(int tag, int depth) {
  this->tag = tag;
#ifdef MP3
  layout_features();

  // ADD CODE HERE

#endif
}

#ifdef MP3
//
// Class codegen. This should performed after every class has been setup.
// Generate code for each method of the class.
//
void CgenNode::code_class() {
  // No code generation for basic classes. The runtime will handle that.
  if (basic())
    return;

  // ADD CODE HERE
}

// Laying out the features involves creating a Function for each method
// and assigning each attribute a slot in the class structure.
void CgenNode::layout_features() {
  // ADD CODE HERE
}
#else

//
// code-gen function main() in class Main
//
void CgenNode::codeGenMainmain() {
  ValuePrinter vp;
  // In Phase 1, this can only be class Main. Get method_class for main().
  assert(std::string(this->name->get_string()) == std::string("Main"));
  method_class *mainMethod = (method_class *)features->nth(features->first());

  // ADD CODE HERE TO GENERATE THE FUNCTION int Mainmain().
  // Generally what you need to do are:
  // -- setup or create the environment, env, for translating this method
  // -- invoke mainMethod->code(env) to translate the method
}

#endif

//
// CgenEnvironment functions
//

//
// Class CgenEnvironment should be constructed by a class prior to code
// generation for each method.  You may need to add parameters to this
// constructor.
//
CgenEnvironment::CgenEnvironment(std::ostream &o, CgenNode *c) {
  cur_class = c;
  cur_stream = &o;
  var_table.enterscope();
  tmp_count = block_count = ok_count = 0;
  // ADD CODE HERE
}

// Look up a CgenNode given a symbol
CgenNode *CgenEnvironment::type_to_class(Symbol t) {
  return t == SELF_TYPE ? get_class()
                        : get_class()->get_classtable()->lookup(t);
}

// Provided CgenEnvironment methods
// Generate unique string names
std::string CgenEnvironment::new_name() {
  std::stringstream s;
  s << tmp_count++;
  return "tmp." + s.str();
}

std::string CgenEnvironment::new_ok_label() {
  std::stringstream s;
  s << ok_count++;
  return "ok." + s.str();
}
const std::string CgenEnvironment::new_label(const std::string &prefix,
                                             bool increment) {
  std::string suffix = itos(block_count);
  block_count += increment;
  return prefix + suffix;
}

void CgenEnvironment::add_local(Symbol name, operand &vb) {
  var_table.enterscope();
  var_table.addid(name, &vb);
}

void CgenEnvironment::kill_local() { var_table.exitscope(); }

////////////////////////////////////////////////////////////////////////////
//
// APS class methods
//
////////////////////////////////////////////////////////////////////////////

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.handcode.h'.
//
//*****************************************************************

#ifdef MP3
// conform and get_class_tag are only needed for MP3

// conform - If necessary, emit a bitcast or boxing/unboxing operations
// to convert an object to a new type. This can assume the object
// is known to be (dynamically) compatible with the target type.
// It should only be called when this condition holds.
// (It's needed by the supplied code for typecase)
operand conform(operand src, op_type type, CgenEnvironment *env) {
  // ADD CODE HERE (MP3 ONLY)
  return operand();
}

// Retrieve the class tag from an object record.
// src is the object we need the tag from.
// src_class is the CgenNode for the *static* class of the expression.
// You need to look up and return the class tag for it's dynamic value
operand get_class_tag(operand src, CgenNode *src_cls, CgenEnvironment *env) {
  // ADD CODE HERE (MP3 ONLY)
  return operand();
}
#endif

//
// Create a method body
//
void method_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "method" << endl;

  // ADD CODE HERE
  ValuePrinter vp(*env->cur_stream);

  // make alloca for all exp
  expr->make_alloca(env);

  // generate code and get the ret val
  operand res_Main_main = expr->code(env);

  // ret
  vp.ret(*env->cur_stream, res_Main_main);
}

//
// Codegen for expressions.  Note that each expression has a value.
//

operand assign_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "assign" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand assign_rhs = expr->code(env);

  operand *assign_lhs = env->lookup(name);
  
  if (assign_lhs == nullptr) {
    std::cerr << "Error: Variable " << name << " not found in scope." << endl;
    return assign_rhs;
  }
  
  // after getting the location assign_lhs 
  vp.store(*env->cur_stream, assign_rhs, *assign_lhs);

  return assign_rhs;
  
}

operand cond_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "cond" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  
  auto check_ = pred->code(env);
  auto true_ = env->new_true_label();
  auto false_ = env->new_false_label();
  auto end_ = env->new_end_label();  

  vp.branch_cond(check_, true_, false_);

  vp.begin_block(true_);
  auto then_ = then_exp->code(env);

  operand cond_res; 
  operand res_ptr = vp.alloca_mem(then_.get_type()); 
  op_type result_type = then_.get_type();

  vp.store(*env->cur_stream, then_, res_ptr);
  vp.branch_uncond(end_);

  vp.begin_block(false_);
  auto else_ = else_exp->code(env);
  vp.store(*env->cur_stream, else_, res_ptr);
  vp.branch_uncond(end_);

  vp.begin_block(end_);
  cond_res = vp.load(result_type, res_ptr);

  return cond_res;
}

operand loop_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "loop" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);

  auto loop_label = env->new_loop_label();
  auto true_label = env->new_true_label();
  auto false_label = env->new_false_label();
  
  vp.branch_uncond(loop_label);
  
  vp.begin_block(loop_label);
  auto pred_op = pred->code(env);
  vp.branch_cond(*env->cur_stream, pred_op, true_label, false_label);

  vp.begin_block(true_label);
  body->code(env);
  vp.branch_uncond(loop_label);

  vp.begin_block(false_label);
  
  int_value i32_0(0);

  return i32_0;

}

operand block_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "block" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  int i = 0;
  for(i = body->first(); body->more(i) && body->more(i+1); i = body->next(i)) {
    auto expr_iter = body->nth(i);
    expr_iter->code(env);
  }

  auto expr_iter = body->nth(i);
  operand block_res = expr_iter->code(env);

  return block_res;
}

operand let_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "let" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);

  op_type id_type = op_type(INT32);
  operand id_op = vp.alloca_mem(id_type);

  int_value i32_0(0);
  bool_value i1_false(false, true);

  operand init_op = init->code(env);

  // store value
  if (init_op.is_empty()) 
  { 
    if (id_type.get_id() == INT32 && id_type.get_name() == "i32") {
      vp.store(*env->cur_stream, i32_0, id_op);
    } else if (id_type.get_id() == INT1 && id_type.get_name() == "i1") {
      vp.store(*env->cur_stream, i1_false, id_op);
    }
  } 
  else 
  { 
    vp.store(*env->cur_stream, init_op, id_op);
  }

  Symbol identifier = this->identifier;   
  env->add_binding(identifier, &id_op);

  env->open_scope();  
  operand let_res = body->code(env);  
  env->close_scope();

  return let_res;
}

operand plus_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "plus" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand op1_ = e1->code(env);
  operand op2_ = e2->code(env);
  operand plus_res = vp.add(op1_, op2_);

  return plus_res;
}

operand sub_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "sub" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand op1_ = e1->code(env);
  operand op2_ = e2->code(env);
  operand sub_res = vp.sub(op1_, op2_);

  return sub_res;
}

operand mul_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "mul" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand op1_ = e1->code(env);
  operand op2_ = e2->code(env);
  operand mul_res = vp.mul(op1_, op2_);

  return mul_res;
}

operand divide_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "div" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand numerator_ = e1->code(env);
  operand demoninator_ = e2->code(env);
  int_value zero(0);
  operand test_zero = vp.icmp(EQ, demoninator_, zero);

  label abort_true = "abort";
  label ok_false = env->new_ok_label();

  vp.branch_cond(*env->cur_stream, test_zero, abort_true, ok_false);

  vp.begin_block(ok_false);
  operand div_res = vp.div(numerator_, demoninator_);

  return div_res;
}

operand neg_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "neg" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  int_value op1_(0);
  operand op2_ = e1->code(env);
  operand neg_res = vp.sub(op1_, op2_);

  return neg_res;
}

operand lt_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "lt" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand op1_ = e1->code(env);
  operand op2_ = e2->code(env);
  operand lt_res = vp.icmp(LT, op1_, op2_);

  return lt_res;
}

operand eq_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "eq" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();
  ValuePrinter vp(*env->cur_stream);
  operand op1_ = e1->code(env);
  operand op2_ = e2->code(env);
  operand eq_res = vp.icmp(EQ, op1_, op2_);

  return eq_res;
}

operand leq_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "leq" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand op1_ = e1->code(env);
  operand op2_ = e2->code(env);
  operand leq_res = vp.icmp(LE, op1_, op2_);

  return leq_res;
}

operand comp_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "complement" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand op1_ = e1->code(env);
  bool_value op2_(true, true);
  operand comp_res = vp.xor_in(op1_, op2_);

  return comp_res;
}

operand int_const_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "Integer Constant" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  std::string int_val = token->get_string();
  const char *string_to_char = int_val.c_str();

  int real_val = std::atoi(string_to_char);
  int val_constraints = std::max(std::numeric_limits<int>::min(), std::min(real_val, std::numeric_limits<int>::max()));
  int_value res(val_constraints);

  return res;
}

operand bool_const_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "Boolean Constant" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  if (val) {
    return bool_value(true, true);
  } 
  
  return bool_value(false, true);
}

operand object_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "Object" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // return operand();

  ValuePrinter vp(*env->cur_stream);
  operand *op = env->lookup(name);
  op_type tp_ = op->get_type().get_deref_type();
  operand object_res = vp.load(tp_, *op);

  return object_res;
}

operand no_expr_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "No_expr" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  return operand();
}

//*****************************************************************
// The next few code() functions are for node types not supported
// in Phase 1 but these functions must be defined because they are
// declared as methods via the Expression_SHARED_EXTRAS hack.
//*****************************************************************

operand static_dispatch_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "static dispatch" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
    // MORE MEANINGFUL
#endif
  return operand();
}

operand string_const_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "string_const" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
    // MORE MEANINGFUL
#endif
  return operand();
}

operand dispatch_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "dispatch" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
    // MORE MEANINGFUL
#endif
  return operand();
}

operand typcase_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "typecase::code()" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
    // MORE MEANINGFUL
#endif
  return operand();
}

operand new__class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "newClass" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
    // MORE MEANINGFUL
#endif
  return operand();
}

operand isvoid_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "isvoid" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
    // MORE MEANINGFUL
#endif
  return operand();
}

// Create the LLVM Function corresponding to this method.
void method_class::layout_feature(CgenNode *cls) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE
#endif
}

// If the source tag is >= the branch tag and <= (max child of the branch class)
// tag, then the branch is a superclass of the source
operand branch_class::code(operand expr_val, operand tag, op_type join_type,
                           CgenEnvironment *env) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
#endif
  return operand();
}

// Assign this attribute a slot in the class structure
void attr_class::layout_feature(CgenNode *cls) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE
#endif
}

void attr_class::code(CgenEnvironment *env) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE
#endif
}

//*****************************************************************
// Implementations of make_alloca which create the necessary alloca
// for an expression
//*****************************************************************
void assign_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "assign" << endl;
  
  // ADD ANY CODE HERE
  expr->make_alloca(env);
  set_expr_type(env, expr->get_expr_type(env));
}

void cond_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "cond" << endl;

  // ADD ANY CODE HERE

  ValuePrinter vp(*env->cur_stream);

  pred->make_alloca(env);
  then_exp->make_alloca(env);
  else_exp->make_alloca(env);

  result_type = then_exp->get_expr_type(env);
  set_expr_type(env, result_type);

  // make alloca 
  res_ptr = vp.alloca_mem(result_type);
}

void loop_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "loop" << endl;

  // ADD ANY CODE HERE
  ValuePrinter vp(*env->cur_stream);

  op_type int_(INT32);
  set_expr_type(env, int_);

  pred->make_alloca(env);
  body->make_alloca(env);
}

void block_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "block" << endl;

  // ADD ANY CODE HERE
  int i = 0;
  for(i = body->first(); body->more(i) && body->more(i+1); i = body->next(i)) {
    auto expr_iter = body->nth(i);
    expr_iter->make_alloca(env);
  }

  auto expr_iter = body->nth(i);
  expr_iter->make_alloca(env);

  set_expr_type(env, expr_iter->get_expr_type(env));
}

void let_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "let" << endl;

  // ADD ANY CODE HERE
  init->make_alloca(env);

  ValuePrinter vp(*env->cur_stream);
  std::string type_name = type_decl->get_string();

  if (type_name == "Int") {
    op_type i32_type(INT32);
    operand alloc_int = vp.alloca_mem(i32_type);
    id_type = i32_type;
    id_op = alloc_int;
    env->var_tp_add_binding(identifier, &i32_type); 
  } else if (type_name == "Bool") {
    op_type i1_type(INT1);
    operand alloc_bool = vp.alloca_mem(i1_type);
    id_type = i1_type;
    id_op = alloc_bool;
    env->var_tp_add_binding(identifier, &i1_type);
  }

  env->var_tp_open_scope();
  body->make_alloca(env);
  set_expr_type(env, body->get_expr_type(env));
  env->var_tp_close_scope();
}

void plus_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "plus" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  e2->make_alloca(env);

  set_expr_type(env, e1->get_expr_type(env));
}

void sub_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "sub" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  e2->make_alloca(env);

  set_expr_type(env, e1->get_expr_type(env));
}

void mul_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "mul" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  e2->make_alloca(env);

  set_expr_type(env, e1->get_expr_type(env));
}

void divide_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "div" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  e2->make_alloca(env);

  set_expr_type(env, e1->get_expr_type(env));
}

void neg_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "neg" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  set_expr_type(env, e1->get_expr_type(env));
}

void lt_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "lt" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  e2->make_alloca(env);

  op_type bool_(INT1);
  set_expr_type(env, bool_);
}

void eq_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "eq" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  e2->make_alloca(env);

  op_type bool_(INT1);
  set_expr_type(env, bool_);
}

void leq_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "leq" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  e2->make_alloca(env);

  op_type bool_(INT1);
  set_expr_type(env, bool_);
}

void comp_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "complement" << endl;

  // ADD ANY CODE HERE
  e1->make_alloca(env);
  set_expr_type(env, e1->get_expr_type(env));
}

void int_const_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "Integer Constant" << endl;
  
  // ADD ANY CODE HERE
  op_type int_(INT32);
  set_expr_type(env, int_);
}

void bool_const_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "Boolean Constant" << endl;
  
  // ADD ANY CODE HERE
  op_type bool_(INT1);
  set_expr_type(env, bool_);
}

void object_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "Object" << endl;
  
  // ADD ANY CODE HERE
  operand* op = env->lookup(name);
  op_type tp = op->get_type();

  set_expr_type(env, tp);
}

void no_expr_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "No_expr" << endl;
  
  // ADD ANY CODE HERE
  set_expr_type(env, op_type());
}

void static_dispatch_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "static dispatch" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}

void string_const_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "string_const" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}

void dispatch_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "dispatch" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}

// Handle a Cool case expression (selecting based on the type of an object)
void typcase_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "typecase::make_alloca()" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}

void new__class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "newClass" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}

void isvoid_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "isvoid" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}

void branch_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "branch_class" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}

void method_class::make_alloca(CgenEnvironment *env) {
  // ADD ANY CODE HERE
}

void attr_class::make_alloca(CgenEnvironment *env) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}
