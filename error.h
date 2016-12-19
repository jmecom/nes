#pragma once

#define ERROR(error_message, error_code) \
	printf("%s\n", error_message); \
	printf("Exited in function %s with error code %d\n", __func__, error_code); \
	exit(error_code);
	