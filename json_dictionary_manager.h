#include <stdio.h>
#include "libraries/dictionary_manager.h"

struct Translation json_translate_stroke(char* stroke) {

}

int json_load_dictionary() {
    printf("Loading JSON dictionary...\n");

    return 1;
}

void setup_json_dictionary_manager(struct DictionaryManager* dictionary_manager) {
    dictionary_manager->translate_stroke = json_translate_stroke;
    dictionary_manager->load_dictionary = json_load_dictionary;
}