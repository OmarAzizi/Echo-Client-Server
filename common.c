#include "common.h"

void handle_errors(const char* fmt, ...) {
    int errno_save;
    va_list ap; // used by functions with a varying number of arguments of varying types
    
    errno_save = errno; // number of last error set by syscalls (ony significant when -1)
    
    va_start(ap, fmt); // initiallizes 'ap' for supsequent use
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    if (errno_save != 0) {
        fprintf(stdout, "(ERROR_NO = %d) : %s\n", errno_save, strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    va_end(ap);

    exit(EXIT_FALIURE);
}

char* bin2hex(const unsigned char* input, size_t len) {
    char* result;
    char* hexits = "0123456789ABCDEF"; 

    if (input == NULL || len < 0) return NULL;

    // 2 * hexits + 1 NULL
    int result_length = (len * 3) + 1;
    
    result = (char*)malloc(result_length);
    bzero(result, result_length);

    for (int i = 0; i < len; ++i) {
        result[i * 3] = hexits[input[i] >> 4];
        result[(i * 3) + 1] = hexits[input[i] & 0x0F];
        result[(i * 3) + 2] = ' '; // for readability
    }
    return result;
}

void Listen(int fd, int backlog) {
    char* ptr;
    if ( (ptr = getenv("LISTENQ")) != NULL ) 
        backlog = atoi(ptr);
    if (listen(fd, backlog) < 0) 
        handle_errors("Listen error.\n");
}
