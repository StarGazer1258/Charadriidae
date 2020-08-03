#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "stroke.h"

struct Translation {
    Stroke stroke;

    unsigned int backspace;
    char* translation[140]; /* 140 characters, in case you want to brief a tweet. */
};

#endif