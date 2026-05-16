#include "dragontongue.h"
#include "lexer/token.h"

int main(int argc, char** argv) {
    printf("DragonTongue Compiler v%s\n", DRAGONTONGUE_VERSION);
    
    if (argc < 2) {
        fprintf(stderr, "Usage: dragontongue <source.dt>\n");
        return 1;
    }
    
    printf("Compiling: %s\n", argv[1]);
    
    // TODO: Lexer, parser, etc.
    
    return 0;
}