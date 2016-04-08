#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "lexer.h"
#include "utility.h"

static void
test_log(void *pArg, enum lexer_log_level type, lexer_size line, const char *fmt, ...);

typedef enum FieldType
{
    field_uint32,
    field_uint64,
    field_int32,
    field_int64,
    field_float,
    field_double,
    field_string,
    field_type,
} FieldType;

#define FIELD_NAME_MAX 128

typedef struct Field
{
    FieldType type;
    char name[FIELD_NAME_MAX];
} Field;

typedef struct Type
{
    char *name;
    uint32_t field_count;
    FieldType fields;
} Type;

#define INSTANCE_NAME_MAX 128
typedef struct TypeInstance
{
    char name[INSTANCE_NAME_MAX];
    Type* type;
    void *ptr;
} TypeInstance;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("You must supply an argument.\n");
        return 1;
    }
    
    // read file
    char *text = read_text_file(argv[1]);
    uint32_t text_length = strlen(text);

    /* initialize lexer */
    struct lexer lexer;
    lexer_init(&lexer, text, text_length, NULL, test_log, NULL);

    typedef enum Mode
    {
        mode_none,
        mode_type,
        mode_instance,
    } Mode;

    /* parse tokens */
    struct lexer_token tok;
    Mode mode = mode_none;
    lexer_read(&lexer, &tok);
    
    char buffer[1024];
    lexer_token_cpy(buffer, 1024, (const struct lexer_token*)&tok);
    fprintf(stderr, "[mab] '%s'\n", buffer);
    const char *type = "type";
    if(lexer_token_cmp(&tok, type))
    {
        printf("inside type\n");
        if(!lexer_expect_type(&lexer, LEXER_TOKEN_PUNCTUATION, LEXER_PUNCT_BRACE_OPEN, &tok)) goto parse_error;
        printf("1\n");
        
        for( ; ; )
        {
            
            
        }
        
        if(!lexer_expect_type(&lexer, LEXER_TOKEN_PUNCTUATION, LEXER_PUNCT_BRACE_CLOSE, &tok)) goto parse_error;
        printf("2\n");
    }
    
        //lexer_expect_string(&lexer, "string");
        //lexer_expect_type(&lexer, LEXER_TOKEN_NUMBER, LEXER_TOKEN_HEX, &tok);

        /* check and parse only if correct */
        //if (lexer_check_string(&lexer, "string")) { }
            /* correct string  */
        //if (lexer_check_type(&lexer, LEXER_TOKEN_NUMBER, LEXER_TOKEN_BIN, &tok)) { }
            /* correct type */

        /* only check but don't parse */
        //if (lexer_peek_string(&lexer, "string")) { }
            /* correct string  */
        //if (lexer_peek_type(&lexer, LEXER_TOKEN_PUNCTUATION, LEXER_PUNCT_DOLLAR, &tok)) { }
            /* correct type */

        /* token compare function */
        //if (!lexer_token_cmp(&tok, "string")) { }
            /* token holds string 'string' */
        //if (!lexer_token_icmp(&tok, "string")) { }
            /* token holds case independent string 'string' */

        /* copy token content into buffer */
        //char buffer[1024];
        //lexer_token_cpy(buffer, 1024, &tok);

        /* token to number conversion */
        /* You should always check if the token (sub)type is correct */
        //int i = lexer_token_to_int(&tok);
        //float f = lexer_token_to_float(&tok);
        //double d = lexer_token_to_double(&tok);
        //unsigned long ul = lexer_token_to_unsigned_long(&tok);
    return 0;
    
parse_error:
    printf("An unrecoverable parse error occurred.\n");
    return 1;
}

static void
test_log(void *pArg, enum lexer_log_level type, lexer_size line, const char *fmt, ...)
{
    char buffer[1024];
    va_list arglist;
    (void)pArg;
    va_start(arglist, fmt);
    printf("%s(%lu):  ", (type == LEXER_WARNING) ? "Warning" : "Error", line);
    vprintf(fmt, arglist);
    printf("\n");
    va_end(arglist);
}
