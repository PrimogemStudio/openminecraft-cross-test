#include "libmmd/encoding.h"
#include "libmmd/memory.h"
#include <cstring>

bool mmd_encoding_is_utf16_high(uint16_t i)
{
    return 0xD800 <= i && i < 0xDC00;
}

bool mmd_encoding_is_utf16_low(uint16_t i)
{
    return 0xDC00 <= i && i < 0xE000;
}

char* mmd_encoding_utf16_to_utf8(char* raw, uint64_t length)
{
    if (length % 2 != 0) return 0;

    uint32_t* codepoints = (uint32_t*) mmd_memory_allocate(length * 2 * sizeof(uint32_t));
    uint64_t p = 0;
    uint64_t codepointp = 0;

    uint16_t* u16str = (uint16_t*) raw;
        while (p < length / 2)
    {
        if (mmd_encoding_is_utf16_high(u16str[p]))
        {
            if (mmd_encoding_is_utf16_low(u16str[p + 1]))
            {
                codepoints[codepointp] = 0x10000 + (uint32_t(u16str[p]) - 0xD800) * 0x400 + (uint32_t(u16str[p + 1]) - 0xDC00);
                p += 2;
            }
            else if (u16str[p + 1] == 0)
            {
                codepoints[codepointp] = (uint32_t) u16str[p];
                p++;
            }
        }
        else if (mmd_encoding_is_utf16_low(u16str[p]))
        {
            if (u16str[p + 1] == 0)
            {
                codepoints[codepointp] = (uint32_t) u16str[p];
                p++;
            }
        }
        else
        {
            codepoints[codepointp] = (uint32_t) u16str[p];
            p++;
        }
        codepointp++;
    }
     
    uint64_t u8l = 0;
    for (uint64_t i = 0; i < codepointp; i++)
    {
        if (codepoints[i] < 0x80) u8l++;
        else if (0x80 <= codepoints[i] && codepoints[i] < 0x800) u8l += 2;
        else if (0x800 <= codepoints[i] && codepoints[i] < 0x10000) u8l += 3;
        else u8l += 4;
    }

    char* target = (char*) mmd_memory_allocate(u8l + 1);
    uint64_t basep = 0;
    for (uint64_t i = 0; i < codepointp; i++)
    {
        uint32_t chr = codepoints[i];
        if (codepoints[i] < 0x80)
        {
            target[basep] = char(chr);
            basep++;
        }
        else if (0x80 <= codepoints[i] && codepoints[i] < 0x800)
        {
            target[basep] = 0b11000000 | char(chr >> 6);
            target[basep + 1] = 0x10000000 | (char(chr) & 0b00111111);
            basep += 2;
        }
        else if (0x800 <= codepoints[i] && codepoints[i] < 0x10000)
        {
            target[basep] = 0b11100000 | char(chr >> 12);
            target[basep + 1] = 0b10000000 | (char(chr >> 6) & 0b00111111);
            target[basep + 2] = 0b10000000 | (char(chr) & 0b00111111);
            basep += 3;
        }
        else
        {
            target[basep] = 0b11110000 | char(chr >> 18);
            target[basep + 1] = 0b10000000 | (char(chr >> 12) & 0b00111111);
            target[basep + 2] = 0b10000000 | (char(chr >> 6) & 0b00111111);
            target[basep + 3] = 0b10000000 | (char(chr) & 0b00111111);
            basep += 4;
        }
    }
    target[basep] = '\0';
    mmd_memory_deallocate(codepoints);

    return target;
}
