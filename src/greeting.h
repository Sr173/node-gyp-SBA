#include <string>
int initYolo();
typedef int (*detect_function)(void* buffer, int w, int h, char* outBuffer, int outBufferLen, bool debug);
typedef int (*init_function)();
typedef void (*get_machine_code_function)(char* buf);

extern get_machine_code_function get_machine_code;
extern detect_function detect;
extern init_function init;
