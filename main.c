/**
 *  main.c
 *  Data structures and algorithms course work
 *
 *  Created by Perttu Kärnä on 24.12.2017.
 *  Copyright © 2018 Perttu Kärnä. All rights reserved.
 *  Test file at /Users/Perttu/Documents/TRA/harjoitustyo/texts/sherlock.txt
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "file_reader/file_reader.h"
#include "hash_map/hash_map.h"

int main(int argc, const char * argv[]) {

	char *file = (char*) malloc(sizeof(char[MAX_WORD_SIZE]));
	clock_t start, end; // For calculating time
	double processing_time;
	long all_words = 0; // For calculating total amount of words in the file
	int occurrence, total_size, map_ok; // Temporary variables
	any_t find; // Pointer for hashmap element value (string)
	map_t map; // Pointer to hashmap

	printf("Please insert the name of the file to scan: ");
	scanf("%s", file);

	int file_exists = check_file_existance(file);

	map = new_hashmap();

	if (!file_exists) {
		printf("File not found, exiting...\n");
		return 0;
	}

	printf("File found! Reading file...\n");

	start = clock(); // Start timestamp of the processing

	FILE *words = fopen(file, "r");
	map_ok = read_words_into_hash_map(words, map);
	fclose(words);

	printf("Reading file completed.\n");

	if (map_ok != MAP_OK) {
		printf("An error occurred during the processing, exiting...");
		return 0;
	}

	printf("Sorting...\n");
	total_size = hashmap_get_size(map);
	// Sort from low to high
	hashmap_quicksort(map, 0, total_size - 1);
	printf("Sorting complete.\n\n");
	end = clock(); // Ending timestamp of the processing

	printf("Printing 100 most common words in the given file and theid occurrences: \n");
	for (int i = total_size - 1; i > total_size - 101; i--) {
		hashmap_get_by_index(map, i, &find);
		occurrence = hashmap_get_occurrence_by_index(map, i);
		printf("%s, %d\n", find, occurrence);
	}

	processing_time = ((double) end - start) / CLOCKS_PER_SEC;
	printf("Reading and sorting time: %lfs\n", processing_time);

	// Calculate total amount of words in the file
	for (int i = 0; i < total_size; i++) {
		all_words += hashmap_get_occurrence_by_index(map, i);
	}
	printf("Total amount of words in the file: %li\n", all_words);

	free(map);
	return 0;
}
