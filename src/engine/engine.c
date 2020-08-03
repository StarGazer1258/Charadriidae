#include <string.h>

#include "engine.h"
#include "translation.h"
#include "../util/logging.h"

#include "../libraries/dictionary_manager.h"

int start_engine(struct StenoEngine* engine) {
    if(!engine->running) {
        logging.log(INFO, "Starting stenography engine...\n");
        engine->running = 1;
    } else {
        logging.log(ERROR, "Stenography engine already running.\n");
    }
    
    

    while(engine->running) {
        for(int i = 0; i < engine->n_input_libraries; i++) {
            engine->input_libraries[i].scan(engine);
        }
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
        engine->input_libraries[engine->n_input_libraries].setup();
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

void print_stroke_bin(Stroke stroke) {
    for(unsigned i = 1; i <= (1 << 22); i *= 2) {
        printf("%d", (stroke & i) ? 1 : 0);
    }
    printf("\n");
}

void on_stroke(struct StenoEngine* engine, Stroke stroke) {
    struct Translation translation = {};
    
    if(stroke == AST) {
        struct Translation prev = pop_translation_history(engine);
        translation.stroke = AST;
        translation.backspace = strlen(prev.translation);
    } else {
        translation = engine->dictionary_manager.translate_stroke(stroke);
        push_translation_history(engine, translation);
    }

    for(int i = 0; i < engine->n_output_libraries; i++) {
        engine->output_libraries[i].output(translation);
    }
}

void push_translation_history(struct StenoEngine* engine, struct Translation translation) {
    engine->translation_history[engine->translation_history_index] = translation;

    if(engine->translation_history_index == sizeof(engine->translation_history) / sizeof(engine->translation_history[0]) - 1) {
        engine->translation_history_index = 0;
    } else {
        engine->translation_history_index++;
    }
}

struct Translation peek_translation_history(struct StenoEngine* engine) {
    struct Translation ret;

    if(engine->translation_history_index == 0) {
        ret = engine->translation_history[sizeof(engine->translation_history) / sizeof(engine->translation_history[0]) - 1];
    } else {
        ret = engine->translation_history[engine->translation_history_index - 1];
    }

    return ret;
}

struct Translation pop_translation_history(struct StenoEngine* engine) {
    if(engine->translation_history_index == 0) {
        engine->translation_history_index = sizeof(engine->translation_history) / sizeof(engine->translation_history[0]) - 1;
    } else {
        engine->translation_history_index--;
    }

    return engine->translation_history[engine->translation_history_index];
}