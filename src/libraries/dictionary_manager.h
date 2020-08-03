#ifndef DICTIONARY_MANAGER_H
#define DICTIONARY_MANAGER_H

#include "../engine/translation.h"

struct DictionaryManager {
    struct Translation (*translate_stroke)(Stroke stroke);
    int (*load_dictionary)();
};

#endif