/* libc.cys supplies assert entirely in Cyclone. Avoid parsing the unused
 * host glibc assert declarations (including its newer _Bool helper). */
#include "cyclone/library/std/assert.h"
