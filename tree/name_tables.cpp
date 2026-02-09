#include "tree.h"

//--------------------------------------------------------------------------------

const char* kKeywords[] = 
{
    "if"      ,
    "else"    ,
    "while"   ,
    ";"       ,
    ","       ,
    "("       ,
    ")"       ,
    "{"       ,
    "}"       ,
    "announce",
    "def"     ,
};

//--------------------------------------------------------------------------------

const char* kMathFunctions[] = {
    "sqrt"  ,
    "log"   ,
    "sin"   ,
    "cos"   ,
    "tg"    ,
    "ctg"   ,
    "arcsin",
    "arccos",
    "arctg" ,
    "arcctg",
    "sh"    ,
    "ch"    ,
};

//--------------------------------------------------------------------------------

const char* kMathOperations[] = {
    "+" ,
    "-" ,
    "*" ,
    "/" ,
    "^" ,
    "=" ,
    "==",
    "!=",
    ">" ,
    ">=",
    "<" ,
    "<=",
    "and",
    "or"
};

//--------------------------------------------------------------------------------
const int kNumOfKeyWords = sizeof (kKeywords      ) / sizeof (*kKeywords      );
const int kNumOfMathOper = sizeof (kMathOperations) / sizeof (*kMathOperations);
const int kNumOfMathFunc = sizeof (kMathFunctions ) / sizeof (*kMathFunctions );

//--------------------------------------------------------------------------------