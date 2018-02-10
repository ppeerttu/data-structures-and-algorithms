/**
 *  file_reader.c
 *  Data structures and algorithms course work
 *
 *  Created by Perttu Kärnä on 24.12.2017.
 *  Copyright © 2018 Perttu Kärnä. All rights reserved.
 */

#include "file_reader.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Check if a file exists at given path (and can be read)
 * @param path - Absolute path to the file (i.e. /home/.../file.txt)
 * @return 1 if file exists, 0 otherwisse
 */
int check_file_existance(char path[]) {
	FILE * file;
	if ((file = fopen(path, "r")) != NULL) {
		fclose(file);
		return 1;
	}
	return 0;
}

/**
 * Read and print words from a file one by one.
 * @param file - The file to read fromd
 */
void print_words_from_file(FILE * file) {
	char word [1024];
	while (fscanf(file, " %1203s", word) == 1) {
		puts(word);
	}
}

/**
 * Read words from a given file to given hash map.
 * A word is lowercased and parsed out of dots and commas.
 * Some rare characters may pass the parsing as a part
 * of the word.
 * @param file - The file to read from
 * @param map - The hash map for storing the words
 * @return - MAP_OK if all ok, fail code otherwise
 * (MAP_OMEM most likely)
 */
int read_words_into_hash_map(FILE * file, map_t map) {
	char *word = (char*) malloc(sizeof(char[MAX_WORD_SIZE]));
	char *tmp = (char*) malloc(sizeof(char));
	int map_state;

	while (fscanf(file, "%c", tmp) == 1) {

		if (tmp[0] == '.' || tmp[0] == ',' || isspace(tmp[0]) || tmp[0] == '"') {
			if (strlen(word) > 0) {
				map_state = hashmap_insert(map, word, 1);
				if (map_state != MAP_OK) return map_state;
				word = (char*) malloc(sizeof(char[MAX_WORD_SIZE]));
			}
			// NOOP
		} else if (isalpha(tmp[0]) || tmp[0] == '\'') {
			tmp[0] = tolower((unsigned char) tmp[0]);
			strcat(word, tmp);
		}
	}
	free(word);
	free(tmp);
	return MAP_OK;
}
