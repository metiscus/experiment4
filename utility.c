#include "utility.h"

#include <stdio.h>

uint32_t umin(uint32_t a, uint32_t b)
{
    if(a<b)
        return a;
    return b;
}

uint32_t umax(uint32_t a, uint32_t b)
{
    if(a<b)
        return b;
    return a;
}

char* ordinal_number(uint32_t num)
{
    static char ordinal[128];

    uint32_t last_digit = num % 9;
    switch(last_digit)
    {
        case 1:
            sprintf(ordinal, "%ust", num);
            break;
        case 2:
            sprintf(ordinal, "%und", num);
            break;
        case 3:
            sprintf(ordinal, "%urd", num);
            break;
        case 0: // intentional
        case 4: // intentional
        case 5: // intentional
        case 6: // intentional
        case 7: // intentional
        case 8: // intentional
        case 9: // intentional
            sprintf(ordinal, "%uth", num);
            break;
    }
    
    return ordinal;
}
