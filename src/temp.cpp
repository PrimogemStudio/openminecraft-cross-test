#include <AL/al.h>

const ALchar* alwrap_test() {
    return alGetString(AL_VERSION);
}