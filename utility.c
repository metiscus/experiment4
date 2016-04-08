#include "utility.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

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

#define BLOCK_SIZE 1024
char* read_text_file(const char* filename)
{
    char* buffer = NULL;
    size_t size = 0;
    size_t free = 0;
    FILE *fp = fopen(filename, "rt");
    while(fp && !feof(fp))
    {
        if(free == 0)
        {
            char* new_buffer = (char*)realloc(buffer, size + BLOCK_SIZE);
            if(new_buffer)
            {
                free += BLOCK_SIZE;
                size += BLOCK_SIZE;
                buffer = new_buffer;
            }
            else
            {
                fprintf(stderr, "read_text_file realloc fail\n");
                return buffer;
            }
        }
        
        if(free == 0)
            break;

        size_t bytes_read = fread(&buffer[size - BLOCK_SIZE], 1, free, fp);
        free -= bytes_read;
    }
    
    if(buffer != NULL && size > 0)
    {
        buffer[size] = '\0';
    }
    return buffer;
}
