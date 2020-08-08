#ifndef KEYBOARD_INPUT_LIBRARY_H
#define KEYBOARD_INPUT_LIBRARY_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>

#include "../../src/engine/engine.h"
#include "../../src/engine/stroke.h"
#include "../../src/libraries/input_library.h"
#include "../../src/util/logging.h"

#define KEY_RELEASED 0x00
#define KEY_PRESSED 0x01
#define KEY_HELD 0x02

int kbd;
fd_set set;
struct timeval select_timeout;
struct input_event event;
int keys_held = 0;
int stroking = 0;
int keyboard_stroke;

int keyboard_setup() {
    kbd = open("/dev/input/event3", O_RDONLY);
    
    sleep(1);

    if(ioctl(kbd, EVIOCGRAB, 1) == 0) {
        logging.log(DEBUG, "Keyboard device grabbed.\n");
    } else {
        logging.log(ERROR, "Failed to grab keyboard device.\n");
        keyboard_cleanup();
        return 0;
    }

    return 1;
}

int keyboard_cleanup() {
    if(ioctl(kbd, EVIOCGRAB, 0) == 0) {
        logging.log(DEBUG, "Keyboard device released.\n");
    } else {
        return 0;
    }
    
    close(kbd);

    return 1;
}

void keyboard_scan(struct StenoEngine* engine) {

    FD_ZERO(&set);
    FD_SET(kbd, &set);

    select_timeout.tv_sec = 0;
    select_timeout.tv_usec = 1000;

    if(select(kbd + 1, &set, NULL, NULL, &select_timeout)) {
        read(kbd, &event, sizeof(event));

        logging.log(DEBUG, "Reading event...\n");

        if(event.type == EV_KEY) {
            logging.log(DEBUG, "Found a key event.\n");
            switch(event.value) {
                case KEY_RELEASED:
                    keys_held--;
                    logging.logf(DEBUG, "Key released. %d keys held.\n", keys_held);
                    break;
                case KEY_PRESSED:
                    keys_held++;
                    logging.logf(DEBUG, "Key pressed. %d keys held.\n", keys_held);
                    stroking = 1;
                    
                    switch(event.code) {
                        case KEY_ESC:
                            if(keyboard_cleanup()) exit(0);
                        case KEY_1:
                        case KEY_2:
                        case KEY_3:
                        case KEY_4:
                        case KEY_5:
                        case KEY_6:
                        case KEY_7:
                        case KEY_8:
                        case KEY_9:
                        case KEY_0:
                            if(!(keyboard_stroke & NUM)) keyboard_stroke += NUM;
                            break;
                        case KEY_Q:
                        case KEY_A:
                            if(!(keyboard_stroke & S1)) keyboard_stroke += S1;
                            break;
                        case KEY_W:
                            if(!(keyboard_stroke & T1)) keyboard_stroke += T1;
                            break;
                        case KEY_S:
                            if(!(keyboard_stroke & K)) keyboard_stroke += K;
                            break;
                        case KEY_E:
                            if(!(keyboard_stroke & P1)) keyboard_stroke += P1;
                            break;
                        case KEY_D:
                            if(!(keyboard_stroke & W)) keyboard_stroke += W;
                            break;
                        case KEY_R:
                            if(!(keyboard_stroke & H)) keyboard_stroke += H;
                            break;
                        case KEY_F:
                            if(!(keyboard_stroke & R1)) keyboard_stroke += R1;
                            break;
                        case KEY_C:
                            if(!(keyboard_stroke & A)) keyboard_stroke += A;
                            break;
                        case KEY_V:
                            if(!(keyboard_stroke & O)) keyboard_stroke += O;
                            break;
                        case KEY_T:
                        case KEY_G:
                        case KEY_Y:
                        case KEY_H:
                            if(!(keyboard_stroke & AST)) keyboard_stroke += AST;
                            break;
                        case KEY_N:
                            if(!(keyboard_stroke & E)) keyboard_stroke += E;
                            break;
                        case KEY_M:
                            if(!(keyboard_stroke & U)) keyboard_stroke += U;
                            break;
                        case KEY_U:
                            if(!(keyboard_stroke & F)) keyboard_stroke += F;
                            break;
                        case KEY_J:
                            if(!(keyboard_stroke & R2)) keyboard_stroke += R2;
                            break;
                        case KEY_I:
                            if(!(keyboard_stroke & P2)) keyboard_stroke += P2;
                            break;
                        case KEY_K:
                            if(!(keyboard_stroke & B)) keyboard_stroke += B;
                            break;
                        case KEY_O:
                            if(!(keyboard_stroke & L)) keyboard_stroke += L;
                            break;
                        case KEY_L:
                            if(!(keyboard_stroke & G)) keyboard_stroke += G;
                            break;
                        case KEY_P:
                            if(!(keyboard_stroke & T2)) keyboard_stroke += T2;
                            break;
                        case KEY_SEMICOLON:
                            if(!(keyboard_stroke & S2)) keyboard_stroke += S2;
                            break;
                        case KEY_LEFTBRACE:
                            if(!(keyboard_stroke & D)) keyboard_stroke += D;
                            break;
                        case KEY_APOSTROPHE:
                            if(!(keyboard_stroke & Z)) keyboard_stroke += Z;
                            break;
                        default:
                            break;
                    }
                    break;
                case KEY_HELD:
                    break;
                default:
                    break;
            }
        }

        if(stroking && !keys_held) {
            logging.logf(DEBUG, "Sending stroke: %d\n", keyboard_stroke);
            on_stroke(engine, engine->dictionary_manager.stroke_to_char(keyboard_stroke));
            keyboard_stroke = 0;
            stroking = 0;
        }
    }
}

int setup_keyboard_input_library(struct InputLibrary* input_library) {
    input_library->setup = keyboard_setup;
    input_library->cleanup = keyboard_cleanup;
    input_library->scan = keyboard_scan;

    return 1;
}

#endif