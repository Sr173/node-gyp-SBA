#include <iostream>
#include <string>
#include "greeting.h"
#include <Windows.h>

get_machine_code_function get_machine_code;
detect_function detect;
init_function init;



int initYolo() {
    static auto handle = LoadLibraryA("detect-lib.dll");
    if (handle == nullptr) {
        return -99;
    }

    init = reinterpret_cast<init_function>(GetProcAddress(handle, "init"));
    detect = reinterpret_cast<detect_function>(GetProcAddress(handle, "detect"));
    get_machine_code = reinterpret_cast<get_machine_code_function>(GetProcAddress(handle, "get_machine_code"));

    if (!init || !detect) {
        return -100;
    }

    return init();
}
