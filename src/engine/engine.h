#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>

#include "stroke.h"
#include "../libraries/dictionary_manager.h"
#include "../libraries/input_library.h"
#include "../libraries/output_library.h"

struct StenoEngine {
    unsigned running;
    
    unsigned n_input_libraries;
    unsigned n_output_libraries;
    unsigned n_translation_history;

    unsigned translation_history_index;
    
    struct DictionaryManager dictionary_manager;
    struct InputLibrary input_libraries[5];
    struct OutputLibrary output_libraries[5];
    struct Translation translation_history[100];
};

int start_engine(struct StenoEngine* engine);
int stop_engine(struct StenoEngine* engine);

void on_stroke(struct StenoEngine* engine, Stroke stroke);

int load_dictionary_manager(struct StenoEngine* engine, struct DictionaryManager dictionary_manager);

int load_input_library(struct StenoEngine* engine, struct InputLibrary input_library);
int unload_input_library(struct StenoEngine* engine, struct InputLibrary input_library);

int load_output_library(struct StenoEngine* engine, struct OutputLibrary output_library);
int unload_output_library(struct StenoEngine* engine, struct OutputLibrary output_library);

void push_translation_history(struct StenoEngine* engine, struct Translation translation);
struct Translation peek_translation_history(struct StenoEngine* engine);
struct Translation pop_translation_history(struct StenoEngine* engine);

#endif