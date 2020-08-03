#include <stdio.h>
#include <string.h>

#include "/usr/local/include/json-c/json.h"
#include "../../src/engine/stroke.h"
#include "../../src/libraries/dictionary_manager.h"
#include "../../src/util/logging.h"

json_object* json_dictionary;

struct Translation json_translate_stroke(Stroke stroke) {

    struct Translation translation = {};
    char lookup[23] = "";

    if(stroke & NUM) strcat(lookup, "#");

    if(!(stroke & (S1 + T1 + K + P1 + W + H + R1 + A + O + AST + E + U))) strcat(lookup, "-");

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

    logging.logf(DEBUG, "%s\n", lookup);

    strcat(translation.translation, json_object_get_string(json_object_object_get(json_dictionary, lookup)));

    logging.logf(DEBUG, "%s\n", translation.translation);

    translation.stroke = stroke;

    return translation;
}

int json_load_dictionary() {
    logging.log(DEBUG, "Loading JSON dictionary...\n");

    json_dictionary = json_object_from_file("dictionaries/main.json");

    /* logging.logf(DEBUG, "%s\n", json_object_to_json_string(json_dictionary)); */

    return 1;
}

int setup_json_dictionary_manager(struct DictionaryManager* dictionary_manager) {
    dictionary_manager->translate_stroke = json_translate_stroke;
    dictionary_manager->load_dictionary = json_load_dictionary;

    return 1;
}