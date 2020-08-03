#ifndef OUTPUT_LIBRARY_H
#define OUTPUT_LIBRARY_H

#include "../engine/translation.h"

struct OutputLibrary {
    void (*output)(struct Translation translation);
};

#endif
