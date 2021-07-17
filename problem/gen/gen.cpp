#include "testlib.h"
#include <cassert>

int main(int argc, char* argv[]) {
    assert(argc > 0);
    registerGen(argc, argv, 1);
    int n = atoi(argv[1]);
    while (n--) {
        printf("%d ", rnd.next(-100, 100));
    }
}
