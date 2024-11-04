//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully and add code to build an LLVM program
//**************************************************************

#include <alloca.h>
#include <strings.h>

#include <ostream>

#include "operand.h"
#define EXTERN
#include <sstream>
#include <string>

#include "cgen.h"

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
    No_class,   // symbol that can't be the name of any user-defined class
    No_type,    // If e : No_type, then no code is generated for e.
    SELF_TYPE,  // Special code is generated for new SELF_TYPE.
    self,       // self generates code differently than other references

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
  vp = ValuePrinter(*ct_stream);

  vp.declare({"Object*"}, "Object_new", {});
  vp.declare({"Object*"}, "Object_abort", {{"Object*"}});
  vp.declare({"String*"}, "Object_type_name", {{"Object*"}});
  vp.declare({"Object*"}, "Object_copy", {{"Object*"}});

  vp.declare({"IO*"}, "IO_new", {});
  vp.declare({"IO*"}, "IO_out_string", {{"IO*"}, {"String*"}});
  vp.declare({"IO*"}, "IO_out_int", {{"IO*"}, {INT32}});
  vp.declare({"String*"}, "IO_in_string", {{"IO*"}});
  vp.declare({INT32}, "IO_in_int", {{"IO*"}});

  vp.declare({"String*"}, "String_new", {{}});
  vp.declare({INT32}, "String_length", {{"String*"}});
  vp.declare({"String*"}, "String_concat", {{"String*"}, {"String*"}});
  vp.declare({"String*"}, "String_substr", {{"String*"}, {INT32}, {INT32}});

  vp.declare({"Int*"}, "Int_new", {{}});

  vp.declare({"Bool*"}, "Bool_new", {{}});

  vp.declare({"Bool*"}, "create_Bool", {{INT1}});
  vp.declare({"Int*"}, "create_Int", {{INT32}});

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

  if (probe(name)) return;

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

  if (probe(name)) return;

  // The class name is legal, so add it to the list of special classes
  // and the symbol table.
  special_nds = new List<CgenNode>(nd, special_nds);
  addid(name, nd);
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree() {
  for (auto node : nds) set_relations(node);
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
  if (c && !c->basic()) return c;  // Found it!

  for (auto child : c->get_children()) {
    if (CgenNode *foundMain = this->getMainmain(child))
      return foundMain;  // Propagate it up the recursive calls
  }

  return 0;  // Make the recursion continue
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
  stringtable.code_string_table(*ct_stream, this);
#endif
}

// generate code to define a global string constant
void StringEntry::code_def(ostream &s, CgenClassTable *ct) {
#ifdef MP3
  // ADD CODE HERE
  ValuePrinter vp(s);
  string literal_name = string("str.") + std::to_string(index);
  string object_name = string("String.") + std::to_string(index);

  string class_name = "String";
  op_type self_type_ptr(class_name, 1);
  op_type self_type = self_type_ptr.get_deref_type();
  string vtable_type_name = "_" + class_name + "_vtable";
  op_type vtable_type(vtable_type_name, 1);
  string prototype_name = vtable_type_name + "_prototype";
  const_value vtable_val =
      const_value{vtable_type, "@" + prototype_name, false};

  op_type literal_ty = op_arr_type(INT8, strlen(this->str) + 1);
  const_value literal_const(literal_ty, this->str, true);
  vp.init_constant(literal_name, literal_const);

  vp.init_struct_constant(
      global_value{self_type, object_name}, {vtable_type, {INT8_PTR}},
      {vtable_val, const_value{literal_ty, "@" + literal_name, false}});
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
  if (cgen_debug) std::cerr << "Building CgenClassTable" << endl;
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

  c->setup(current_tag++, depth, ct_stream);
  for (auto child : c->get_children()) setup_classes(child, depth + 1);

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
  c->code_class();
  for (auto child : c->get_children()) code_classes(child);
}
#endif

//
// Create LLVM entry point. This function will initiate our Cool program
// by generating the code to execute (new Main).main()
//
op_type main_return_type;
void CgenClassTable::code_main() {
  // Define a function main that has no parameters and returns an i32

  // Define an entry basic block

  // Call Main_main(). This returns int* for phase 1, Object for phase 2

  if (cgen_debug) std::cerr << "===Code Main===" << endl;
  ValuePrinter vp(*ct_stream);

  vp.define({INT32}, "main", {});
  vp.begin_block("entry");

#ifndef MP3
  string msg_name = "main.printout.str";
  string msg = "Main.main() returned %d\n";
  op_type msg_ty = op_arr_type(INT8, msg.size() + 1);
  const_value msg_const(msg_ty, msg, true);
  vp.init_constant(msg_name, msg_const);

  operand ret = vp.call({}, {INT32}, "Main.main", true, {});
  op_arr_ptr_type msg_ptr_ty(INT8, msg.size() + 1);
  global_value msg_glob(msg_ptr_ty, msg_name, msg_const);
  operand msg_arr_ptr =
      vp.getelementptr(msg_ty, msg_glob, int_value(0), int_value(0), INT8_PTR);
  vp.call({INT8_PTR, {VAR_ARG}}, {INT32}, "printf", true, {msg_arr_ptr, ret});
#else
  // MP3
  operand main_obj = vp.call({{}}, {"Main*"}, "Main_new", true, {});
  vp.call({{"Main*"}}, main_return_type, "Main_main", true, {main_obj});

#endif
  vp.ret(int_value(0));
  vp.end_define();
}

