/*
 * Vijay Anand R
 * Bit map library.
 */

#ifndef __BITMAP_H__
#define __BITMAP_H__
#include <stdint.h>

#define BITMAP_HANDLE_NULL NULL
#define BITMAP_SIZE_1K_BITS 1024
#define BITMAP_SNAME_LEN 10

/**
 * Bit map handle 
 */
typedef struct bitmap_t_* bitmap_handle;

/**
 * Retval for apis.
 */
typedef enum bitmap_retval_t_ {
    BITMAP_RETVAL_SUCCESS,
    BITMAP_RETVAL_FAILURE,
    BITMAP_RETVAL_MATCHED,
    BITMAP_RETVAL_MATCH_FAIL,
    BITMAP_RETVAL_INVALID_INPUT,
    BITMAP_RETVAL_INTERNAL_FAILURE
} bitmap_retval_t;

/**
 * Attributed structure to set parameter of a bitmap.
 */
typedef struct bitmap_attr_t_ {
    uint32_t bits_per_block;
    char short_name[BITMAP_SNAME_LEN]; // Name is a mandatory parameter
} bitmap_attr_t;

/**
 * @ bitmap_create
 * Create a bitmap and return a valid handle.
 * retval - SUCCESS if successfully allocated.
 *        - FAILURE if failed to allocate.
 *  if attr == NULL, then default values are assumed.
 */
bitmap_retval_t
bitmap_create(bitmap_handle *handle, 
                              bitmap_attr_t *attr);
/**
 * @ bitmap_destroy
 * Destroy a bitmap
 * retval - SUCCESS if successfully destroyed
 *        - FAILURE if failed to destroy
 */
bitmap_retval_t
bitmap_destroy(bitmap_handle *handle);

/**
 * @ bitmap_set
 * set a bit corresponding to a position
 * retval - SUCCESS if successfully set the bit
 *        - FAILURE if failed to set the bit
 */
bitmap_retval_t
bitmap_set(bitmap_handle handle, 
           uint16_t bitpos);

/**
 * @ bitmap_clear
 * clear a bit corresponding to a position
 * retval - SUCCESS if successfully cleared the bit
 *        - FAILURE if failed to clear the bit
 */
bitmap_retval_t
bitmap_clear(bitmap_handle handle,
             uint16_t bitpos);
/**
 * @ bitmap_clear_all
 * clear a bit corresponding to a position
 * retval - SUCCESS if successfully cleared all the bits
 *        - FAILURE if failed to clear the bits
 */
bitmap_retval_t
bitmap_clear_all(bitmap_handle handle);


/**
 * @ bitmap_check
 * check a bit position.
 * retval - SUCCESS if successfully set the bit
 *        - FAILURE if failed to set the bit
 */
bitmap_retval_t
bitmap_check(bitmap_handle handle, uint16_t bitpos);

/**
 * @ bitmap_get_block_details
 * Get block details.
 *
 * retval - SUCCESS if retrieved all relevant information
 *        - FAILURE if failed to retrieve information.
 */
bitmap_retval_t
bitmap_get_block_details(bitmap_handle handle,
                         uint32_t *bits_per_block,
                         uint32_t *bytes_per_block,
                         uint32_t *words_per_block);

/**
 * @ bitmap_get_block_count
 * Get number of block count.
 *
 * retval - SUCCESS if retreive block count
 *        - FAILURE if failed to retrieve block count.
 */
bitmap_retval_t
bitmap_get_block_count(bitmap_handle handle,
                       uint16_t *count);
/**
 * @bitmap_get_wordsize
 * Get the number of bits in a word
 *
 * retval - SUCCESS if retreive wordsize successfully
 *        - FAILURE if failed to retrieve word size
 */
bitmap_retval_t
bitmap_get_wordsize(uint16_t *size);

/**
 * @bitmap_set_logger_fd
 * Set FD for logger
 */
bitmap_retval_t
bitmap_set_logger_fd(uint16_t fd);

#endif // __BITMAP_H__
