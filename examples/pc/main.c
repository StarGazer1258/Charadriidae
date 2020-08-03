#include <stdio.h>

#include "../../src/engine/engine.h"
#include "../../src/util/logging.h"

int main() {
    struct StenoEngine engine = {0, 0, 0, 0, 0};
    struct DictionaryManager dictionary_manager;
    struct InputLibrary input_library;
    struct OutputLibrary output_library;

    /* Setup and load the dictionary manager */
    setup_json_dictionary_manager(&dictionary_manager);
    load_dictionary_manager(&engine, dictionary_manager);

    /* Setup and load the input library */
    setup_keyboard_input_library(&input_library);
    load_input_library(&engine, input_library);
    
    /* Setup and load the output library */
    setup_console_output_library(&output_library);
    load_output_library(&engine, output_library);

    /* Start the stenography engine */
    start_engine(&engine);

    /* Stop the stenography engine */
    stop_engine(&engine);
}