///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTable *ct)
    : class__class((const class__class &)*nd),
      parentnd(0),
      children(0),
      basic_status(bstatus),
      class_table(ct),
      tag(-1) {
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
// My helpers
#define str_eq(a, b) (strcmp(a, b) == 0)
op_type sym_as_type(Symbol s, CgenNode *cls) {
  char *symbol_str = s->get_string();
  if (cgen_debug) std::cerr << "Operand conversion: " << symbol_str << endl;
  if (str_eq(symbol_str, "Int")) return op_type(INT32);
  if (str_eq(symbol_str, "int")) return op_type(INT32);
  if (str_eq(symbol_str, "Bool")) return op_type(INT1);
  if (str_eq(symbol_str, "bool")) return op_type(INT1);
  if (str_eq(symbol_str, "sbyte")) return op_type(INT8);
  if (str_eq(symbol_str, "sbyte*")) return op_type(INT8_PTR);
  if (str_eq(symbol_str, "SELF_TYPE")) return op_type(cls->get_type_name());
  return op_type(symbol_str);
}
op_type sym_as_type(Symbol s, CgenEnvironment *env) {
  return sym_as_type(s, env->get_class());
}
bool is_basic_type(Symbol s) {
  char *symbol_str = s->get_string();
  if (str_eq(symbol_str, "Int")) return true;
  if (str_eq(symbol_str, "int")) return true;
  if (str_eq(symbol_str, "Bool")) return true;
  if (str_eq(symbol_str, "bool")) return true;
  if (string(symbol_str).find("*") != string::npos) return true;
  return false;
}
op_type sym_as_type_passable(Symbol s, CgenNode *cls) {
  auto ty = sym_as_type(s, cls);
  if (is_basic_type(s)) return ty;
  return ty.get_ptr_type();
}
#define vp_init auto vp = ValuePrinter(*(env->cur_stream));
#define nvp() (ValuePrinter(*(env->cur_stream)))
#define ret_code_bin_op_vals(op, e1, e2) return nvp().op(e1, e2);
#define ret_code_bin_op(op) \
  ret_code_bin_op_vals(op, e1->code(env), e2->code(env))

template <class L, class T>
bool list_contains(L list, T target) {
  for (auto l : list)
    if (l == target) return true;
  return false;
}
bool replace(std::string &str, const std::string &from, const std::string &to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos) return false;
  str.replace(start_pos, from.length(), to);
  return true;
}
std::string replaceAll(std::string str, const std::string &from,
                       const std::string &to) {
  if (from.empty()) return "";
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();  // In case 'to' contains 'from', like replacing
                               // 'x' with 'yx'
  }
  return str;
}
op_type return_type_boxed(op_type ty) {
  auto id = ty.get_id();
  if (id == INT1 || id == INT8_PTR || id == INT32) return ty;
  return ty.get_ptr_type();
}
operand get_init_val(op_type ty, ValuePrinter &vp) {
  auto id = ty.get_id();
  if (id == INT1) return const_value{{INT1}, "0", false};
  if (id == INT8_PTR) return const_value{{INT8_PTR}, "null", false};
  if (id == INT32) return const_value{{INT32}, "0", false};
  if (str_eq(ty.get_name().c_str(), "\%String*"))
    return vp.call({}, op_type{"String", 1}, "String_new", true, {});
  if (str_eq(ty.get_name().c_str(), "\%IO*"))
    return vp.call({}, op_type{"IO", 1}, "IO_new", true, {});
  return vp.bitcast(const_value{{INT8_PTR}, "null", false}, ty);
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
void CgenNode::setup(int tag, int depth, ostream *ct_stream) {
  this->tag = tag;
  this->ct_stream = ct_stream;
#ifdef MP3
  layout_features();

  // ADD CODE HERE
  ValuePrinter vp(*ct_stream);
  // Name of class
  string class_name = name->get_string();
  const_value type_name_const(op_arr_type(INT8, class_name.length() + 1),
                              class_name, true);
  vp.init_constant(string("str.") + class_name, type_name_const);

  // Vtable and instance
  this->vtable_type_name = "_" + string(name->get_string()) + "_vtable";
  this->vtable_ptr_ty = op_type(vtable_type_name, 1);

  // Type
  vector<op_type> attr_types{vtable_ptr_ty};
  for (auto attr : member_attrs) {
    attr_types.push_back(attr.type);
  }
  vp.type_define(name->get_string(), attr_types);

  // VTable
  vector<op_type> vtable_types{{INT32}, {INT32}, {INT8_PTR}};
  // i32 ptrtoint (%Main* getelementptr (%Main, %Main* null, i32 1) to i32),
  vector<const_value> vtable_init_values{
      int_value(tag),
      const_value(
          {INT32},
          replaceAll(
              "ptrtoint (%!!* getelementptr (%!!, %!!* null, i32 1) to i32)",
              "!!", class_name),
          false),
      const_value(op_arr_type(INT8, class_name.length() + 1),
                  string("@str.") + class_name, true)};
  for (auto method : member_methods) {
    vtable_types.push_back(method.func_ty);
    vtable_init_values.push_back(method.func_val);
  }
  vp.type_define(vtable_type_name, vtable_types);

  // TODO: generate type_define and vtable
  // Prototype
  prototype_name = vtable_type_name + "_prototype";
  vp.init_struct_constant(
      global_value{vtable_ptr_ty.get_deref_type(), prototype_name},
      vtable_types, vtable_init_values);

#endif
}

#ifdef MP3
//
// Class codegen. This should performed after every class has been setup.
// Generate code for each method of the class.
//
void CgenNode::code_class() {
  // No code generation for basic classes. The runtime will handle that.
  if (basic()) return;

  CgenEnvironment env(*ct_stream, this);

  // Generate methods, except inherited ones
  for (auto m : member_methods)
    if (list_contains(features, m.method)) m.method->code(&env);

  // Generate obj_new
  ValuePrinter vp(*ct_stream);
  auto &o = *(env.cur_stream);

  string class_name = name->get_string();
  op_type self_type_ptr(get_type_name(), 1);
  op_type self_type = self_type_ptr.get_deref_type();
  global_value vtable_val = global_value{vtable_ptr_ty, prototype_name};

  vp.define(self_type_ptr, std::string(get_name()->get_string()) + "_new", {});
  {
    auto ok_label = env.new_ok_label();
    vp.begin_block("entry");

    operand size_addr = vp.getelementptr(
        vtable_ptr_ty.get_deref_type(),
        const_value(vtable_ptr_ty, "@" + prototype_name, true), int_value(0),
        int_value(1), op_type(INT32).get_ptr_type());
    operand size = vp.load({INT32}, size_addr);
    operand new_ptr = vp.bitcast(vp.malloc_mem(size), self_type_ptr);
    vp.branch_cond(vp.icmp(EQ, new_ptr, const_value{{"null"}, "null", false}),
                   "abort", ok_label);

    vp.begin_block(ok_label);
    // push self on stack
    operand *self_stack =
        new operand{new_ptr.get_type().get_ptr_type(), "self_ptr"};
    vp.alloca_mem(o, self_type_ptr, *self_stack);
    vp.store(new_ptr, *self_stack);
    env.add_local(self, *self_stack);

    // fill in vtable ptr
    operand vtable_dst_addr =
        vp.getelementptr(self_type, new_ptr, int_value(0), int_value(0),
                         vtable_ptr_ty.get_ptr_type());
    vp.store(vtable_val, vtable_dst_addr);

    int i = 1;  // initialize fields
    for (auto attr : member_attrs) attr.attr->make_alloca(&env);

    for (auto attr : member_attrs) {
      operand init_val = attr.init->code(&env);
      if (attr.init->no_code()) init_val = get_init_val(attr.type, vp);

      operand dst_addr =
          vp.getelementptr(self_type, new_ptr, int_value(0), int_value(i),
                           attr.type.get_ptr_type());

      // init_val = vp.bitcast(init_val, attr.type);
      init_val = conform(init_val, attr.type, &env);
      vp.store(init_val, dst_addr);
      i++;
    }
    vp.ret(new_ptr);

    vp.begin_block("abort");
    vp.call({}, {VOID}, "abort", true, {});
    vp.unreachable();
    vp.end_define();
  }

  // ADD CODE HERE
}

// Laying out the features involves creating a Function for each method
// and assigning each attribute a slot in the class structure.
void CgenNode::layout_features() {
  // ADD CODE HERE
  // Inherit everything from parent
  member_attrs = parentnd->member_attrs;
  member_methods = parentnd->member_methods;
  for (auto f : features) {
    f->layout_feature(this);
  }
}
#else

//
// code-gen function main() in class Main
//
void CgenNode::codeGenMainmain() {
  ValuePrinter vp(*ct_stream);
  // In Phase 1, this can only be class Main. Get method_class for main().
  assert(std::string(this->name->get_string()) == std::string("Main"));
  method_class *mainMethod = (method_class *)features->nth(features->first());

  // ADD CODE HERE TO GENERATE THE FUNCTION int Mainmain().
  // Generally what you need to do are:
  // -- setup or create the environment, env, for translating this method
  // -- invoke mainMethod->code(env) to translate the method
  CgenEnvironment env(*ct_stream, this);
  mainMethod->code(&env);
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
CgenNode *CgenEnvironment::op_type_to_class(op_type ty) {
  while (ty.is_ptr()) ty = ty.get_deref_type();
  op_type_id id = ty.get_id();

  if (id == INT32) return type_to_class(Int);
  if (id == INT1) return type_to_class(Bool);
  Symbol s = idtable.lookup_string(ty.get_name().substr(1).c_str());
  return type_to_class(s);
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
  vp_init;
  op_type src_type = src.get_type();

  if (src_type.is_same_with(type)) return src;

  // Cast
  if (src_type.is_ptr() && type.is_ptr()) {
    return vp.bitcast(src, type);
  }

  // Unbox
  if (src_type.is_ptr() && src_type.is_int_object()) {
    operand ptr = vp.getelementptr(src_type.get_deref_type(), src, int_value(0),
                                   int_value(1), {INT32_PTR});
    return vp.load({INT32}, ptr);
  }
  if (src_type.is_ptr() && src_type.is_bool_object()) {
    operand ptr = vp.getelementptr(src_type.get_deref_type(), src, int_value(0),
                                   int_value(1), {INT1_PTR});
    return vp.load({INT1}, ptr);
  }

  // Box
  if (!src_type.is_ptr() && src_type.get_id() == INT32) {
    return vp.call({{INT32}}, {"Int", 1}, "create_Int", true, {src});
  }
  if (!src_type.is_ptr() && src_type.get_id() == INT1) {
    return vp.call({{INT1}}, {"Bool", 1}, "create_Bool", true, {src});
  }

  return src;
}

operand load_vtable_ptr(operand src, CgenNode *src_cls, CgenEnvironment *env) {
  vp_init;
  operand vtb_ptr_addr =
      vp.getelementptr(src.get_type().get_deref_type(), src, int_value(0),
                       int_value(0), src_cls->vtable_ptr_ty.get_ptr_type());
  operand vtb_ptr = vp.load(src_cls->vtable_ptr_ty, vtb_ptr_addr);
  return vtb_ptr;
}

operand ith_from_vtable(int i, op_type expected_ty, operand src,
                        CgenNode *src_cls, CgenEnvironment *env) {
  vp_init;
  operand vtb_ptr_addr =
      vp.getelementptr(src.get_type().get_deref_type(), src, int_value(0),
                       int_value(0), src_cls->vtable_ptr_ty.get_ptr_type());
  operand vtb_ptr = vp.load(src_cls->vtable_ptr_ty, vtb_ptr_addr);

  operand tag_ptr =
      vp.getelementptr(src_cls->vtable_ptr_ty.get_deref_type(), vtb_ptr,
                       int_value(0), int_value(i), expected_ty.get_ptr_type());
  operand tag = vp.load(expected_ty, tag_ptr);
  return tag;
}

// Retrieve the class tag from an object record.
// src is the object we need the tag from.
// src_class is the CgenNode for the *static* class of the expression.
// You need to look up and return the class tag for it's dynamic value
operand get_class_tag(operand src, CgenNode *src_cls, CgenEnvironment *env) {
  // ADD CODE HERE (MP3 ONLY)
  vp_init;
  if (!src.get_type().is_ptr()) return int_value(src_cls->get_tag());

  // operand vtb_ptr_addr =
  // vp.getelementptr(src.get_type(), int_value(0), int_value(0),
  // src_cls->vtable_ptr_ty.get_ptr_type());
  // operand vtb_ptr = vp.load(src_cls->vtable_ptr_ty, vtb_ptr_addr);

  // operand tag_ptr = vp.getelementptr(src_cls->vtable_ptr_ty.get_deref_type(),
  // int_value(0), int_value(0), {INT32_PTR});
  // operand tag = vp.load({INT32}, tag_ptr);
  return ith_from_vtable(0, {INT32}, src, src_cls, env);
}
#endif

//
// Create a method body
//
void method_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "method" << endl;
  // ADD CODE HERE
  vp_init;
  auto &o = *(env->cur_stream);
  string method_name =
      env->get_class()->get_type_name() + "_" + name->get_string();

  operand self_op({env->get_class()->get_type_name(), 1}, "self");

  std::vector<operand> args{self_op};
  operand *self_stack =
      new operand{self_op.get_type().get_ptr_type(), "self_ptr"};
  env->add_local(self, *self_stack);  // add self

  for (auto formal : formals) {
    op_type arg_ty =
        sym_as_type_passable(formal->get_type_decl(), env->get_class());
    operand arg(arg_ty, formal->get_name()->get_string());
    operand *arg_stack = new operand{arg_ty.get_ptr_type(),
                                     string(arg.get_name()).substr(1) + "_ptr"};
    env->add_local(formal->get_name(), *arg_stack);
    args.push_back(arg);
  }

  vp.define(return_type_boxed(sym_as_type(get_return_type(), env)), method_name,
            args);
  // Push args on stack
  for (auto arg : args) {
    op_type arg_ty = arg.get_type();
    operand arg_stack = {arg_ty.get_ptr_type(),
                         string(arg.get_name()).substr(1) + "_ptr"};
    vp.alloca_mem(o, arg_ty, arg_stack);
    vp.store(arg, arg_stack);
    env->method_var_count++;
  }

  expr->make_alloca(env);
  operand ret_op = expr->code(env);

  // derefence basic types on return
  ret_op = conform(ret_op,
                   return_type_boxed(sym_as_type(get_return_type(), env)), env);
  ret_op = conform(ret_op,
                   return_type_boxed(sym_as_type(get_return_type(), env)), env);

  vp.ret(ret_op);

  vp.begin_block("abort");
  vp.call({}, {VOID}, "abort", true, {});
  vp.unreachable();
  vp.end_define();
  while (env->method_var_count-- > 0) env->kill_local();
}

//
// Codegen for expressions.  Note that each expression has a value.
//

operand deref_stack(operand stack_ptr, CgenEnvironment *env) {
  vp_init;
  return vp.load(stack_ptr.get_type().get_deref_type(), stack_ptr);
}

operand assign_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "assign" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;
  operand val = expr->code(env);
  operand *dst_ptr = env->lookup(name);

  if (dst_ptr) {  // dst is local var
    val = conform(val, dst_ptr->get_type().get_deref_type(), env);
    vp.store(val, *dst_ptr);
    return val;

  } else {  // dst is attribute
    operand self_ptr = deref_stack(*(env->lookup(self)), env);
    CgenNode::Attr *attr = env->get_class()->get_attr(name);

    operand attr_ptr = vp.getelementptr(
        self_ptr.get_type().get_deref_type(), self_ptr, int_value(0),
        int_value(attr->attr_idx), attr->type.get_ptr_type());

    val = conform(val, attr_ptr.get_type().get_deref_type(), env);
    vp.store(val, attr_ptr);
    return val;
  }

  return val;
}

