/* Flex 文件通常分为三部分：
 * 1. 定义部分（ %{...%} 之间）
 * 2. 正则表达式定义部分
 * 3. 规则部分
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

/* 定义注释嵌套计数 */
int comment_level = 0;

%}

%x COMMENT STRING

%option noyywrap



/*
 * Define names for regular expressions here.
 */

digit       [0-9]
lowercase   [a-z]
uppercase   [A-Z]
letter      ({lowercase}|{uppercase})
id          ({letter}|_)({letter}|{digit}|_)*
number      {digit}+
whitespace  [ \t\f\r\v]+
newline     \n|\r\n


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

/* 忽略空白字符 */
{whitespace}            { /* Ignore whitespace */ }

/* 处理换行符并更新行号 */
{newline}               { curr_lineno++; }

/* 注释（包括嵌套注释） */
"(*"                    { comment_level = 1; BEGIN(COMMENT); }
<COMMENT>"(*"           { comment_level++; }
<COMMENT>"*)"           { comment_level--; if (comment_level == 0) BEGIN(INITIAL); }
<COMMENT>{newline}      { curr_lineno++; }
<COMMENT>.|\n           ;

/* 在 COMMENT 状态下遇到 EOF 时，提示注释未结束 */
<COMMENT><<EOF>> {
    fprintf(stderr, "Unterminated comment at line %d\n", curr_lineno);
    exit(1);
}


/* 字符串常量 */
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
<STRING>\\(.)           { *string_buf_ptr++ = yytext[1]; }
<STRING>{newline}       {
                            curr_lineno++;
                            fprintf(stderr, "Unterminated string constant at line %d\n", curr_lineno);
                            exit(1);
                        }
<STRING><<EOF>>         {
                            fprintf(stderr, "EOF in string constant at line %d\n", curr_lineno);
                            exit(1);
                        }
<STRING>.               {
                            *string_buf_ptr++ = yytext[0];
                            if (string_buf_ptr - string_buf >= MAX_STR_CONST) {
                                fprintf(stderr, "String constant too long at line %d\n", curr_lineno);
                                exit(1);
                            }
                        }

/* 类型标识符（首字母大写） */
{uppercase}({letter}|{digit}|_)*  {
                                    cool_yylval.symbol = stringtable.add_string(yytext);
                                    return TYPEID;
                                  }

/* 关键字和标识符 */
{id}   {
       /* 检查是否为关键字 */
       if (strcasecmp(yytext, "class") == 0) return CLASS;
       else if (strcasecmp(yytext, "else") == 0) return ELSE;
       else if (strcasecmp(yytext, "fi") == 0) return FI;
       else if (strcasecmp(yytext, "if") == 0) return IF;
       else if (strcasecmp(yytext, "in") == 0) return IN;
       else if (strcasecmp(yytext, "inherits") == 0) return INHERITS;
       else if (strcasecmp(yytext, "let") == 0) return LET;
       else if (strcasecmp(yytext, "loop") == 0) return LOOP;
       else if (strcasecmp(yytext, "pool") == 0) return POOL;
       else if (strcasecmp(yytext, "then") == 0) return THEN;
       else if (strcasecmp(yytext, "while") == 0) return WHILE;
       else if (strcasecmp(yytext, "case") == 0) return CASE;
       else if (strcasecmp(yytext, "esac") == 0) return ESAC;
       else if (strcasecmp(yytext, "of") == 0) return OF;
       else if (strcasecmp(yytext, "new") == 0) return NEW;
       else if (strcasecmp(yytext, "isvoid") == 0) return ISVOID;
       else if (strcasecmp(yytext, "not") == 0) return NOT;
       else if (strcmp(yytext, "true") == 0) { cool_yylval.boolean = 1; return BOOL_CONST; }
       else if (strcmp(yytext, "false") == 0) { cool_yylval.boolean = 0; return BOOL_CONST; }
       else {
            cool_yylval.symbol = stringtable.add_string(yytext);
            return OBJECTID;
            }
       }


/* 整数常量 */
{number} {
         cool_yylval.symbol = inttable.add_string(yytext);
         return INT_CONST;
         }

/* 操作符和标点符号 */
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

/* 非法字符 */
.                       {
                            fprintf(stderr, "Illegal character '%s' at line %d\n", yytext, curr_lineno);
                            /* 根据需求选择继续或退出 */
                        }


%%
