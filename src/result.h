#pragma once
#include "std.h"

#define Result(p, t) (Result) {.ok = p, .result = (any_t)t}
#define Ok(ptr) Result(true, ptr)
#define Err(msg) Result(false, msg)

typedef void* any_t;

struct Result {
    bool ok;
    any_t result;
};

typedef struct Result Result;
