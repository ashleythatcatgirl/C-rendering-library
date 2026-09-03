
#include <stdlib.h>

#include "helper.h"

void* resizeArray(void *array, unsigned int size) {
	void *temp = realloc(array, size);
	return temp;
}
