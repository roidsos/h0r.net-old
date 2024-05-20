#ifndef __VECTOR_H__
#define __VECTOR_H__

#ifdef HEAP_ACCESSABLE
#include "libk/stdtypes.h"
#include "libk/string.h"
#include "utils/heapdef.h"

#define vector_struct(type) \
	struct {                \
		size_t len;         \
		size_t cap;         \
		type *data;         \
	}

#define vector_extern(type, name) extern vector_struct(type) name
#define vector(type, name) vector_struct(type) name = { 0 }
#define vector_static(type, name) static vector(type, name)

#define vector_init(vec) 			\
(vec)->len = 0; 					\
(vec)->cap = 500; 					\
(vec)->data = malloc((vec)->cap)	\


#define vector_push_back(vec, elem)                          \
	{                                                        \
		(vec)->len++;                                        \
		if ((vec)->cap < (vec)->len * sizeof(elem)) {        \
			(vec)->cap = (vec)->len * sizeof(elem) * 4;      \
			(vec)->data = realloc((vec)->data, (vec)->cap); \
		}                                                    \
		(vec)->data[(vec)->len - 1] = elem;                  \
	}

#define vector_length(vec) (vec)->len
#define vector_at(vec, index) (vec)->data[index]

#define vector_erase(vec, index)                                 \
	{                                                            \
		memcpy(&((vec)->data[index]), &((vec)->data[index + 1]), \
			   sizeof((vec)->data[0]) * (vec)->len - index - 1); \
		(vec)->len--;                                            \
	}

#define vector_erase_all(vec)    \
	{                            \
		(vec)->len = 0;          \
		(vec)->cap = 0;          \
		if ((vec)->data != NULL) \
			free((vec)->data);  \
		(vec)->data = NULL;      \
	}

#define vector_erase_val(vec, val)                      \
	{                                                   \
		for (size_t __i = 0; __i < (vec)->len; __i++) { \
			if (vector_at(vec, __i) == (val)) {         \
				vector_erase(vec, __i);                 \
				break;                                  \
			}                                           \
		}                                               \
	}
#endif
#endif // __VECTOR_H__