operand cond_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "cond" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;
  string true_label = env->new_label("if_true", 1),
         false_label = env->new_label("if_false", 0),
         done_label = env->new_label("end_if", 0);

  operand pr_val = pred->code(env);
  // op_type ret_ty = sym_as_type(this->get_type(), env); //TODO ret ty wrong
  // swap(null_stream, env->cur_stream);  // disable output for now
  auto good_stream = env->cur_stream;
  env->cur_stream = null_stream;
  op_type ret_ty = then_exp->code(env).get_type();
  *env->cur_stream << "aaaaaaaaaaaaaaa\n";
  env->cur_stream = good_stream;
  // swap(null_stream, env->cur_stream);  // enable again

  operand dst = vp.alloca_mem(ret_ty);
  vp.branch_cond(pr_val, true_label, false_label);

  vp.begin_block(true_label);
  operand then_op = then_exp->code(env);
  then_op = conform(then_op, ret_ty, env);
  vp.store(then_op, dst);
  vp.branch_uncond(done_label);

  vp.begin_block(false_label);
  operand else_op = else_exp->code(env);
  else_op = conform(else_op, ret_ty, env);
  vp.store(else_op, dst);
  vp.branch_uncond(done_label);

  vp.begin_block(done_label);
  return vp.load(ret_ty, dst);
}

