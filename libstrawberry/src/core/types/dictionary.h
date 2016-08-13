/*******************************************************************************
**                                                                            **
**   The MIT License                                                          **
**                                                                            **
**   Copyright 2016 strawberryentrypoint                                      **
**                                                                            **
**   Permission is hereby granted, free of charge, to any person              **
**   obtaining a copy of this software and associated documentation files     **
**   (the "Software"), to deal in the Software without restriction,           **
**   including without limitation the rights to use, copy, modify, merge,     **
**   publish, distribute, sublicense, and/or sell copies of the Software,     **
**   and to permit persons to whom the Software is furnished to do so,        **
**   subject to the following conditions:                                     **
**                                                                            **
**   The above copyright notice and this permission notice shall be           **
**   included in all copies or substantial portions of the Software.          **
**                                                                            **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   **
**                                                                            **
********************************************************************************
**
**  Notes:
**    -
**
*/

#ifndef __SB_CORE_TYPE_DICTIONARY_H
#define __SB_CORE_TYPE_DICTIONARY_H


#include <stdint.h>
#include <stddef.h>

#include "bool.h"
#include "../sbapi.h"


typedef struct sb_dictionary_entry {
	char *key;
	void *value;
} sb_dictionary_entry_t;

typedef struct sb_dictionary {
	sb_dictionary_entry_t *entries;
	sb_size_t __size;
	sb_size_t count;
} sb_dictionary_t;


#ifdef __cplusplus
extern "C" {
#endif

	SBAPI void sb_dictionary_init(sb_dictionary_t *dictionary, sb_size_t size);
	SBAPI void sb_dictionary_clear(sb_dictionary_t *dictionary);
	SBAPI sb_bool_t sb_dictionary_get_index(sb_dictionary_t *dictionary, const char *key, sb_size_t *index);
	SBAPI sb_dictionary_entry_t* sb_dictionary_get(sb_dictionary_t *dictionary, const char *key);
	SBAPI sb_bool_t sb_dictionary_set(sb_dictionary_t *dictionary, const char *key, void *value);

#ifdef __cplusplus
}
#endif


#endif
