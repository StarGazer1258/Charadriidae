#include "engine.h"
#include "translation.h"

#include "../libraries/dictionary_manager.h"

int start_engine(struct StenoEngine* engine) {
    if(!engine->running) {
        printf("Starting stenography engine...\n");
        engine->running = 1;
    } else {
        printf("Stenography engine already running.\n");
    }

    return engine->running;
}

int stop_engine(struct StenoEngine* engine) {
    if(engine->running) {
        printf("Stopping stenography engine...\n");
        engine->running = 0;
    } else {
        printf("Stenography engine already stopped.\n");
    }

    return !engine->running;
}

int load_dictionary_manager(struct StenoEngine* engine, struct DictionaryManager dictionary_manager) {
    printf("Loading dictionary manager...\n");
    engine->dictionary_manager = dictionary_manager;
    if (engine->dictionary_manager.load_dictionary()) {
        printf("Dictionary manger loaded successfully.\n");
        return 1;
    } else {
        printf("Error loading dictionary manager.\n");
        return 0;
    }
}

int load_input_library(struct StenoEngine* engine, struct InputLibrary input_library) {
    printf("Loading input library %d...\n", engine->n_input_libraries + 1);
    if(engine->n_input_libraries < sizeof(engine->input_libraries)/sizeof(engine->input_libraries[0])) {
        engine->input_libraries[engine->n_input_libraries] = input_library;
        engine->n_input_libraries++;
    } else {
        printf("Error loading input library %d: Max # of input libraries exceeded.\n", engine->n_input_libraries + 1);
        return 0;
    }

    printf("Input library %d loaded successfully.\n", engine->n_input_libraries);
    return 1;
}

int load_output_library(struct StenoEngine* engine, struct OutputLibrary output_library) {
    printf("Loading output library %d...\n", engine->n_output_libraries + 1);
    if(engine->n_output_libraries < sizeof(engine->output_libraries)/sizeof(engine->output_libraries[0])) {
        engine->output_libraries[engine->n_output_libraries] = output_library;
        engine->n_output_libraries++;
    } else {
        printf("Error loading output library %d: Max # of output libraries exceeded.\n", engine->n_output_libraries + 1);
        return 0;
    }

    printf("Output library %d loaded successfully.\n", engine->n_output_libraries);
    return 1;
}

void on_stroke(struct StenoEngine engine, char* stroke) {
    struct Translation translation = engine.dictionary_manager.translate_stroke(stroke);
    for(int i = 0; i < sizeof(engine.output_libraries); i++) { 
        /* output(engine.output_libraries[i], translation); */
    }
}