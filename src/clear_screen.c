#include <stdlib.h>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

void cls() {
    system(CLEAR_SCREEN);
}