operand loop_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "loop" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;
  string body_label = env->new_label("loop_body", 1),
         done_label = env->new_label("loop_done", 0);

  auto good_stream = env->cur_stream;
  env->cur_stream = null_stream;
  op_type ret_ty = body->code(env).get_type();
  *env->cur_stream << "aaaaaaaaaaaaaaa\n";
  env->cur_stream = good_stream;

  operand dst = vp.alloca_mem(ret_ty);
  vp.branch_uncond(body_label);

  vp.begin_block(body_label);
  operand body_op = body->code(env);
  vp.store(body_op, dst);
  operand pred_op = pred->code(env);
  vp.branch_cond(pred_op, body_label, done_label);

  vp.begin_block(done_label);
  return vp.load(ret_ty, dst);
}

operand block_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "block" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL

  operand last;
  for (auto exp_cls : body) {
    last = exp_cls->code(env);
  }
  return last;
}

operand let_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "let" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;
  op_type ty = sym_as_type_passable(type_decl, env->get_class());

  operand val = init->code(env);
  // operand dst_reg(ty, identifier->get_string());
  operand &dst_stack = *(new operand{});
  dst_stack = vp.alloca_mem(ty);
  env->add_local(identifier, dst_stack);

  if (val.is_empty()) {  // fill in default value if not specified
    if (str_eq(type_decl->get_string(), "Int"))
      val = int_value(0);
    else if (str_eq(type_decl->get_string(), "Bool"))
      val = bool_value(false, true);
    else
      val = null_value{ty};
  } else
    val = conform(val, ty, env);
  vp.store(val, dst_stack);

  operand ret = body->code(env);
  env->kill_local();
  return ret;
}

