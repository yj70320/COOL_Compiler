/* three parts of flex file：
 * 1. define（ %{...%} ）
 * 2. regular expression
 * 3. rules
 */

/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <string>
#include <unordered_set>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
  if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
    YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */

/* 定义注释嵌套级别 */
int comment_level = 0;

bool is_keyword(const std::string& id_lower) {
    static const std::unordered_set<std::string> keywords = {
        "class", "Class", "else", "fi", "if", "in", "inherits", "let",
        "loop", "pool", "then", "while", "case", "esac", "of",
        "new", "isvoid", "not"
    };
    return keywords.find(id_lower) != keywords.end();
}

int get_keyword_token(const std::string& id_lower) {
    if (id_lower == "class") return CLASS;
    else if (id_lower == "Class") return CLASS;
    else if (id_lower == "else") return ELSE;
    else if (id_lower == "fi") return FI;
    else if (id_lower == "if") return IF;
    else if (id_lower == "in") return IN;
    else if (id_lower == "inherits") return INHERITS;
    else if (id_lower == "let") return LET;
    else if (id_lower == "loop") return LOOP;
    else if (id_lower == "pool") return POOL;
    else if (id_lower == "then") return THEN;
    else if (id_lower == "while") return WHILE;
    else if (id_lower == "case") return CASE;
    else if (id_lower == "esac") return ESAC;
    else if (id_lower == "of") return OF;
    else if (id_lower == "new") return NEW;
    else if (id_lower == "isvoid") return ISVOID;
    else if (id_lower == "not") return NOT;
    else return 0; // 不应到达这里
}

%}

%x COMMENT STRING

%option noyywrap


/*
 * Define names for regular expressions here.
 */

digit            [0-9]
lowercase        [a-z]
uppercase        [A-Z]
letter           ({lowercase}|{uppercase})
letter_or_digit  ({letter}|{digit}|_)
id_start         ({letter}|_)
id_continue      ({letter}|{digit}|_)
id               {id_start}{id_continue}+
number           {digit}+
whitespace       [ \t\f\r\v]+
newline          (\r\n|\n|\r)

%%

 /*
  * Define regular expressions for the tokens of COOL here. Make sure, you
  * handle correctly special cases, like:
  *   - Nested comments
  *   - String constants: They use C like systax and can contain escape
  *     sequences. Escape sequence \c is accepted for all characters c. Except
  *     for \n \t \b \f, the result is c.
  *   - Keywords: They are case-insensitive except for the values true and
  *     false, which must begin with a lower-case letter.

  *   - Multiple-character operators (like <-): The scanner should produce a
  *     single token for every such operator.
  *   - Line counting: You should keep the global variable curr_lineno updated
  *     with the correct line number
  */


{whitespace}            {  }
{newline}               { curr_lineno++; }


"(*"                    { comment_level = 1; BEGIN(COMMENT); }
<COMMENT>\(\*           { comment_level++; }
<COMMENT>\*\)           {
                            comment_level--;
                            if (comment_level == 0) BEGIN(INITIAL);
                        }
<COMMENT>{newline}      { curr_lineno++; }
<COMMENT>[^(\*\n]+      {  }
<COMMENT>.              {  }

<COMMENT><<EOF>>        {
                            if (comment_level > 0)
                            {
                                yylval.error_msg = "EOF in comment";
                                BEGIN(INITIAL);
                                return ERROR;
                            }
                        }

"--"[^\n]*              {  }



\"                      { string_buf_ptr = string_buf; BEGIN(STRING); } 
<STRING>\"              {
                            *string_buf_ptr = '\0';
                            cool_yylval.symbol = stringtable.add_string(string_buf);
                            BEGIN(INITIAL);
                            return STR_CONST;
                        }
<STRING>\\n             { *string_buf_ptr++ = '\n'; }
<STRING>\\t             { *string_buf_ptr++ = '\t'; }
<STRING>\\b             { *string_buf_ptr++ = '\b'; }
<STRING>\\f             { *string_buf_ptr++ = '\f'; }
<STRING>\\\\            { *string_buf_ptr++ = '\\'; }
<STRING>\\\"            { *string_buf_ptr++ = '\"'; }
<STRING>\\[^\n]         { *string_buf_ptr++ = yytext[1]; }
<STRING>{newline}       {
                            curr_lineno++;
                            yylval.error_msg = "Unterminated string constant";
                            return ERROR;
                        }
<STRING>\0              {
                            yylval.error_msg = "String contains null character";
                            return ERROR;
                        }
<STRING><<EOF>>         {
                            yylval.error_msg = "EOF in string constant";
                            return ERROR;
                        }
<STRING>.               {
                            *string_buf_ptr++ = yytext[0];
                            if (string_buf_ptr - string_buf >= MAX_STR_CONST) {
                                yylval.error_msg = "String constant too long";
                                return ERROR;
                            }
                        }



"class"                 { return CLASS; }
"Class"                 { return CLASS; }
"else"                  { return ELSE; }
"fi"                    { return FI; }
"if"                    { return IF; }
"in"                    { return IN; }
"inherits"              { return INHERITS; }
"let"                   { return LET; }
"loop"                  { return LOOP; }
"pool"                  { return POOL; }
"then"                  { return THEN; }
"while"                 { return WHILE; }
"case"                  { return CASE; }
"esac"                  { return ESAC; }
"of"                    { return OF; }
"new"                   { return NEW; }
"isvoid"                { return ISVOID; }
"not"                   { return NOT; }
"true"                  { cool_yylval.boolean = 1; return BOOL_CONST; }
"false"                 { cool_yylval.boolean = 0; return BOOL_CONST; }

{uppercase}{id_continue}*  {
                        //    printf("TYPEID matched: %s\n", yytext);
                           cool_yylval.symbol = idtable.add_string(yytext);
                           return TYPEID;
                           }

{lowercase}{id_continue}* {
                        //   printf("OBJECTID matched: %s\n", yytext);
                          cool_yylval.symbol = idtable.add_string(yytext);
                          return OBJECTID;
                          }

{number} {
             cool_yylval.symbol = inttable.add_string(yytext);
             return INT_CONST;
         }


"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"~"                     { return '~'; }
"="                     { return '='; }
"<="                    { return LE; }
"<"                     { return '<'; }
"("                     { return '('; }
")"                     { return ')'; }
"{"                     { return '{'; }
"}"                     { return '}'; }
";"                     { return ';'; }
":"                     { return ':'; }
","                     { return ','; }
"@"                     { return '@'; }
"."                     { return '.'; }
"<-"                    { return ASSIGN; }
"=>"                    { return DARROW; }



.                       {
                            char buffer[256];
                            snprintf(buffer, sizeof(buffer), "Illegal character: %c", yytext[0]);
                            yylval.error_msg = strdup(buffer);
                            return ERROR;
                        }


%%

