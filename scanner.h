#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#pragma once

enum Token {
    tok_eof =   -1,

    // commands
    tok_def =   -2,
    tok_extern  =   -3,

    // primary
    tok_identifier  =   -4,
    tok_number  =   -5,
};

static std::string IdentifierStr;   // Filled in if tok_identifier
static double NumVal;

extern int gettok();