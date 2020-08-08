#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "translation.h"

#include "../util/minmax.h"
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

void on_stroke(struct StenoEngine* engine, char* stroke) {
    struct Translation translation = {};
    
    if(strlen(stroke) == 0) return;

    if(!strcmp(stroke, "*")) {
        struct Translation prev = *pop_translation_history(engine);
        translation.stroke = AST;
        translation.backspace = strlen(prev.translation);
    } else {
        for(int i = *engine->dictionary_manager.longest_translation; i > 0; i--) {
            if(peek_translation_history(engine, i)->stroke == NULL) continue;
            
            char* history_stroke = malloc(24 * (i + 1) + 1);

            strcpy(history_stroke, "");
            for(int h = i; h > 0; h--) {
                strcat(history_stroke, peek_translation_history(engine, h)->stroke);
                strcat(history_stroke, "/");
            }

            strcat(history_stroke, stroke);

            /* logging.logf(INFO, "History stroke: %s\n", history_stroke); */

            struct Translation history_translation = engine->dictionary_manager.translate_stroke(history_stroke);

            if(strlen(history_translation.translation) > 0) {
                /*
                    1. Compare the last translation and current to determine # or backspaces needed
                    2. *                                                   * needed replacement characters
                */

                char* past_translation = malloc(i * (sizeof(history_translation.translation) + 1));
                strcpy(past_translation, "");
                
                for(int n = i; n > 0; n--) {
                    struct Translation nth_translation = *pop_translation_history(engine);
                    strcat(past_translation, nth_translation.translation);
                    strcat(past_translation, " ");
                }
                
                
                for(int c = 0; c < MIN(strlen(past_translation), strlen(history_translation.translation)); c++) {
                    
                    if(past_translation[c] != history_translation.translation[c]) {
                        history_translation.stroke = malloc(sizeof(history_stroke));
                        strcpy(history_translation.stroke, history_stroke);
                        history_translation.backspace = strlen(past_translation) - (c + 1);
                        break;
                    }
                }
                
                push_translation_history(engine, history_translation);
                for(int i = 0; i < engine->n_output_libraries; i++) {
                    engine->output_libraries[i].output(history_translation);
                }

                free(history_stroke);
                return;
            }

            free(history_stroke);
        }

        translation = engine->dictionary_manager.translate_stroke(stroke);
        if(strlen(translation.translation) == 0) {
            strcpy(translation.translation, stroke);
            strcat(translation.translation, " ");
        }
        translation.stroke = malloc(sizeof(translation.stroke));
        strcpy(translation.stroke, stroke);

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

struct Translation* peek_translation_history(struct StenoEngine* engine, unsigned go_back_n) {
    struct Translation* ret;

    unsigned back = go_back_n % (sizeof(engine->translation_history)/sizeof(engine->translation_history[0]));

    if(engine->translation_history_index < back) {
        ret = &engine->translation_history[(sizeof(engine->translation_history) / sizeof(engine->translation_history[0]) - 1) - (back - engine->translation_history_index)];
    } else {
        ret = &engine->translation_history[engine->translation_history_index - back];
    }

    return ret;
}

struct Translation* pop_translation_history(struct StenoEngine* engine) {
    if(engine->translation_history_index == 0) {
        engine->translation_history_index = sizeof(engine->translation_history) / sizeof(engine->translation_history[0]) - 1;
    } else {
        engine->translation_history_index--;
    }

    return &engine->translation_history[engine->translation_history_index];
}