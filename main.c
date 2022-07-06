#include <limits.h>
#include "printf.h"

int main(void) {
    myprintf("Hey boy |%#d| +o %#o %#X %u %% %n\n", INT_MIN, LONG_MAX, ULONG_MAX, ULONG_MAX);
    return 0;
}
