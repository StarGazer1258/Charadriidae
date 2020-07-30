#ifndef DICTIONARY_MANAGER_H
#define DICTIONARY_MANAGER_H

#include "../engine/translation.h"

struct DictionaryManager {
    struct Translation (*translate_stroke)(char* stroke);
    int (*load_dictionary)();
};

/*struct Translation translate_stroke(struct DictionaryManager dictionary_manager, char* stroke);*/

#endif