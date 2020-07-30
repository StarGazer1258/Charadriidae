#include "engine.h"
#include "translation.h"
#include "../util/logging.h"

#include "../libraries/dictionary_manager.h"

int start_engine(struct StenoEngine* engine) {
    if(!engine->running) {
        logging.log(INFO, "Starting stenography engine...\n");
        engine->running = 1;
    } else {
        logging.log(INFO, "Stenography engine already running.\n");
    }

    return engine->running;
}

int stop_engine(struct StenoEngine* engine) {
    if(engine->running) {
        logging.log(INFO, "Stopping stenography engine...\n");
        engine->running = 0;
    } else {
        logging.log(ERROR, "Stenography engine already stopped.\n");
    }

    return !engine->running;
}

int load_dictionary_manager(struct StenoEngine* engine, struct DictionaryManager dictionary_manager) {
    logging.log(INFO, "Loading dictionary manager...\n");
    engine->dictionary_manager = dictionary_manager;
    if (engine->dictionary_manager.load_dictionary()) {
        logging.log(INFO, "Dictionary manger loaded successfully.\n");
        return 1;
    } else {
        logging.log(ERROR, "Error loading dictionary manager.\n");
        return 0;
    }
}

int load_input_library(struct StenoEngine* engine, struct InputLibrary input_library) {
    logging.logf(INFO, "Loading input library %d...\n", engine->n_input_libraries + 1);
    if(engine->n_input_libraries < sizeof(engine->input_libraries)/sizeof(engine->input_libraries[0])) {
        engine->input_libraries[engine->n_input_libraries] = input_library;
        engine->n_input_libraries++;
    } else {
        logging.logf(ERROR, "Error loading input library %d: Max # of input libraries exceeded.\n", engine->n_input_libraries + 1);
        return 0;
    }

    logging.logf(INFO, "Input library %d loaded successfully.\n", engine->n_input_libraries);
    return 1;
}

int load_output_library(struct StenoEngine* engine, struct OutputLibrary output_library) {
    logging.logf(INFO, "Loading output library %d...\n", engine->n_output_libraries + 1);
    if(engine->n_output_libraries < sizeof(engine->output_libraries)/sizeof(engine->output_libraries[0])) {
        engine->output_libraries[engine->n_output_libraries] = output_library;
        engine->n_output_libraries++;
    } else {
        logging.logf(ERROR, "Error loading output library %d: Max # of output libraries exceeded.\n", engine->n_output_libraries + 1);
        return 0;
    }

    logging.logf(INFO, "Output library %d loaded successfully.\n", engine->n_output_libraries);
    return 1;
}

void on_stroke(struct StenoEngine engine, char* stroke) {
    struct Translation translation = engine.dictionary_manager.translate_stroke(stroke);
    for(int i = 0; i < sizeof(engine.output_libraries); i++) { 
        /* output(engine.output_libraries[i], translation); */
    }
}