#include "Manager.h"
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

Tester tester_get() {
    return new Manager();
}

void tester_destroy(Tester tester) {
    delete static_cast<Manager*>(tester);
}

int tester_prepare(Tester tester, int workers) {
    return static_cast<Manager*>(tester)->prepare(workers);
}

void tester_unprepare(Tester tester) {
    static_cast<Manager*>(tester)->unprepare();
}

int tester_start(Tester tester) {
    return static_cast<Manager*>(tester)->start();
}

void tester_stop(Tester tester) {
    static_cast<Manager*>(tester)->stop();
}

void tester_wait(Tester tester) {
    static_cast<Manager*>(tester)->wait();
}

const TESTER_STATUS tester_status(Tester tester) {
    return (TESTER_STATUS)static_cast<Manager*>(tester)->status();
}

#ifdef __cplusplus
}
#endif