#include <stdio.h>
#include <string.h>

#include "/usr/local/include/json-c/json.h"
#include "../../src/engine/stroke.h"
#include "../../src/libraries/dictionary_manager.h"
#include "../../src/util/logging.h"

int json_longest_translation = 0;
json_object* json_dictionary;

char* json_stroke_to_char(int stroke) {
    char* lookup = malloc(24);
    strcpy(lookup, "");

    if(stroke & NUM) strcat(lookup, "#");

    if(!(stroke & (NUM + S1 + T1 + K + P1 + W + H + R1 + A + O + AST + E + U)) && stroke > 0) strcat(lookup, "-");

    if(stroke & S1) strcat(lookup, "S");
    if(stroke & T1) strcat(lookup, "T");
    if(stroke & K) strcat(lookup, "K");
    if(stroke & P1) strcat(lookup, "P");
    if(stroke & W) strcat(lookup, "W");
    if(stroke & H) strcat(lookup, "H");
    if(stroke & R1) strcat(lookup, "R");
    if(stroke & A) strcat(lookup, "A");
    if(stroke & O) strcat(lookup, "O");
    if(stroke & AST) strcat(lookup, "*");
    if(stroke & E) strcat(lookup, "E");
    if(stroke & U) strcat(lookup, "U");
    if(stroke & F) strcat(lookup, "F");
    if(stroke & R2) strcat(lookup, "R");
    if(stroke & P2) strcat(lookup, "P");
    if(stroke & B) strcat(lookup, "B");
    if(stroke & L) strcat(lookup, "L");
    if(stroke & G) strcat(lookup, "G");
    if(stroke & T2) strcat(lookup, "T");
    if(stroke & S2) strcat(lookup, "S");
    if(stroke & D) strcat(lookup, "D");
    if(stroke & Z) strcat(lookup, "Z");

    return lookup;
}

struct Translation json_translate_stroke(char* stroke) {

    struct Translation translation = {};

    logging.logf(DEBUG, "%s\n", stroke);

    char* result = json_object_get_string(json_object_object_get(json_dictionary, stroke));
    if(result != NULL) {
        strcat(translation.translation, result);
        strcat(translation.translation, " ");
    }

    logging.logf(DEBUG, "%s\n", translation.translation);

    return translation;
}

int json_load_dictionary() {
    logging.log(DEBUG, "Loading JSON dictionary...\n");

    json_dictionary = json_object_from_file("dictionaries/main.json");

    json_object_object_foreach(json_dictionary, key, val) {
        int stroke_count = 0;
        for(int i = 0; i < strlen(key); i++) {
            if(key[i] == '/') stroke_count++;
        }
        if(stroke_count > json_longest_translation) json_longest_translation = stroke_count;
    }

    logging.logf(DEBUG, "Longest stroke: %d\n", json_longest_translation);

    return 1;
}

int setup_json_dictionary_manager(struct DictionaryManager* dictionary_manager) {
    dictionary_manager->longest_translation = &json_longest_translation;
    dictionary_manager->stroke_to_char = json_stroke_to_char;
    dictionary_manager->translate_stroke = json_translate_stroke;
    dictionary_manager->load_dictionary = json_load_dictionary;

    return 1;
}