#include "../libraries/output_library.h"

int teensy_console_output(struct Translation translation) {

}

void setup_teensy_console_output_library(struct OutputLibrary* output_library) {
    output_library->output = teensy_console_output;
}