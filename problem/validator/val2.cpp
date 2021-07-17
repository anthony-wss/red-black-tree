#include "testlib.h"

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int a = inf.readInt(-2147483648, 2147483647, "a");
    inf.readSpace();
    int b = inf.readInt(-2147483648, 2147483647, "b");
    inf.readEoln();
    inf.readEof();
}
