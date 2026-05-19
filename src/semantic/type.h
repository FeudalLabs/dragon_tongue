#ifndef TYPE_H
#define TYPE_H

#include "../dragontongue.h"

typedef enum {
    TYPE_INT,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_VOID,
    TYPE_ERROR,
    TYPE_UNKNOWN
} TypeKind;

typedef struct TypeInfo {
    TypeKind kind;
} TypeInfo;

// Type creation
TypeInfo type_int(void);
TypeInfo type_bool(void);
TypeInfo type_string(void);
TypeInfo type_void(void);
TypeInfo type_error(void);
TypeInfo type_unknown(void);

// Type checking
bool types_equal(TypeInfo a, TypeInfo b);
const char* type_to_string(TypeInfo t);

#endif