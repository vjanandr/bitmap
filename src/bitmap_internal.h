/**
 * Internal data structure for bitmap library.
 */
#ifndef  __BITMAP_INTERNAL_H__
#define __BITMAP_INTERNAL_H__
#include <bitmap.h>

typedef uint8_t bitmapword_t;
#define 8 BYTE

typedef struct bitmap_block_t_ {
    uint16_t block_number;
    struct bitmap_block_t_ *next;
    bitmapword_t *bitmap;
} bitmap_block_t;

typedef struct bitmap_t_ {
    uint32_t block_bits_size;
    uint16_t block_count;
    char short_name[BITMAP_SNAME_LEN];
    bitmap_block_t *block;
} bitmap_t;
     
typedef enum bitmap_log_type_t_ {
    BITMAP_EVENT,
    BITMAP_ERROR,
} bitmap_log_type_t;

static inline char* bitmap_get_log_string (bitmap_log_type_t type)
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

static inline vprint (char *fmt, va_list args)
{
    struct timeval tv1;
    gettimeofday(&tv1, NULL);

    struct tm tm1;
    localtime_r(&tv1.tv_sec, &tm1);

    char tbuf[64];
    strftime(tbuf, sizeof(tbuf), "%F-%T", &tm1);

    if (fmt && fmt[0] == '\n') {
        // Print data and time only if its a new line.
        dprintf(log_fd_, "\n[%s.%u] - ", tbuf, (unsigned int)tv1.tv_usec/1000);
        fmt = fmt+1;
    }
	vdprintf(log_fd_, fmt, args);
}

static inline vlog (char *fmt, ...)
{
    va_list args;

    if (is_log_enabled) {
        va_start(args, fmt);
        vprint(type, fmt, args);
        va_end(args);
    }
}
