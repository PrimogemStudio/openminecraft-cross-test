#include "shaderc/shaderc.h"

int main() {
    unsigned int a, b;
    shaderc_get_spv_version(&a, &b);
    return 0;
}