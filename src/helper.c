
#include <stdlib.h>
#include "helper.h"

void* resizeArray(void *array, unsigned int size) {
	void *temp = realloc(array, size);
	if (!temp) return 0;
	return temp;
}
