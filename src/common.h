#ifndef OMPCHAT_COMMON_H
#define OMPCHAT_COMMON_H

#define BUF_SIZE 256

// Shorthand to run a function that returns -1 on error and then report the error
#define CHECK_ERROR(var, func, error_msg) \
var = func; \
if (var == -1) \
return error(error_msg);

int error(char *text);

#endif //OMPCHAT_COMMON_H