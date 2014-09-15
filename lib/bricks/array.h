#ifndef __bricks_array
#define __bricks_array

#include "bricks.h" // for Boolean

typedef struct Array {
  int length;
  int capacity;

  void **values;
} Array;

typedef void (ArrayIterator)(void *);

#define array_length(A)      ((A)->length)
#define array_capacity(A)    ((A)->capacity)
#define array_values(A)      ((A)->values)
#define array_at_index(A, I) (array_values(A)[I])
#define array_get(A, I)      array_at_index(A, I)
#define array_free(A)        (pfree(A))
#define array_free_each(A)   (array_each(A, free)) // macro cannot handle pfree :(
#define array_r_free(A)      (array_free_each(A), array_free(A))

Array     *Array_create(int capacity);
void      array_push(Array *array, void *element);
Boolean   array_expand(Array *array, int seed);
void      array_each(Array *array, ArrayIterator f);
void      *array_pop(Array *array);
void      array_set(Array *array, int index, void *value);

#endif