operand plus_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "plus" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  ret_code_bin_op(add);
}

operand sub_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "sub" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  ret_code_bin_op(sub);
}

operand mul_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "mul" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  ret_code_bin_op(mul);
}

operand divide_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "div" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;
  auto is_zero = vp.icmp(EQ, e2->code(env), int_value(0));
  auto ok = env->new_ok_label();
  vp.branch_cond(is_zero, "abort", ok);

  vp.begin_block(ok);
  ret_code_bin_op(div);
}

operand neg_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "neg" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  ret_code_bin_op_vals(mul, int_value(-1), e1->code(env));
}

operand lt_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "lt" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  return nvp().icmp(LT, e1->code(env), e2->code(env));
}

operand eq_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "eq" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;
  operand e1o = e1->code(env);
  operand e2o = e2->code(env);
  if (!e1o.get_type().is_same_with(e2o.get_type())) {
    e1o = conform(e1o, {INT8_PTR}, env);
    e2o = conform(e2o, {INT8_PTR}, env);
  }
  return vp.icmp(EQ, e1o, e2o);
}

operand leq_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "leq" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  return nvp().icmp(LE, e1->code(env), e2->code(env));
}

operand comp_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "complement" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  return nvp().xor_in(e1->code(env), bool_value(true, true));
}

operand int_const_class::code(CgenEnvironment *env) {
  if (cgen_debug)
    std::cerr << "Integer Constant:" << token->get_string() << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  return int_value(atoi(token->get_string()));
}

operand bool_const_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "Boolean Constant" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  return bool_value(val, false);
}

operand object_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "Object" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;
  operand *src = env->lookup(name);
  if (src) {
    return deref_stack(*src, env);
  } else  // self type
  {
    operand self_loaded = deref_stack(*env->lookup(self), env);
    CgenNode::Attr *attr = env->get_class()->get_attr(name);

    if (attr) {
      operand attr_ptr = vp.getelementptr(
          self_loaded.get_type().get_deref_type(), self_loaded, int_value(0),
          int_value(attr->attr_idx), attr->type.get_ptr_type());
      return vp.load(attr->type, attr_ptr);
    }

    return self_loaded;
  }
}

operand no_expr_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "No_expr" << endl;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  // TODO use the no_class type ?
  return operand();
}

//*****************************************************************
// The next few code() functions are for node types not supported
// in Phase 1 but these functions must be defined because they are
// declared as methods via the Expression_SHARED_EXTRAS hack.
//*****************************************************************

