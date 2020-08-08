#ifndef DICTIONARY_MANAGER_H
#define DICTIONARY_MANAGER_H

#include "../engine/translation.h"

struct DictionaryManager {
    unsigned* longest_translation;
    char* (*stroke_to_char)(int stroke);
    struct Translation (*translate_stroke)(char* stroke);
    int (*load_dictionary)();
};

#endif