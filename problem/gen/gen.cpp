#include "testlib.h"
#include <cassert>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    assert(argc > 0);
    registerGen(argc, argv, 1);
    int n = atoi(argv[1]);
    int arr[n];
    vector<int> rt2 = rnd.perm(n, 1);
    for (int i = 0; i < n; i++) {
        arr[i] = rt2[i];
        printf("1 %d\n", arr[i]);
    }
    vector<int> rt = rnd.perm(n, 0);
    for (int i = 0; i < n; i++) {
        printf("2 %d\n", arr[rt[i]]);
    }
}
