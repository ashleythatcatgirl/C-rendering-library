
#include <stdlib.h>

#include "helper.h"

void* resizeArray(void *array, uint size) {
	void *temp = realloc(array, size);
	return temp;
}
