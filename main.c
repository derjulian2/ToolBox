#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define def_vector(type)									\
typedef struct												\
{															\
	size_t size;											\
	##type* data;											\
} vector_##type;											\
															\
static inline vector_##type make_vector_##type()			\
{															\
	vector_##type res = { 0, NULL };						\
	return res;												\
}															\
															\
static inline void add_vector_##type(						\
	vector_##type* vec,										\
	##type elem)											\
{															\
	##type* new_data = malloc(sizeof(##type) * (vec->size + 1));\
	if (new_data != NULL)									\
	{														\
		if (vec->data != NULL)								\
		{													\
			for (size_t i = 0; i < vec->size; i++)			\
			{												\
				new_data[i] = vec->data[i];					\
			}												\
		}													\
		new_data[vec->size] = elem;							\
		vec->size++;										\
		free(vec->data);									\
		vec->data = new_data;								\
	}														\
}															\
															\
static inline void add_range_vector_##type(					\
	vector_##type* vec,										\
	##type* elem_ptr,										\
	size_t elem_count)										\
{															\
	##type* new_data = malloc(sizeof(##type) * (vec->size + elem_count));\
	if (new_data != NULL)									\
	{														\
		if (vec->data != NULL)								\
		{													\
			for (size_t i = 0; i < vec->size; i++)			\
			{												\
				new_data[i] = vec->data[i];					\
			}												\
		}													\
		if (elem_ptr != NULL)								\
		{													\
			for (size_t i = vec->size; i < (vec->size + elem_count); i++)\
			{												\
				new_data[i] = elem_ptr[i - vec->size];		\
			}												\
		}													\
		vec->size += elem_count;							\
		free(vec->data);									\
		vec->data = new_data;								\
	}														\
}															\
															\
static inline void free_vector_##type(vector_##type* vec)	\
{															\
	vec->size = 0;											\
	free(vec->data);										\
	vec->data = NULL;										\
}	

#define iterate(x) \
for (size_t i = 0; i < x; i++)

def_vector(int)
