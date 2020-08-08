#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "stroke.h"

struct Translation {
    char* stroke;

    unsigned int backspace;
    char* translation[141]; /* 140 characters, in case you want to brief a tweet. + 1 for a space */
};

#endif