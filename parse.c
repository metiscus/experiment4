#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("You must supply an argument.\n");
        return 1;
    }

    typedef enum Mode
    {
        normal,
        precompiler,
        string,
        block
    } Mode;

    Mode mode = normal;
    
    FILE *fp = fopen(argv[1], "rt");

#define TOKEN_LENGTH 1000
#define TOKEN_ADD(c) do { token[token_idx++] = c; token[token_idx] = '\0'; } while(0);
#define TOKEN_RESET() do { token_idx=0; token[0] = '\0'; } while(0);

    int token_idx = 0;
    char token [1000];

    while(fp && !feof(fp))
    {
        char c = fgetc(fp);

        switch(mode)
        {
            case precompiler:
            {
                if(c == '\n') mode = normal;
                TOKEN_RESET();
                break;
            }

            case block:
            case normal:
            {
                if(c == '#') mode = precompiler;
                else if(c == '"') mode = string;
                else if(c == '{') mode = block;
                else if(c == '}') mode = normal;
                else if(c != ' ' && c != '\t' && c != ';' && c != ',' && c != '(' && c != ')') 
                {
                    TOKEN_ADD(c)
                }
                else {
                    if(token[0] != '\0' || token[0] != ' ')
                    {
                        //printf("ccc %d\n", (int)c);
                        printf("%s\n", token);
                    }
                    TOKEN_RESET();
                }
            }
            break;

            case string:
            {
                if(c!='\\')
                {
                    c = fgetc(fp);
                    if(c == '"')
                    {
                        TOKEN_ADD('"');
                    }
                    else
                    {
                        TOKEN_ADD('\\');
                        TOKEN_ADD(c);
                    }
                }
                else if (c=='"')
                {
                    printf("%s\n", token);
                    TOKEN_RESET();
                    mode = normal;
                }
                break;
            }
        }
    }
    
}
