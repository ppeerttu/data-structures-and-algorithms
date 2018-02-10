/**
 *  hash_map.c
 *  Data structures and algorithms course work
 *
 *  Created by Perttu Kärnä on 25.12.2017.
 *  Copyright © 2018 Perttu Kärnä. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_map.h"

/**
 * Definging the item of hashmap.
 * @property used - The number of occurrences this key has (0 means empty)
 * @property key - The key of the item
 */
typedef struct _hash_map_item {
	int used;
	char *key;
} hashmap_item;

/**
 * Definingh the hashmap.
 * @property size - The actual size of the hashmap; how many places there is in total.
 * @property used_size - The amount of items in the hash map.
 * @property data - The hashmap data.
 */
typedef struct _hash_map {
	int size;
	int used_size;
	hashmap_item *data;
} hash_map;

// No need to declare public API
int get_index(map_t m, char *key);
void hashmap_switch_item(hashmap_item *a, hashmap_item *b);
int partition(map_t m, int low, int high);

/**
 * Intitialize and return new hashmap.
 * HashMap has reserved size MAX_SIZE and
 * initial used_size is 0 (no elements).
 * @return - A pointer to the new hashmap or
 * NULL if creation failed.
 * @see struct hash_map
 */
map_t new_hashmap() {
	hash_map* map = (hash_map*) malloc(sizeof(hash_map));

	map->data = (hashmap_item*) calloc(MAX_SIZE, sizeof(hashmap_item));

	if (!map) goto err;

	map->size = MAX_SIZE;

	map->used_size = 0;

	return map;

	err:
		if (map) {
			free(map->data);
			free(map);
		}
		return NULL;
}

/**
 * djb2 hash by Dan Bernstein.
 * @param key - An unsigned char pointer to the key (string).
 * @return - The hash of the key.
 */
