#ifndef TRANSLATION_H
#define TRANSLATION_H

struct Translation {
    char* stroke[23];

    unsigned int backspace;
    char* translation[140]; /* 140 characters, in case you want to brief a tweet. */
};

#endif