#include "common.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

int error(char *text)
{
	printf("%s %d: %s", text, errno, strerror(errno));
	return 1;
}