#ifndef _HEADER_STR_
#define _HEADER_STR_

#include <stdlib.h>
#include <stdio.h>
#define _MAX_STACK_SIZE 2

typedef struct
{
	char* buffer;
	size_t size;
} str;

#define null_str ((str){ 0, 0 })

typedef str* str_p;

// destroy a string
void free_str(str_p p);
// create a new string, return NULL if fail
str_p new_str(const char* string);
// hard copy construction, return NULL if fail
str_p cpy_str(const str_p p);
// get the size
size_t sizeof_buff(const char* string);
// return the size
size_t size_str(const str_p p);
// create and initialize an str, return NULL if fail
str_p zero_str(size_t size);
// generate a str with elements in reverse, return NULL if fail
str_p revs_str(const str_p p);
// overwrite a string, return NULL if fail
str_p overwrite_str(str_p p, const char* string);
// swap
void swp_str(str_p p, str_p q);
// compare lexicographically
int cmp_str(const str_p p, const str_p q);
// compare lexicographically
int cmp_charArray_(const char* p1, const char* p2);
// read a whole file into str, return NULL if fail
str_p readin_file_str(FILE* fp);
// write string to file
int write2_file_str(FILE* fp, const str_p p);
// sort the string between index low to index high lexicographically
str_p nsort_str(str_p p, size_t low, size_t high);
// sort the string lexicographically
str_p sort_str(str_p p);
// search
char* srh_str(const str_p string_origin, const char* substring);
// check state and update the size
size_t chk_str(str_p p);

// append a string to generate a new one (very easy to cause memory leak!), return NULL if fail
str_p append_str(str_p p, const char* string);
/*
safe way to use append_str:
str_p q = append_str(p, string);
free_str(p); p need to be freed if it is of no use later
*/

//swap the position of two characters within a string, index start from 0
str_p swap_chr(str_p p, unsigned long index1, unsigned long index2);

// compare two memory space
int memcmp_str(const char* s1, const char* s2, size_t n);
size_t partition(char* arr, size_t low, size_t high);
void quickSort(char* arr, size_t low, size_t high);

//recursive quicksort leads to stack overflow when go too deep
//need a simple stack data structure for non-recursive quicksort
//需要一个case insensitive的cmpstr用在http里


#endif /* _HEADER_STR_ */