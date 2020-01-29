#pragma once

#define Vector(name, T) \
    struct name { \
        size_t length;\
        T* entries;\
    }; \
    typedef struct name name

#define VectorInstance(name, length) \
    (name) {.length = length, .entries = calloc(sizeof(name), length)}

#define length(name) (name).length

#define VectorPush(vec, value, result) \
    result = realloc(vec.entries, (vec.length + 1) * sizeof(vec.entries[0])); \
    if(result) { \
        vec.entries = result; \
        memcpy((void*) &vec.entries[vec.length], (void*) &value, sizeof(vec.entries[0])); \
        vec.length++; \
    } \
    
