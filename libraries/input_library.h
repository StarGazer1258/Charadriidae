#ifndef INPUT_LIBRARY_H
#define INPUT_LIBRARY_H

struct InputLibrary {
    void (*on_stroke)(char* stroke);
};

#endif
