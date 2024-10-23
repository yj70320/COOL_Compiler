/*
 * This file provides the runtime library for cool. It implements
 * the cool classes in C.  Feel free to change it to match the structure
 * of your code generator.
 */

#include <stdbool.h>

typedef struct Object Object;
typedef struct Int Int;
typedef struct Bool Bool;
typedef struct String String;
typedef struct IO IO;

typedef struct _Object_vtable Object_vtable;
typedef struct _Int_vtable Int_vtable;
typedef struct _Bool_vtable Bool_vtable;
typedef struct _String_vtable String_vtable;
typedef struct _IO_vtable IO_vtable;

/* class type definitions */
struct Object {
  /* ADD CODE HERE */
  Object_vtable const *vtblptr;
};

struct Int {
  /* ADD CODE HERE */
  Int_vtable const *vtblptr;
  int val;
};

struct Bool {
  /* ADD CODE HERE */
  Bool_vtable const *vtblptr;
  bool val;
};

struct String {
  /* ADD CODE HERE */
  String_vtable const *vtblptr;
  char *val;
};

struct IO {
  /* ADD CODE HERE */
  IO_vtable const *vtblptr;
};

/* vtable type definitions */
struct _Object_vtable {
  /* ADD CODE HERE */
  int type;
  int size;  //??
  char *name;
  Object *(*Object_new)();
  Object *(*Object_abort)(Object *);
  const String *(*Object_type_name)(Object *);
  Object *(*Object_copy)(Object *);
};

struct _Int_vtable {
  /* ADD CODE HERE */
  int type;
  int size;  //??
  char *name;
  Int *(*Object_new)();
  Object *(*Object_abort)(Object *);
  const String *(*Object_type_name)(Object *);
  Object *(*Object_copy)(Object *);
};

struct _Bool_vtable {
  /* ADD CODE HERE */
  int type;
  int size; 
  char *name;
  Bool *(*Object_new)();
  Object *(*Object_abort)(Object *);
  const String *(*Object_type_name)(Object *);
  Object *(*Object_copy)(Object *);
};

struct _String_vtable {
  /* ADD CODE HERE */
  int type;
  int size; 
  char *name;
  String *(*Object_new)();
  Object *(*Object_abort)(Object *);
  const String *(*Object_type_name)(Object *);
  Object *(*Object_copy)(Object *);
  int (*String_length)(String *);
  String *(*String_concat)(String *, String *);
  String *(*String_substr)(String *, int, int);
};

struct _IO_vtable {
  /* ADD CODE HERE */
  int type;
  int size;  //??
  char *name;
  IO *(*Object_new)();
  Object *(*Object_abort)(Object *);
  const String *(*Object_type_name)(Object *);
  Object *(*Object_copy)(Object *);
  IO *(*IO_out_string)(IO *, String *);
  IO *(*IO_out_int)(IO *, int);
  String *(*IO_in_string)(IO *);
  int (*IO_in_int)(IO *);
};

/* methods in class Object */
Object *Object_abort(Object *self);
const String *Object_type_name(Object *self);
/* ADD CODE HERE */
Object *Object_new();
Object *Object_copy(Object *src);

/* methods in class Int */
/* ADD CODE HERE */
Int *Int_new();

/* methods in class Bool */
/* ADD CODE HERE */
Bool *Bool_new();

/* methods in class String */
/* ADD CODE HERE */
String *String_new();
int String_length(String *src);
String *String_concat(String *src1, String *src2);
String *String_substr(String *src1, int i1, int i2);

/* methods in class IO */
IO *IO_new(void);
IO *IO_out_string(IO *self, String *s);
IO *IO_out_int(IO *self, int x);
String *IO_in_string(IO *self);
int IO_in_int(IO *self);

Int* create_Int(int x);
Bool* create_Bool(bool x);