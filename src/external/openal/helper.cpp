#include <AL/al.h>
#include "stdio.h"

int main() {
    printf(alGetString(AL_VERSION));
    return 0;
}