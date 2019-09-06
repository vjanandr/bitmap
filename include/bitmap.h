/*
 * Vijay Anand R
 * Bit map library.
 */

#ifndef __BITMAP_H__
#define __BITMAP_H__

#define BITMAP_INIT_HANDLE NULL
#define BITMAP_SIZE_1K_BITS 1024

typedef struct bitmap_t* bitmap_handle;
typedef bitmap_retval_t_ {
    BITMAP_RETVAL_SUCCESS,
    BITMAP_RETVAL_FAILURE,
} bitmap_retval_t;

typedef struct bitmap_attr_t_ {
    uint32_t block_size;
} bitmap_attr_t;

/**
 * @ bitmap_create
 * Create a bitmap and return a valid handle.
 * retval - SUCCESS if successfully allocated.
 *        - FAILURE if failed to allocate.
 *  if attr == NULL, then default values are assumed.
 */
bitmap_retval_t bitmap_create(bitmap_handle *handle, 
                              bitmap_attr_t *attr);
/**
 * @ bitmap_destroy
 * Destroy a bitmap
 * retval - SUCCESS if successfully destroyed
 *        - FAILURE if failed to destroy
 */
bitmap_retval_t bitmap_destroy(bitmap_handle handle);
