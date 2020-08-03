#ifndef INPUT_LIBRARY_H
#define INPUT_LIBRARY_H

struct InputLibrary {
    int listening;
    int (*setup)();
    int (*cleanup)();
    void (*scan)(struct StenoEngine* engine);
};

#endif
