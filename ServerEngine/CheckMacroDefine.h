#pragma once
#include <assert.h>

#define ASSERT_FAIELD (assert(false))
#undef ASSERT
#define ASSERT(x)     (assert(x))


////////////////////////////////////////////



