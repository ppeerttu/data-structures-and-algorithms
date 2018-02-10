/**
 * file_reader.h
 * Data structures and algorithms course work
 *
 * Created by Perttu Kärnä on 24.12.2017.
 * Copyright © 2018 Perttu Kärnä. All rights reserved.
 *
 * Header file for file_reader. Please see  file_reader.c
 * for documentation.
 */

#ifndef file_reader_h
#define file_reader_h

#include <stdio.h>
#include "../hash_map/hash_map.h"

#define MAX_WORD_SIZE 256

int check_file_existance(char file[]);

void print_words_from_file(FILE * file);

int read_words_into_hash_map(FILE * file, map_t map);

#endif /* file_reader_h */
