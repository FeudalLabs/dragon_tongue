#include "source.h"
#include <ctype.h>

Source* source_from_file(const char* filename, Arena* arena) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory from arena
    Source* source = arena_alloc(arena, sizeof(Source));
    source->content = arena_alloc(arena, size + 1);
    source->length = size;
    source->position = 0;
    source->line = 1;
    source->column = 1;
    source->filename = arena_alloc(arena, strlen(filename) + 1);
    strcpy((char*)source->filename, filename);
    
    // Read file content
    size_t bytes_read = fread(source->content, 1, size, file);
    source->content[bytes_read] = '\0';
    fclose(file);
    
    LOG("Source loaded from file");
    return source;
}

Source* source_from_string(const char* content, Arena* arena) {
    int len = strlen(content);
    
    Source* source = arena_alloc(arena, sizeof(Source));
    source->content = arena_alloc(arena, len + 1);
    strcpy(source->content, content);
    source->length = len;
    source->position = 0;
    source->line = 1;
    source->column = 1;
    source->filename = "<string>";
    
    LOG("Source created from string");
    return source;
}

char source_peek(Source* s) {
    if (source_is_eof(s)) {
        return '\0';
    }
    return s->content[s->position];
}

char source_peek_ahead(Source* s, int offset) {
    int pos = s->position + offset;
    if (pos < 0 || pos >= s->length) {
        return '\0';
    }
    return s->content[pos];
}

char source_advance(Source* s) {
    if (source_is_eof(s)) {
        return '\0';
    }
    
    char c = s->content[s->position];
    s->position++;
    
    // Track line and column
    if (c == '\n') {
        s->line++;
        s->column = 1;
    } else {
        s->column++;
    }
    
    return c;
}

bool source_is_eof(Source* s) {
    return s->position >= s->length;
}

void source_skip_whitespace(Source* s) {
    while (!source_is_eof(s)) {
        char c = source_peek(s);
        if (c == ' ' || c == '\t' || c == '\r') {
            source_advance(s);
        } else if (c == '\n') {
            source_advance(s);
        } else {
            break;
        }
    }
}

void source_get_location(Source* s, int* line, int* column) {
    *line = s->line;
    *column = s->column;
}

void source_position_to_line_column(Source* s, int position, int* line, int* column) {
    *line = 1;
    *column = 1;
    
    for (int i = 0; i < position && i < s->length; i++) {
        if (s->content[i] == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
    }
}