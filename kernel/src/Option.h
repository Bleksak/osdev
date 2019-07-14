#pragma once
#include <stdbool.h>

typedef void* any_t;

struct Option
{
    bool ok;
    any_t result;
};

typedef struct Option option_t;