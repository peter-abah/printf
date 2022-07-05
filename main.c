#include <limits.h>
#include "printf.h"

int main(void) {
    myprintf("Hey boy %o %i %x %u %% %n\n", ULONG_MAX, LONG_MAX, ULONG_MAX, ULONG_MAX);
    return 0;
}