operand static_dispatch_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "static dispatch" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  auto &o = *env->cur_stream;
  vp_init;
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  operand self_val = expr->code(env);

  if (self_val.get_type().get_id() == INT32)
    self_val = conform(self_val, {"Int", 1}, env);
  else if (self_val.get_type().get_id() == INT1)
    self_val = conform(self_val, {"Bool", 1}, env);
  else {
    // TODO error handling with null_value{self_val.get_type()}
    auto ok = env->new_ok_label();
    operand is_null = vp.icmp(EQ, self_val, null_value{self_val.get_type()});
    vp.branch_cond(is_null, "abort", ok);
    vp.begin_block(ok);
  }

  op_type self_ty = self_val.get_type();

  std::vector<operand> actual_args{self_val};

  for (auto exp : actual) {
    actual_args.push_back(exp->code(env));
  }

  CgenNode *self_cls = env->type_to_class(type_name);

  CgenNode::Method *to_call = self_cls->get_method(name);
  // TODO find the right vtable function, call it

  for (int i = 0; i < actual_args.size(); i++) {
    actual_args[i] = conform(actual_args[i], to_call->arg_types[i], env);
  }

  string resolved_func_name = to_call->func_val.get_name().substr(1);
  auto ret = vp.call(to_call->arg_types, to_call->ret_ty, resolved_func_name,
                     true, actual_args);
  // if (str_eq(env->op_type_to_class(self_val.get_type()) // special case
  //->get_parentnd()
  //->get_type_name()
  //.c_str(),
  // self_cls->get_type_name().c_str())) {
  // ret = vp.bitcast(ret, self_val.get_type());
  //}
  return ret;
#endif
  return operand();
}

operand string_const_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "string_const" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
    // MORE MEANINGFUL
#endif
  auto idx = stringtable.lookup_string(this->token->get_string())->get_index();
  return global_value(op_type{"String", 1},
                      string("String.") + std::to_string(idx));
}

operand dispatch_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "dispatch" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  auto &o = *env->cur_stream;
  vp_init;
  operand self_val = expr->code(env);

  if (self_val.get_type().get_id() == INT32) {
    self_val = conform(self_val, {"Int", 1}, env);
  } else if (self_val.get_type().get_id() == INT1)
    self_val = conform(self_val, {"Bool", 1}, env);

  op_type self_ty = self_val.get_type();

  // TODO error handling with null_value{self_val.get_type()}

  std::vector<operand> actual_args{self_val};

  for (auto exp : actual) {
    actual_args.push_back(exp->code(env));
  }

  CgenNode *self_cls = env->op_type_to_class(self_ty);

  CgenNode::Method *to_call = self_cls->get_method(name);

  // find the right vtable function, call it
  operand vtb_ptr = load_vtable_ptr(self_val, self_cls, env);
  operand func_ptr = vp.getelementptr(
      self_cls->vtable_ptr_ty.get_deref_type(), vtb_ptr, int_value(0),
      int_value(to_call->vtable_idx), to_call->func_ptr_ty);
  operand func_to_call{to_call->func_ptr_ty.get_deref_type(), env->new_name()};
  vp.load(o, func_to_call.get_type(), func_ptr, func_to_call);

  for (int i = 0; i < actual_args.size(); i++) {
    actual_args[i] = conform(actual_args[i], to_call->arg_types[i], env);
  }

  operand ret{to_call->ret_ty, env->new_name()};
  vp.call(o, to_call->arg_types, func_to_call.get_name().substr(1), false,
          actual_args, ret);

  if (str_eq(name->get_string(), "copy")) {  // special case for object copy
    ret = vp.bitcast(ret, self_ty);
  }

  return ret;
#endif
  return operand();
}

operand typcase_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "typecase::code()" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  ValuePrinter vp(*env->cur_stream);
  CgenClassTable *ct = env->get_class()->get_classtable();

  string header_label = env->new_label("case.hdr.", false);
  string exit_label = env->new_label("case.exit.", false);

  // Generate code for expression to select on, and get its static type
  operand code_val = expr->code(env);
  operand expr_val = code_val;
  string code_val_t = code_val.get_typename();
  op_type join_type = env->type_to_class(type)->get_type_name();
  CgenNode *cls = env->type_to_class(expr->get_type());

  // Check for case on void, which gives a runtime error
  if (code_val.get_type().get_id() != INT32 &&
      code_val.get_type().get_id() != INT1) {
    op_type bool_type(INT1), empty_type;
    null_value null_op(code_val.get_type());
    operand icmp_result(bool_type, env->new_name());
    vp.icmp(*env->cur_stream, EQ, code_val, null_op, icmp_result);
    string ok_label = env->new_ok_label();
    vp.branch_cond(icmp_result, "abort", ok_label);
    vp.begin_block(ok_label);
  }

  operand tag = get_class_tag(expr_val, cls, env);
  vp.branch_uncond(header_label);
  string prev_label = header_label;
  vp.begin_block(header_label);

  env->branch_operand = alloca_op;

  std::vector<operand> values;
  env->next_label = exit_label;

  // Generate code for the branches
  for (int i = ct->get_num_classes() - 1; i >= 0; --i) {
    for (auto case_branch : cases) {
      if (i == ct->lookup(case_branch->get_type_decl())->get_tag()) {
        string prefix = string("case.") + itos(i) + ".";
        string case_label = env->new_label(prefix, false);
        vp.branch_uncond(case_label);
        vp.begin_block(case_label);
        operand val = case_branch->code(expr_val, tag, join_type, env);
        values.push_back(val);
      }
    }
  }

  // Abort if there was not a branch covering the actual type
  vp.branch_uncond("abort");

  // Done with case expression: get final result
  env->new_label("", true);
  vp.begin_block(exit_label);
  operand final_result(alloca_type, env->new_name());
  alloca_op.set_type(alloca_op.get_type().get_ptr_type());
  vp.load(*env->cur_stream, alloca_op.get_type().get_deref_type(), alloca_op,
          final_result);
  alloca_op.set_type(alloca_op.get_type().get_deref_type());

  if (cgen_debug) cerr << "Done typcase::code()" << endl;
  return final_result;
