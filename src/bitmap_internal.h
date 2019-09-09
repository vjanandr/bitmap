/**
 * Vijay Anand R
 * Internal data structure for bitmap library.
 */
#ifndef  __BITMAP_INTERNAL_H__
#define __BITMAP_INTERNAL_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <bitmap.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

#define VALID_MEM 0xABCD
#define DEAD_MEM 0xDEAD

typedef uint8_t bitmapword_t;
#define BYTE 8

typedef struct bitmap_block_t_ {
    uint16_t block_number;
    struct bitmap_block_t_ *next;
    bitmapword_t *bitmap;
} bitmap_block_t;

typedef struct bitmap_t_ {
    uint32_t bits_per_block;
    uint32_t bytes_per_block;
    uint32_t words_per_block;
    uint16_t block_count;
    uint16_t mem_sanity;
    char short_name[BITMAP_SNAME_LEN];
    bitmap_block_t *block;
} bitmap_t;
     
typedef enum bitmap_log_type_t_ {
    BITMAP_EVENT,
    BITMAP_ERROR,
} bitmap_log_type_t;

static inline 
char* bitmap_get_log_string (bitmap_log_type_t type)
{
    switch (type) {
        case BITMAP_EVENT:
            return "EVENT";
        case BITMAP_ERROR:
            return "ERROR";
        default:
            return "DEFAULT";
    }
}

#endif