unsigned long hashmap_hash(unsigned char *key) {
	unsigned long hash = 5381;
	int c;

	while (c = *key++) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

/**
 * Get the index for given key at given map.
 * @param m - The pointer to the hashmap.
 * @param key - The key for which the index is being searched.
 * @return - The index of given key as an integer or constant MAP_FULL (-3).
 */
int get_index(map_t m, char *key) {
	hash_map *map = (hash_map *) m;
	int index = hashmap_hash((unsigned char*) key) % map->size;

	// Linear probing
	for (int i = 0; i < map->size; i++) {

		// Found an empty spot
		if (map->data[index].used == 0) return index;

		// Key found already in the hash map
		if (map->data[index].used > 0 && (strcmp(map->data[index].key, key) == 0)) return index;

		index = (index + 1) % map->size;
	}

	return MAP_FULL;
}

/**
 * Insert given key into the given map.
 * @param m - A pointer to the hashmap.
 * @param key - The key to be inserted into the hashmap.
 * @param amount - The number of occurrences you're willing
 * to insert this key into the hashmap (hashmap_item used -property).
 */
int hashmap_insert(map_t m, char *key, int amount) {
	int index = get_index(m, key);
	int tmp;
	hash_map *map = (hash_map *) m;

	// Expand the hashmap if running out of indexes
	while (index == MAP_FULL) {
		tmp = hashmap_expand(map);
		if (tmp != MAP_OK) return MAP_OMEM;

		index = get_index(map, key);
	}

	if (map->data[index].used == 0) {
		map->data[index].used = amount;
		map->data[index].key = key;
		map->used_size++;
	} else if (map->data[index].used > 0) {
		map->data[index].used += amount;
	}
	return MAP_OK;
}

/**
 * Remove the given key from the hashmap.
 * @param m - The hashmap reference.
 * @param key - The key to be removed from the hasmap.
 * @return - An integer indicating whether the removement
 * was successfull (-4 == MAP_MISSING, -1 = MAP_OK).
 */
int hashmap_remove(map_t m, char *key) {
	hash_map *map = (hash_map *) m;
	int index = get_index(m, key);

	for (int i = 0; i < map->size; i++) {
		if (map->data[index].used > 0 && (strcmp(map->data[index].key, key) == 0)) {
			map->data[index].used = 0;
			map->data[index].key = NULL;
			map->used_size--;
			return MAP_OK;
		}
		index = (index + 1) % map->size;
	}

	return MAP_MISSING;
}

/**
 * Get the pointer of given key into the given pointer.
 * @param m - The hashmap reference.
 * @param key - The key to look for.
 * @param ptr - The pointer in which the hashmap item's key pointer
 * should be inserted into. Please note that this will be set to
 * NULL if the key is not found.
 * @return - An integer indicating if the operation was successful
 * (MAP_MISSING = -3 or MAP_OK = -1).
 */
int hashmap_get_by_key(map_t m, char *key, any_t *ptr) {
	hash_map *map = (hash_map *) m;
	int index = get_index(m, key);
	*ptr = NULL;

	if (index == MAP_FULL) return MAP_MISSING;

	for (int i = 0; i < map->size; i++) {
		if (map->data[index].used > 0 && (strcmp(map->data[index].key, key) == 0)) {
			*ptr = (map->data[index].key);
			return MAP_OK;
		}
		index = (index + 1) % map->size;
	}

	return MAP_MISSING;
}

/**
 * Get the pointer of key at given index into the pointer.
 * @param m - The hashmap reference.
 * @param index - The index of key.
 * @param ptr - The pointer in which the pointer to key is
 * inserted into.
 * @return - Operation result (MAP_OK = -1 if ok, MAP_MISSING = -3
 * otherwise).
 */
int hashmap_get_by_index(map_t m, int index, any_t *ptr) {
	hash_map *map = (hash_map *) m;

	if (index >= map->size) return MAP_MISSING;

	*ptr = (map->data[index].key);
	return MAP_OK;
}

/**
 * Get the current used size of the map.
 * @param m - The hashmap reference.
 * @return - The used size of given hashmap.
 */
int hashmap_get_used_size(map_t m) {
	hash_map *map = (hash_map *) m;
	return map->used_size;
}

/**
 * Return the occurrence of a key in the given map by given index.
 * @param m - The hashmap reference.
 * @param index - The index from where the occurances are seeked from.
 * @return - The number of occurrences in given index.
 */
int hashmap_get_occurrence_by_index(map_t m, int index) {
	hash_map *map = (hash_map *) m;
	if (index >= map->size) return MAP_MISSING;
	return map->data[index].used;
}

/**
 * Get the hashmap max size.
 * @param m - The hashmap reference.
 * @return - The max size of given hashmap.
 */
int hashmap_get_size(map_t m) {
	hash_map *map = (hash_map *) m;
	return map->size;
}


void hashmap_print_key(map_t m, int index) {
	hash_map *map = (hash_map*) m;
	printf("%s\n", map->data[index].key);
}

/**
 * Expand the size of the hashmap into double and rehash all elements.
 * @param m - The hashmap to expand.
 * @return - An integer indicating operation result
 * (MAP_OMEM = -2, MAP_OK = -1).
 */
int hashmap_expand(map_t m) {

	hashmap_item *old;
	int old_size;
	int status;

	// Allocate memory
	hash_map *map = (hash_map *) m;
	hashmap_item *temp = (hashmap_item *) calloc(2 * map->size, sizeof(hashmap_item));

	if (!temp) return MAP_OMEM;

	// Initialize data
	old = map->data;
	map->data = temp;

	// Initialize sizes
	old_size = map->size;
	map->size = 2 * old_size;
	map->used_size = 0;

	for (int i = 0; i < old_size; i++) {
		if (old[i].used == 0) continue;

		status = hashmap_insert(m, old[i].key, old[i].used);
		if (status != MAP_OK) return status;
	}

	free(old);

	return MAP_OK;
}

/**
 * Classical quicksort for hashmap. Sort
 * items in the hashmap by occurrences from
 * low to high.
 * @param m - The hashmap reference.
 * @param low - The lowest index to sort.
 * @param high - The highes index to sort.
 */
void hashmap_quicksort(map_t m, int low, int high) {
	//printf("Starting with %d and %d\n", low, high);
	if (low >= high) return;
	int p = partition(m, low, high);
	hashmap_quicksort(m, low, p - 1);
	hashmap_quicksort(m, p + 1, high);
}

/**
 * Classic partition operation for quicksort.
 * @param m - The hashmap reference.
 * @param low - The lowest index to sort.
 * @param high - The highes index to sort.
 * @return - The index of used pivot.
 */
int partition(map_t m, int low, int high) {
	hash_map *map = (hash_map *) m;
	int l = low - 1;
	hashmap_item pivot = map->data[high];
	for (int j = low; j < high; j++) {
		if (pivot.used > map->data[j].used) {
			l++;
			hashmap_switch_item(&map->data[l], &map->data[j]);
		}
	}
	l++;
	hashmap_switch_item(&map->data[l], &map->data[high]);

	return l;
}

/**
 * Switch the places of two given hashmap_item pointers.
 * @param a - An hashmap_item pointer
 * @param b - An hashmap_item pointer
 */
void hashmap_switch_item(hashmap_item *a, hashmap_item *b) {
	hashmap_item tmp = *b;
	*b = *a;
	*a = tmp;
}
