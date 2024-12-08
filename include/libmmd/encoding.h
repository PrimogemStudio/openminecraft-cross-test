#ifndef ENCODING_H
#define ENCODING_H

#include "stdint.h"

extern "C" {

char* mmd_encoding_utf16_to_utf8(char* raw, uint64_t length);

}

#endif
