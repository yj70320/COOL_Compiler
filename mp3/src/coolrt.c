#include "coolrt.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This file provides the runtime library for cool. It implements
   the functions of the cool classes in C
   */

/* Class name strings */
const char Object_string[] = "Object";
const char String_string[] = "String";
const char Int_string[] = "Int";
const char Bool_string[] = "Bool";
const char IO_string[] = "IO";

const char default_string[] = "";

/* Class vtable prototypes */
extern const Object_vtable _Object_vtable_prototype;
extern const Int_vtable _Int_vtable_prototype;
extern const Bool_vtable _Bool_vtable_prototype;
extern const String_vtable _String_vtable_prototype;
extern const IO_vtable _IO_vtable_prototype;
/*
const Object_vtable _Object_vtable_prototype = {
    .type = 0,
    .size = sizeof(Object),
    .name = (char *)Object_string,
    .Object_new = Object_new,
    .Object_abort = Object_abort,
    .Object_type_name = Object_type_name,
    .Object_copy = Object_copy};
const Int_vtable _Int_vtable_prototype = {.type = 1,
                                          .size = sizeof(Int),
                                          .name = (char *)Int_string,
                                          .Object_new = Int_new,
                                          .Object_abort = Object_abort,
                                          .Object_type_name = Object_type_name,
                                          .Object_copy = Object_copy};
const Bool_vtable _Bool_vtable_prototype = {
    .type = 2,
    .size = sizeof(Bool),
    .name = (char *)Bool_string,
    .Object_new = Bool_new,
    .Object_abort = Object_abort,
    .Object_type_name = Object_type_name,
    .Object_copy = Object_copy};
const String_vtable _String_vtable_prototype = {
    .type = 3,
    .size = sizeof(String),
    .name = (char *)String_string,
    .Object_new = String_new,
    .Object_abort = Object_abort,
    .Object_type_name = Object_type_name,
    .Object_copy = Object_copy,
    .String_length = String_length,
    .String_concat = String_concat,
    .String_substr = String_substr};
const IO_vtable _IO_vtable_prototype = {.type = 4,
                                        .size = sizeof(IO),
                                        .name = (char *)IO_string,
                                        .Object_new = IO_new,
                                        .Object_abort = Object_abort,
                                        .Object_type_name = Object_type_name,
                                        .Object_copy = Object_copy,
                                        .IO_out_string = IO_out_string,
                                        .IO_out_int = IO_out_int,
                                        .IO_in_string = IO_in_string,
                                        .IO_in_int = IO_in_int};
*/

/*
// Methods in class object (only some are provided to you)
*/
Object *Object_abort(Object *self) {
  printf("Abort called from class %s\n",
         !self ? "Unknown" : self->vtblptr->name);
  exit(1);
  return self;
}

const String *Object_type_name(Object *self) {
  if (self == 0) {
    fprintf(stderr, "At %s(line %d): self is NULL\n", __FILE__, __LINE__);
    abort();
  }
  String *s = String_new();
  s->val = self->vtblptr->name;
  return s;
}

/* ADD CODE HERE FOR MORE METHODS OF CLASS OBJECT */

/*
// Methods in class IO (only some are provided to you)
*/

IO *IO_out_string(IO *self, String *s) {
  if (self == 0 || s == 0) {
    fprintf(stderr, "At %s(line %d): NULL object\n", __FILE__, __LINE__);
    abort();
  }
  printf("%s", s->val);
  return self;
}

IO *IO_out_int(IO *self, int x) {
  if (self == 0) {
    fprintf(stderr, "At %s(line %d): NULL object\n", __FILE__, __LINE__);
    abort();
  }
  printf("%d", x);
  return self;
}

/*
 * Get one line from stream using get_line(), then discard newline character.
 * Allocate string *in_string_p and store result there.
 * Return number of chars read.
 */
static int get_one_line(char **in_string_p, FILE *stream) {
  /* Get one line worth of input */
  size_t len = 0;
  ssize_t num_chars_read;
  num_chars_read = getline(in_string_p, &len, stdin);
  if (*in_string_p == 0) {
    fprintf(stderr, "At %s(line %d): allocation failed in IO::in_string()\n",
            __FILE__, __LINE__);
    exit(1);
  }

  /* Discard the newline char, if any.  It may not exist if EOF reached. */
  if (num_chars_read > 0 && (*in_string_p)[num_chars_read - 1] == '\n') {
    (*in_string_p)[num_chars_read - 1] = '\0';
    --len;
  }

  return len;
}

