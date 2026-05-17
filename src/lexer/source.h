#ifndef SOURCE_H
#define SOURCE_H

#include "../dragontongue.h"

// Source represents a loaded source file
typedef struct Source {
    char* content;      // Full file content
    int length;         // Total length
    int position;       // Current read position (0-based)
    int line;           // Current line number (1-based)
    int column;         // Current column number (1-based)
    char* filename;     // Original filename (for error messages)
} Source;

// Create a new source from a file
// Returns NULL on error (file not found, read error)
Source* source_from_file(const char* filename, Arena* arena);

// Create a new source from a string (for testing)
Source* source_from_string(const char* content, Arena* arena);

// Peek at current character without advancing
char source_peek(Source* s);

// Peek ahead N characters (0 = current)
char source_peek_ahead(Source* s, int offset);

// Advance to next character and return current
char source_advance(Source* s);

// Check if we've reached end of file
bool source_is_eof(Source* s);

// Skip whitespace (spaces, tabs, newlines)
void source_skip_whitespace(Source* s);

// Get current location for error messages
void source_get_location(Source* s, int* line, int* column);

// Helper: convert position to line/column (for error recovery)
void source_position_to_line_column(Source* s, int position, int* line, int* column);

#endif