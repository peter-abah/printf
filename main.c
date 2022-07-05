#include "printf.h"

int main(void) {
    myprintf("Hey boy %x %d %l %% %n\n", -1, 10024, 67896541L);
    return 0;
}
