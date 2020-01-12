#ifndef __PYTHON_CTYPES_SPEED_TEST_INTERFACE__
#define __PYTHON_CTYPES_SPEED_TEST_INTERFACE__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TESTER_IDLE,
    TESTER_PREPARED,
    TESTER_STARTED,
} TESTER_STATUS;

typedef void* Tester;

Tester                  tester_get          ();
void                    tester_destroy      (Tester tester);

int                     tester_prepare      (Tester tester, int workers);
void                    tester_unprepare    (Tester tester);

int                     tester_start        (Tester tester);
void                    tester_stop         (Tester tester);
void                    tester_wait         (Tester tester);

const TESTER_STATUS     tester_status       (Tester tester);

#ifdef __cplusplus
}
#endif

#endif