/**
 *  hash_map.h
 *  Data structures and algorithms course work
 *
 *  Created by Perttu Kärnä on 25.12.2017.
 *  Copyright © 2018 Perttu Kärnä. All rights reserved.
 */

#ifndef hash_map_h
#define hash_map_h

#include <stdio.h>
#define MAX_SIZE 256

#define MAP_MISSING -4  /* No such element */
#define MAP_FULL -3 	/* Hashmap is full */
#define MAP_OMEM -2 	/* Out of Memory */
#define MAP_OK -1 		/* OK */

typedef void *any_t;

typedef any_t map_t;

map_t new_hashmap(void);

unsigned long hashmap_hash(unsigned char *key);

int hashmap_insert(map_t map, char *key, int amount);

int hashmap_remove(map_t map, char *key);

int hashmap_get_by_key(map_t map, char *key, any_t *ptr);

int hashmap_get_by_index(map_t map, int index, any_t *ptr);

int hashmap_get_occurrence_by_index(map_t map, int index);

int hashmap_get_used_size(map_t map);

int hashmap_get_size(map_t map);

int hashmap_expand(map_t map);

void hashmap_print_key(map_t map, int index);

void hashmap_quicksort(map_t m, int low, int high);

#endif /* hash_map_h */
