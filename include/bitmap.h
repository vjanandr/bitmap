/*
 * Vijay Anand R
 * Bit map library.
 */

#ifndef __BITMAP_H__
#define __BITMAP_H__

#define BITMAP_HANDLE_INIT NULL
#define BITMAP_SIZE_1K_BITS 1024
#define BITMAP_SNAME_LEN 10

/**
 * Bit map handle 
 */
typedef struct bitmap_t_* bitmap_handle;

/**
 * Retval for apis.
 */
typedef bitmap_retval_t_ {
    BITMAP_RETVAL_SUCCESS,
    BITMAP_RETVAL_FAILURE,
    BITMAP_RETVAL_INVALID_INPUT,
    BITMAP_RETVAL_INTERNAL_FAILURE
} bitmap_retval_t;

/**
 * Attributed structure to set parameter of a bitmap.
 */
typedef struct bitmap_attr_t_ {
    uint32_t block_bits_size;
    char short_name[BITMAP_SNAME_LEN]; // Name is a mandatory parameter
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

/**
 * @ bitmap_set
 * set a bit corresponding to a position
 * retval - SUCCESS if successfully set the bit
 *        - FAILURE if failed to set the bit
 */
bitmap_retval_t bitmap_set(bitmap_handle handle, 
                           uint16_t bitpos);

/**
 * @ bitmap_clear
 * clear a bit corresponding to a position
 * retval - SUCCESS if successfully cleared the bit
 *        - FAILURE if failed to clear the bit
 */
bitmap_retval_t bitmap_clear(bitmap_handle handle,
                           uint16_t bitpos);

/**
 * @ bitmap_check_bit_pos
 * check a bit position.
 * retval - SUCCESS if successfully set the bit
 *        - FAILURE if failed to set the bit
 */
bool bitmap_check_bit_pos(bitmap_handle handle);

#endif // __BITMAP_H__
                          
