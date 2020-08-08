#include <string.h>

#include "../../src/engine/engine.h"
#include "../../src/libraries/output_library.h"
#include "../../src/util/logging.h"

void console_output(struct Translation translation) {

    char backsp[sizeof(translation.translation) * (sizeof(((struct StenoEngine*)0)->translation_history) / sizeof(((struct StenoEngine*)0)->translation_history[0]))] = "";

    for(int i = 0; i < translation.backspace; i++) {
         strcat(backsp, "\b \b");
    }

    logging.logf(INFO, "%s%s", backsp, translation.translation);
}

int setup_console_output_library(struct OutputLibrary* output_library) {
    output_library->output = console_output;

    return 1;
}