#endif
}

operand new__class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "newClass" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;
  if (get_type() == SELF_TYPE) {
    // TODO new SELF_TYPE
  } else {
    CgenNode *cls = env->type_to_class(type_name);
    op_type ret_ty{cls->get_type_name(), 1};
    return vp.call({}, ret_ty, cls->get_type_name() + "_new", true, {});
  }
#endif
  return operand();
}

operand isvoid_class::code(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "isvoid" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE AND REPLACE "return operand()" WITH SOMETHING
  // MORE MEANINGFUL
  vp_init;

  operand e1o = e1->code(env);
  op_type e1ty = e1o.get_type();

  if (!e1ty.is_ptr()) return bool_value(false, true);

  return vp.icmp(EQ, e1o, null_value{e1ty});

#endif
  return operand();
}

// Create the LLVM Function corresponding to this method.
void method_class::layout_feature(CgenNode *cls) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE
  CgenNode::Method entry{.method = this, .name = name, .expr = expr};

  string method_name = cls->get_type_name() + "_" + name->get_string();

  operand self({cls->get_type_name(), 1}, "self");

  std::vector<op_type> arg_types{self.get_type()};
  std::vector<operand> args{self};
  for (auto formal : formals) {
    op_type arg_ty = sym_as_type_passable(formal->get_type_decl(), cls);
    operand arg(arg_ty, formal->get_name()->get_string());
    arg_types.push_back(arg_ty);
    args.push_back(arg);
  }
  entry.args = args;
  entry.arg_types = arg_types;
  entry.ret_ty = sym_as_type_passable(get_return_type(), cls);
  if (str_eq(method_name.c_str(), "Main_main")) main_return_type = entry.ret_ty;

  entry.func_ty = op_func_type(entry.ret_ty, arg_types);
  entry.func_ptr_ty = op_func_ptr_type(entry.ret_ty, arg_types);

  global_value func_global({method_name}, method_name);
  entry.func_val = const_value({method_name}, func_global.get_name(), false);

  entry.vtable_idx = cls->member_methods.size() + 3;

  if (CgenNode::Method *parent_entry = cls->get_method(entry.name)) {
    entry.vtable_idx = parent_entry->vtable_idx;
    *parent_entry = entry;
  } else
    cls->member_methods.push_back(entry);

#endif
}

// Handle one branch of a Cool case expression.
// If the source tag is >= the branch tag
// and <= (max child of the branch class) tag,
// then the branch is a superclass of the source.
// See the MP3 handout for more information about our use of class tags.
operand branch_class::code(operand expr_val, operand tag, op_type join_type,
                           CgenEnvironment *env) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  operand empty;
  ValuePrinter vp(*env->cur_stream);
  if (cgen_debug) cerr << "In branch_class::code()" << endl;

  CgenNode *cls = env->get_class()->get_classtable()->lookup(type_decl);
  int my_tag = cls->get_tag();
  int max_child = cls->get_max_child();

  // Generate unique labels for branching into >= branch tag and <= max child
  string sg_label =
      env->new_label(string("src_gte_br") + "." + itos(my_tag) + ".", false);
  string sl_label =
      env->new_label(string("src_lte_mc") + "." + itos(my_tag) + ".", false);
  string exit_label =
      env->new_label(string("br_exit") + "." + itos(my_tag) + ".", false);

  int_value my_tag_val(my_tag);
  op_type old_tag_t(tag.get_type()), i32_t(INT32);
  tag.set_type(i32_t);

  // Compare the source tag to the class tag
  operand icmp_result = vp.icmp(LT, tag, my_tag_val);
  vp.branch_cond(icmp_result, exit_label, sg_label);
  vp.begin_block(sg_label);
  int_value max_child_val(max_child);

  // Compare the source tag to max child
  operand icmp2_result = vp.icmp(GT, tag, max_child_val);
  vp.branch_cond(icmp2_result, exit_label, sl_label);
  vp.begin_block(sl_label);
  tag.set_type(old_tag_t);

  // Handle casts of *arbitrary* types to Int or Bool.  We need to:
  // (a) cast expr_val to boxed type (struct Int* or struct Bool*)
  // (b) unwrap value field from the boxed type
  // At run-time, if source object is not Int or Bool, this will never
  // be invoked (assuming no bugs in the type checker).
  if (cls->get_type_name() == "Int") {
    expr_val = conform(expr_val, op_type(cls->get_type_name(), 1), env);
  } else if (cls->get_type_name() == "Bool") {
    expr_val = conform(expr_val, op_type(cls->get_type_name(), 1), env);
  }

  // If the case expression is of the right type, make a new local
  // variable for the type-casted version of it, which can be used
  // within the expression to evaluate on this branch.
  operand conf_result = conform(expr_val, alloca_type, env);
  vp.store(conf_result, alloca_op);
  env->add_local(name, alloca_op);

  // Generate code for the expression to evaluate on this branch
  operand val = conform(expr->code(env), join_type.get_ptr_type(), env);
  operand conformed = conform(val, env->branch_operand.get_type(), env);
  env->branch_operand.set_type(env->branch_operand.get_type().get_ptr_type());
  // Store result of the expression evaluated
  vp.store(conformed, env->branch_operand);
  env->branch_operand.set_type(env->branch_operand.get_type().get_deref_type());
  env->kill_local();
  // Branch to case statement exit label
  vp.branch_uncond(env->next_label);
  vp.begin_block(exit_label);
  if (cgen_debug) cerr << "Done branch_class::code()" << endl;
  return conformed;
