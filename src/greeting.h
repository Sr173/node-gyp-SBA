#include <string>
int initYolo();
typedef int (*detect_function)(void* buffer, int w, int h, char* outBuffer, int outBufferLen, bool debug);
typedef int (*init_function)();
typedef void (*get_machine_code_function)(char* buf);
typedef void (*enable_gpu_function)(bool buf);

extern get_machine_code_function get_machine_code;
extern detect_function detect;
extern init_function init;
extern enable_gpu_function enable_gpu;
extern bool debug_gpu;