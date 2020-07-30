#include <stdio.h>

#include "engine/engine.h"

#include "json_dictionary_manager.h"

int main() {
    struct StenoEngine engine = {0, 0, 0, 0};
    struct DictionaryManager dictionary_manager;
    struct InputLibrary input_library;
    struct OutputLibrary output_library;

    /* Set the dictionary manager methods */
    setup_json_dictionary_manager(&dictionary_manager);
    
    /* Load the needed libraries */
    load_dictionary_manager(&engine, dictionary_manager);
    load_input_library(&engine, input_library);
    load_output_library(&engine, output_library);

    /* Start the stenography engine */
    start_engine(&engine);

    /* Stop the stenography engine */
    stop_engine(&engine);
}