# Data structures and algorithms

**2017-2018**

This project contains an implementation of coursework for data structures and algorithms course at the University of Oulu.

### Table of contents
- [Problem](#problem)
- [Solution](#solution)
- [Data structures](#data-structures)
- [Performance analysis](#performance-analysis)
    - [Theoretical analysis](#theoretical-analysis)
        - [hashmap_hash](#hashmap_hash)
        - [get_index](#get_index)
        - [hashmap_expand](#hashmap_expand)
        - [read_words_into_hash_map](#read_words_into_hash_map)
        - [hashmap_quicksort](#hashmap_quicksort)
    - [Tests](#tests)
    - [Observations](#observations)


## Problem

In this assignment, you shall find the 100 most frequently occurring words from a large text file. The program shall be implemented in C language. A continuous string of characters a..z and A..Z, with possible apostrophes ’, is considered a word. You can also include characters å,ä, ö, Å,Ä, and Ö if you wish, but this is not obligatory. Words with uppercase and lowercase letters are considered equal. For example, in the text

Herman Melville’s book Moby Dick starts, as we all know, with the sentence ”Call me Ishmael”. the words are

herman, melville’s, book, moby, dick, starts, as, we, all, know, with, the, sentence, call, me, and ishmael

The name of the text file can be either given as a command-line argument or as an input from the user. The program prints the 100 most frequently occurring words and their frequencies in the file. The words are printed in descending order according to their frequencies.

## Solution

The solution is divided into two modules; file_reader and hash_map. The files in the [file_reader](file_reader) -directory contain all file-related operations, such as file existence checker and function for reading words from a file into a hash map. The files in the [hash_map](hash_map) -directory contain all hash map -related operations, such as constructor, element inserting and sorting the hash map. The hash_map API is implemented via `map_t` -pointer and keys of elements in the hash map can be accessed via `any_t` -pointer.

See example of hash map API usage below.

```c
#include "hash_map/hash_map.h"

map_t map = new_hashmap();
char *word = (char*) malloc(sizeof(char[MAX_WORD_SIZE]));
/*
 ... read a string into word
*/
int map_state = hashmap_insert(map, word, 1);
/*
map_state is
MAP_MISSING -4    No such element
MAP_FULL -3 	    Hashmap is full
MAP_OMEM -2 	    Out of Memory
MAP_OK -1 	      OK, insert successful
*/

```

An example with file_reader API.

```c
#include "hash_map/hash_map.h"
#include "file_reader/file_reader.h"

map_t map = new_hashmap();
char *file = (char*) malloc(sizeof(char[MAX_WORD_SIZE]));
/*
 ... read a path into the file
*/
int valid_file = check_file_existance(file);
if (!valid_file) {
  printf("No such file, exiting...\n");
  return 0;
}
FILE *text = fopen(file, "r");
int status = read_words_into_hash_map(text, map);
/*
status is
MAP_MISSING -4    No such element
MAP_FULL -3 	    Hashmap is full
MAP_OMEM -2 	    Out of Memory
MAP_OK -1 	      OK, operation successful
*/
```


## Data structures

This program uses hash map for storing the words and their occurrences in the file. The map is being abstracted from the user via pointer API (map_t type). The map has elements, each of which have properties char* (string) key and int occurrence (default 0). The map has properties int used_size (element counter), int size (total size of the map) and a pointer to the elements.

When new elements do not fit in the hash map, it gets expanded. During this process all the elements in the map will get rehashed since the indexing operation uses the map's size property.

It's worth to mention that once the hash map is sorted (with quicksort), it doesn't work in the way expected. After that it is strictly read-only structure.

## Performance analysis

### Theoretical analysis

The main relevant functions to analyze in this program are functions read_words_into_hash_map (file_reader), hashmap_insert (hash_map) and hashmap_quicksort (hash_map). All funtions they are using are listed below in the table with performance analysis on each function.

|Name                    |Header file  |Input             |Performance|
|------------------------|-------------|------------------|-----------|
|hashmap_hash            |hash_map.h   |str (length n)    |θ(n)       |
|get_index               |hash_map.h   |str, map (size m) |O(1)       |
|hashmap_expand          |hash_map.h   |map (size m)      |O(m)       |
|hashmap_insert          |hash_map.h   |str, map (size m) |θ(n)       |
|read_words_into_hash_map|file_reader.h|file, map         |θ(n)       |
|hashmap_quicksort       |hash_map.h   |map (size m)      |O(m log m) |

#### hashmap_hash

This function's performance is strictly related to the input key size, leading to linear growth of execution time (θ(n)). The hash function is djb2.

#### get_index

This function performs `hashmap_hash` and then does linear probing over the whole map, which results to a performance of O(m) (m being the size of the map) according to [Wikipedia](https://en.wikipedia.org/wiki/Linear_probing).

#### hashmap_expand

This function doubles the size of the hash map. All elements in the hash map are re-hashed and inserted again into the hashmap, which leads to a performance of O(m), m being the size of the old hashmap.

#### read_words_into_hash_map

This function reads words from a file into given hash map. The file is read character by character, which basically leads to a performance of θ(n) (n being the amount of characters in the file).

#### hashmap_quicksort

Very basic implement of quicksort algorithm. The performance is O(m log m) on average, where m is the size of the hash map.

### Tests

The following table contains information about the results of five different books, some of which are commonly known novels, processed with this program. Each file was downloaded from [Project Gutenber website](https://www.gutenberg.org/) and each of them was .txt -files. Please not that the word count is not verified from any external sources; it's calculated only by this program.


| Title                                     | Author           |File size|Words |Processing time|
|-------------------------------------------|------------------|---------|------|---------------|
|The Adventures of Sherlock Holmes          |Arthur Conan Doyle|595 kB   |107734|220,051ms      |
|Moby Dick; or The Whale                    |Herman Melville   |1,3 MB   |215604|588,287ms      |
|Ulysses                                    |James Joyce       |1.6 MB   |267189|701,687ms      |
|Analysis of the Phenomena of the Human Mind|James Mill        |1.6 MB   |266037|350,823ms      |
|War and Peace                              |Leo Tolstoy       |3.4 MB   |565669|896,491ms      |


The following table contains the processing time on the second run, respectively.

| Title                                     |Processing time|
|-------------------------------------------|---------------|
|The Adventures of Sherlock Holmes          |244,8...ms     |
|Moby Dick; or The Whale                    |588,454ms      |
|Ulysses                                    |691,153ms      |
|Analysis of the Phenomena of the Human Mind|364,232ms      |
|War and Peace                              |896,717ms      |

### Observations

The common pattern seems to be that the processing time increases while the total amount of words in the file increases. However, there is one significant exception in the results. Since the data structure for storing words is hash map, the amount of time taken by insert into hash map -operation is not constant, thus leading to the results of "Analysis of the Phenomena of the Human Mind" to be remarkably different from "Ulysses". Both of them are about equal size files with about equal amount of words. Still, it takes about twice as long to process the latter.

A table below shows the word/ms rate on each file when calculating with overall process time and word count.

| Title                                     | Word/ms |
|-------------------------------------------|---------|
|The Adventures of Sherlock Holmes          |489,6    |
|Moby Dick; or The Whale                    |366,5    |
|Ulysses                                    |380,8    |
|Analysis of the Phenomena of the Human Mind|758,3    |
|War and Peace                              |631,0    |
|Avg.                                       |525,2    |

Since the quicksort function is the limiting algorithm in this implementation, the variety of words will most likely be the limiting part of the program. However, by calculating the average word/ms rate, we can make some estimations about max file sizes this program can handle in reasonable time.

Let's make the time limit for calculations to 10 seconds.

Thus, when file is written in english (5,1 chars per word on avg.), the file size can be calculated:

525,2 words/ms * 10 (s) * 1000 ms = 5 252 000 words

File size 5252000 * 5,1 (chars) * 1 byte = 26 785 200 bytes

===> About 26,8 MB

Thus, we can estimate, that on average, the program will process files less than 30 MB in reasonable time (most likely under 15 seconds).
