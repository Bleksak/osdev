#pragma once

#define PACKED __attribute__((packed))
#define ALIGNED(x) __attribute__((aligned(x)))
#define NORETURN __attribute__((noreturn))
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define INTERRUPT __attribute__((always_inline))
