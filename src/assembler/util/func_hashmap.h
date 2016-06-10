/*
 * Generic hashmap manipulation functions
 *
 * Originally by Elliot C Back - http://elliottback.com/wp/hashmap-implementation-in-c/
 *
 * Modified by Pete Warden to fix a serious performance problem, support strings as keys
 * and removed thread synchronization - http://petewarden.typepad.com
 *
 * Further modified by Emanuele Rossi to support function pointers as values
 */
#ifndef FUNC_HASHMAP_H
#define FUNC_HASHMAP_H

#include "hashmap.h"
#include "../../emulator/arm11.h"

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */

/*
 * any_t is a pointer.  This allows you to put arbitrary structures in
 * the hashmap.
 */

typedef void (*func_t)(char*, union decoded_instr*);

/*
 * func_map_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how hashmaps are
 * represented.  They see and manipulate only func_map_t's.
 */
typedef any_t func_map_t;

/*
 * Return an empty hashmap. Returns NULL if empty.
*/
extern func_map_t func_hashmap_new();

/*
 * Add an element to the hashmap. Return MAP_OK or MAP_OMEM.
 */
extern int func_hashmap_put(func_map_t in, const char* key, func_t value);

/*
 * Get an element from the hashmap. Return NULL if the element is not present.
 */
extern func_t func_hashmap_get(func_map_t in, const char* key);

/*
 * Remove an element from the hashmap. Return MAP_OK or MAP_MISSING.
 */
extern int func_hashmap_remove(func_map_t in, const char* key);

/*
 * Free the hashmap
 */
extern void func_hashmap_free(func_map_t in);

/*
 * Get the current size of a hashmap
 */
extern int func_hashmap_length(func_map_t in);

#endif
