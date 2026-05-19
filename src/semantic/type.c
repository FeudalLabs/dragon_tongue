#include "type.h"
#include <stdio.h>

TypeInfo type_int(void) {
    TypeInfo t = {TYPE_INT};
    return t;
}

TypeInfo type_bool(void) {
    TypeInfo t = {TYPE_BOOL};
    return t;
}

TypeInfo type_string(void) {
    TypeInfo t = {TYPE_STRING};
    return t;
}

TypeInfo type_void(void) {
    TypeInfo t = {TYPE_VOID};
    return t;
}

TypeInfo type_error(void) {
    TypeInfo t = {TYPE_ERROR};
    return t;
}

TypeInfo type_unknown(void) {
    TypeInfo t = {TYPE_UNKNOWN};
    return t;
}

bool types_equal(TypeInfo a, TypeInfo b) {
    if (a.kind == TYPE_UNKNOWN || b.kind == TYPE_UNKNOWN) return true;
    if (a.kind == TYPE_ERROR || b.kind == TYPE_ERROR) return true;
    return a.kind == b.kind;
}

const char* type_to_string(TypeInfo t) {
    switch (t.kind) {
        case TYPE_INT: return "Int";
        case TYPE_BOOL: return "Bool";
        case TYPE_STRING: return "String";
        case TYPE_VOID: return "Void";
        case TYPE_ERROR: return "Error";
        case TYPE_UNKNOWN: return "Unknown";
        default: return "?";
    }
}