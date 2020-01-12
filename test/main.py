import os
import platform
import ctypes

__filepath__ = os.path.dirname(os.path.abspath(__file__))
__system__ = platform.system()

if __system__ == "Linux":
    __dyext__ = "so"
elif __system__ == "Darwin":
    __dyext__ = "dylib"
else:
    __dyext__ = "dll"

class Tester:
    __lib_path = __filepath__ + "/../build/libpythonCtypesSpeedTest." + __dyext__
    __lib = ctypes.cdll.LoadLibrary(__lib_path)

    __lib.tester_get.restype = ctypes.c_void_p
    __lib.tester_destroy.argtypes = [ctypes.c_void_p]

    __lib.tester_prepare.restype = ctypes.c_int
    __lib.tester_prepare.argtypes = [ctypes.c_void_p, ctypes.c_int]

    __lib.tester_unprepare.argtypes = [ctypes.c_void_p]

    __lib.tester_start.restype = ctypes.c_int
    __lib.tester_start.argtypes = [ctypes.c_void_p]

    __lib.tester_stop.argtypes = [ctypes.c_void_p]

    __lib.tester_wait.argtypes = [ctypes.c_void_p]

    __lib.tester_status.restype = ctypes.c_uint
    __lib.tester_status.argtypes = [ctypes.c_void_p]

    IDLE = 0
    PREPARED = 1
    STARTED = 2

    def __init__(self):
        self.tester = self.__lib.tester_get()

    def __del__(self):
        self.__lib.tester_destroy(self.tester)

    def prepare(self, workers=1):
        return self.__lib.tester_prepare(self.tester, workers)

    def unprepare(self):
        self.__lib.tester_unprepare(self.tester)

    def start(self):
        return self.__lib.tester_start(self.tester)

    def stop(self):
        self.__lib.tester_stop(self.tester)

    def wait(self):
        self.__lib.tester_wait(self.tester)

    def status(self):
        return self.__lib.tester_status(self.tester)

tester = Tester()

tester.prepare(50)

for i in range(10):
    tester.start()
    tester.wait()

tester.unprepare()