/*
 * The method IO::in_string(): String reads a string from
 * the standard input, up to but not including a newline character.
 */
String *IO_in_string(IO *self) {
  if (self == 0) {
    fprintf(stderr, "At %s(line %d): self is NULL\n", __FILE__, __LINE__);
    abort();
  }

  /* Get one line worth of input with the newline, if any, discarded */
  char *in_string = 0;
  ssize_t len = get_one_line(&in_string, stdin);
  assert(in_string);

  /* We can take advantage of knowing the internal layout of String objects */
  String *str = String_new();
  str->val = in_string;
  return str;
}

/*
 * The method IO::in_int(): Int reads a single integer, which may be preceded
 * by whitespace.
 * Any characters following the integer, up to and including the next newline,
 * are discarded by in_int.
 */
int IO_in_int(IO *self) {
  if (self == 0) {
    fprintf(stderr, "At %s(line %d): self is NULL\n", __FILE__, __LINE__);
    abort();
  }

  /* Get one line worth of input with the newline, if any, discarded */
  char *in_string = 0;
  ssize_t len = get_one_line(&in_string, stdin);
  assert(in_string);

  /* Now extract initial int and ignore the rest of the line */
  int x;
  int num_ints = 0;
  if (len) /* Discards initial spaces*/
    num_ints = sscanf(in_string, " %d", &x);

  /* If no text found, abort. */
  if (num_ints == 0) {
    fprintf(stderr,
            "At %s(line %d): Invalid integer on input in IO::in_int()\n",
            __FILE__, __LINE__);
    Object_abort((Object *)self);
  }
  return x;
}

/* ADD CODE HERE FOR MORE METHODS OF CLASS IO */
/* ADD CODE HERE FOR METHODS OF OTHER BUILTIN CLASSES */
Object *Object_new() {
  Object *ret = (Object *)malloc(sizeof(Object));
  ret->vtblptr = &_Object_vtable_prototype;
  return ret;
}
Object *Object_copy(Object *src) {
  Object *ret = (Object *)malloc(src->vtblptr->size);
  memcpy(ret, src, src->vtblptr->size);
  return ret;
}

Int *Int_new() {
  Int *ret = (Int *)malloc(sizeof(Int));
  ret->vtblptr = &_Int_vtable_prototype;
  ret->val = 0;
  return ret;
}

Bool *Bool_new() {
  Bool *ret = (Bool *)malloc(sizeof(Bool));
  ret->vtblptr = &_Bool_vtable_prototype;
  ret->val = false;
  return ret;
}

char *empty_string = (char *)"";
String *String_new() {
  String *ret = (String *)malloc(sizeof(String));
  ret->vtblptr = &_String_vtable_prototype;
  ret->val = empty_string;
  return ret;
}
int String_length(String *src) { return strlen(src->val); }
String *String_concat(String *src1, String *src2) {
  String *newstr = String_new();
  newstr->val =
      (char *)calloc(String_length(src1) + String_length(src2) + 1, 1);
  strcat(newstr->val, src1->val);
  strcat(newstr->val, src2->val);
  return newstr;
}
String *String_substr(String *src, int i1, int i2) {
  String *newstr = String_new();
  int length = String_length(src);
  if (length < i1 + i2) {
    fprintf(stderr, "At %s(line %d): substring index oob\n", __FILE__,
            __LINE__);
    fprintf(stderr, "Len: %d, i1: %d, i2: %d\n", length, i1, i2);
    fprintf(stderr, "string: %s\n", src->val);
    abort();
  }
  newstr->val = (char *)calloc(String_length(src), 1);
  newstr->val = strncpy(newstr->val, src->val + i1, i2);
  return newstr;
}

IO *IO_new() {
  IO *ret = (IO *)malloc(sizeof(IO));
  ret->vtblptr = &_IO_vtable_prototype;
  return ret;
}
Int *create_Int(int x) {
  Int *i = Int_new();
  i->val = x;
  return i;
}

Bool *create_Bool(bool x) {
  Bool *i = Bool_new();
  i->val = x;
  return i;
}
