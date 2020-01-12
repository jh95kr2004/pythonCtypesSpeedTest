#include "interface.h"

int main(int argc, char* argv[]) {
    Tester tester = tester_get();

    tester_prepare(tester, 50);

    int i;

    for(i = 0; i < 10; i++) {
        tester_start(tester);
        tester_wait(tester);
    }

    tester_unprepare(tester);

    tester_destroy(tester);

    return 0;
}