#endif
}

// Assign this attribute a slot in the class structure
void attr_class::layout_feature(CgenNode *cls) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD CODE HERE
  CgenNode::Attr entry{.attr = this, .name = name, .init = init};
  entry.type = sym_as_type_passable(type_decl, cls);
  entry.attr_idx = cls->member_attrs.size() + 1;
  cls->member_attrs.push_back(entry);
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
  if (cgen_debug) std::cerr << "assign" << endl;
  expr->make_alloca(env);
  // ADD ANY CODE HERE
}

void cond_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "cond" << endl;
  vp_init;

  pred->make_alloca(env);
  then_exp->make_alloca(env);
  else_exp->make_alloca(env);
  // ADD ANY CODE HERE
}

void loop_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "loop" << endl;
  pred->make_alloca(env);
  body->make_alloca(env);

  // ADD ANY CODE HERE
}

void block_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "block" << endl;
  for (auto e : body) e->make_alloca(env);

  // ADD ANY CODE HERE
}

void let_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "let" << endl;
  init->make_alloca(env);
  body->make_alloca(env);
  // ADD ANY CODE HERE
}

void plus_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "plus" << endl;
  e1->make_alloca(env);
  e2->make_alloca(env);
  // ADD ANY CODE HERE
}

void sub_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "sub" << endl;
  e1->make_alloca(env);
  e2->make_alloca(env);
  // ADD ANY CODE HERE
}

void mul_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "mul" << endl;
  e1->make_alloca(env);
  e2->make_alloca(env);
  // ADD ANY CODE HERE
}

void divide_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "div" << endl;
  e1->make_alloca(env);
  e2->make_alloca(env);

  // ADD ANY CODE HERE
}

void neg_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "neg" << endl;
  e1->make_alloca(env);

  // ADD ANY CODE HERE
}

void lt_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "lt" << endl;
  e1->make_alloca(env);
  e2->make_alloca(env);

  // ADD ANY CODE HERE
}

void eq_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "eq" << endl;
  e1->make_alloca(env);
  e2->make_alloca(env);

  // ADD ANY CODE HERE
}

void leq_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "leq" << endl;
  e1->make_alloca(env);
  e2->make_alloca(env);

  // ADD ANY CODE HERE
}

void comp_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "complement" << endl;
  e1->make_alloca(env);

  // ADD ANY CODE HERE
}

void int_const_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "Integer Constant" << endl;

  // ADD ANY CODE HERE
}

void bool_const_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "Boolean Constant" << endl;

  // ADD ANY CODE HERE
}

void object_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "Object" << endl;

  // ADD ANY CODE HERE
}

void no_expr_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "No_expr" << endl;

  // ADD ANY CODE HERE
}

void static_dispatch_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "static dispatch" << endl;
  expr->make_alloca(env);
  for (auto a : actual) a->make_alloca(env);

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD ANY CODE HERE
#endif
}

void string_const_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "string_const" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD ANY CODE HERE
#endif
}

void dispatch_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "dispatch" << endl;

  expr->make_alloca(env);
  for (auto a : actual) a->make_alloca(env);
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD ANY CODE HERE
#endif
}

// Handle a Cool case expression (selecting based on the type of an object)
void typcase_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "typecase::make_alloca()" << endl;
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  ValuePrinter vp(*env->cur_stream);
  expr->make_alloca(env);

  // Get result type of case expression
  branch_class *b = (branch_class *)cases->nth(cases->first());
  string case_result_type = b->get_expr()->get_type()->get_string();
  if (case_result_type == "SELF_TYPE")
    case_result_type = env->get_class()->get_type_name();

  // Allocate space for result of case expression
  alloca_type = op_type(case_result_type, 1);
  alloca_op = operand(alloca_type, env->new_name());
  vp.alloca_mem(*env->cur_stream, alloca_type, alloca_op);

  for (auto case_branch : cases) {
    case_branch->make_alloca(env);
  }
#endif
}

void new__class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "newClass" << endl;

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
    // ADD ANY CODE HERE
#endif
}

void isvoid_class::make_alloca(CgenEnvironment *env) {
  if (cgen_debug) std::cerr << "isvoid" << endl;
  e1->make_alloca(env);

#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
#endif
}

void branch_class::make_alloca(CgenEnvironment *env) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  ValuePrinter vp(*env->cur_stream);
  if (cgen_debug) cerr << "In branch_class::make_alloca()" << endl;

  CgenNode *cls = env->get_class()->get_classtable()->lookup(type_decl);
  alloca_type = op_type(cls->get_type_name(), 1);

  if (cls->get_type_name() == "Int") {
    alloca_type = op_type(INT32);
  } else if (cls->get_type_name() == "Bool") {
    alloca_type = op_type(INT1);
  }

  alloca_op = vp.alloca_mem(alloca_type);

  expr->make_alloca(env);

  if (cgen_debug) cerr << "Done branch_class::make_alloca()" << endl;
#endif
}

void method_class::make_alloca(CgenEnvironment *env) {
  // ADD ANY CODE HERE
  expr->make_alloca(env);
}

void attr_class::make_alloca(CgenEnvironment *env) {
#ifndef MP3
  assert(0 && "Unsupported case for phase 1");
#else
  // ADD ANY CODE HERE
  init->make_alloca(env);
#endif
}
