#include "bitmap_internal.h"
#include <math.h>
#include <string.h>

//int log_fd_ = STDOUT_FILENO;

#define BITMAP_GET_BLOCK_NUMBER(bitmap_ptr, bitpos) \
        (int)(bitpos / bitmap_ptr->bits_per_block)

#define BITMAP_GET_BLOCK_WORDPOS(bitmap_ptr, bitpos) \
        (int)((bitpos % bitmap_ptr->bits_per_block) / sizeof(bitmapword_t))

#define BITMAP_GET_BLOCK_WORDPOS_OFFSET(bitmap_ptr, bitpos) \
        (int)((bitpos % bitmap_ptr->bits_per_block) %  \
                 sizeof(bitmapword_t))

void* bitmap_malloc (size_t size) 
{
    return (malloc(size));
}
void bitmap_free (void *mem)
{
    free(mem);
}

void
bitmap_set_default_sname (struct bitmap_t_ *bitmap_ptr)
{
    char *def_sname = "Bmap-App";
    if (bitmap_ptr) {
        strcpy(bitmap_ptr->short_name, def_sname);
    }
}

void 
bitmap_set_default_values (struct bitmap_t_ *bitmap_ptr)
{
    if (!bitmap_ptr) {
        return;
    }
    bitmap_ptr->bits_per_block = BITMAP_SIZE_1K_BITS;
    bitmap_set_default_sname(bitmap_ptr);
}

bitmap_retval_t
bitmap_add_block (struct bitmap_t_ *bitmap_ptr)
{
    bitmap_block_t *block_ptr, *new_block;

    if (!bitmap_ptr) {
        return BITMAP_RETVAL_INVALID_INPUT;
    }

    block_ptr = bitmap_ptr->block;
    new_block = (bitmap_block_t *)bitmap_malloc(sizeof(bitmap_block_t));

    if (!new_block) {
        return BITMAP_RETVAL_INTERNAL_FAILURE;
    }

    new_block->next = NULL;
    // Calculate the number of bytes required, this is same as sizeof.
    new_block->bitmap = (bitmapword_t *) 
        bitmap_malloc(bitmap_ptr->bytes_per_block);
    // Number of words x sizeof a word
    memset(new_block->bitmap, 0, 
          bitmap_ptr->bytes_per_block);

    if (!new_block->bitmap) {
        bitmap_free(new_block);
        return BITMAP_RETVAL_INTERNAL_FAILURE;
    }

    if (block_ptr == NULL) {
        // This is the first block in the bitmap.
        bitmap_ptr->block_count = 1;
        bitmap_ptr->block = new_block;
        new_block->block_number = bitmap_ptr->block_count - 1;
    } else {
        // Most recent data is most likely to be accessed often.
        // So insert in the head.
        new_block->next = bitmap_ptr->block;
        bitmap_ptr->block = new_block;
        new_block->block_number = bitmap_ptr->block_count;
        bitmap_ptr->block_count = bitmap_ptr->block_count+1;
    }
    return BITMAP_RETVAL_SUCCESS;
}

bitmap_retval_t 
bitmap_create (bitmap_handle *handle,
               bitmap_attr_t *attr)
{
    struct bitmap_t_ *bitmap_ptr;
    bitmap_retval_t retval;

    if (!handle) {
        // If the input handle is null.
        vlog("\n%s [%s] Invalid handle",
                bitmap_get_log_string(BITMAP_ERROR),
                __FUNCTION__);
        return (BITMAP_RETVAL_INVALID_INPUT);
    }

    if (attr) {
        if (attr->bits_per_block % (sizeof(bitmapword_t) * BYTE) != 0) {
            vlog("\n%s [%s] Invalid bits per block config ",
                bitmap_get_log_string(BITMAP_ERROR),
                __FUNCTION__);
            return (BITMAP_RETVAL_INVALID_INPUT);
        }
    }

    bitmap_ptr = (struct bitmap_t_ *) 
              bitmap_malloc(sizeof(struct bitmap_t_));

    if (!bitmap_ptr) {
        // Memory allocation to the bitmap ptr failed.
        vlog("\n%s [%s] memory allocation failed bitmap_ptr",
              bitmap_get_log_string(BITMAP_ERROR),
              __FUNCTION__);
        return (BITMAP_RETVAL_INTERNAL_FAILURE);
    }
    memset(bitmap_ptr, 0, sizeof(struct bitmap_t_));

    bitmap_ptr->block_count = 0;

    if (attr) {
        size_t len;
        bitmap_ptr->bits_per_block = attr->bits_per_block;
        len = strnlen(attr->short_name, BITMAP_SNAME_LEN);
        if ((len < BITMAP_SNAME_LEN) && 
            (attr->short_name[len] == '\0')) {
            strncpy(bitmap_ptr->short_name, attr->short_name, len);
        } else {
            vlog("\n%s [%s] Name string outof bounds default name",
                  bitmap_get_log_string(BITMAP_EVENT),
                  __FUNCTION__);
            bitmap_set_default_sname(bitmap_ptr);
        }
    } else {
        bitmap_set_default_values(bitmap_ptr);
    }
            

    // Calculate the number of bytes required, this is same as sizeof.
    bitmap_ptr->words_per_block = ceil(
            bitmap_ptr->bits_per_block/(sizeof(bitmapword_t) * BYTE));
    bitmap_ptr->bytes_per_block = ceil(bitmap_ptr->bits_per_block/BYTE);
    *handle = bitmap_ptr;
    // By default add a block
    retval = bitmap_add_block(bitmap_ptr);

    if (retval != BITMAP_RETVAL_SUCCESS) {
        *handle = NULL;
        bitmap_free(bitmap_ptr);
    }
    vlog("\n%s [%s] sname %s, mapword length %d "
            "bits_per_block %d, bytes_per_block %d,"
            " words_per_block %d, block_count %d  \n",
            bitmap_get_log_string(BITMAP_EVENT), 
            __FUNCTION__, bitmap_ptr->short_name,
            sizeof(bitmapword_t),
            bitmap_ptr->bits_per_block, 
            bitmap_ptr->bytes_per_block,
            bitmap_ptr->words_per_block,
            bitmap_ptr->block_count);
    return retval;
}

bitmap_block_t* 
bitmap_get_block (struct bitmap_t_ *bitmap_ptr, 
                  uint16_t bitpos)
{
    bitmap_block_t *block = NULL;
    int block_number = BITMAP_GET_BLOCK_NUMBER(bitmap_ptr, bitpos);

    if (bitmap_ptr) {
        while (block) {
            if (block->block_number == block_number) {
                break;
            }
            block = block->next;
        }
        return block;
    }
    return NULL;
}

bitmap_retval_t
bitmap_set (struct bitmap_t_ *bitmap_ptr, uint16_t bitpos)
{
    int block_number, word_pos, word_pos_offset;
    bitmap_retval_t retval;
    bitmap_block_t *block = NULL;
    bitmapword_t b_one = 0x01;

    block_number = word_pos_offset = word_pos = 0;
    if (!bitmap_ptr) {
        return BITMAP_RETVAL_INVALID_INPUT;
    }
    block_number = BITMAP_GET_BLOCK_NUMBER(bitmap_ptr, bitpos);
    while (bitmap_ptr->block_count - (block_number+1)) {
        // block count is one more than block number.
        // Block number starts from 1, block count starts from 0
        retval = bitmap_add_block(bitmap_ptr);
    }
    block = bitmap_get_block(bitmap_ptr, bitpos);
    word_pos = BITMAP_GET_BLOCK_WORDPOS(bitmap_ptr, bitpos);
    word_pos_offset = BITMAP_GET_BLOCK_WORDPOS_OFFSET(bitmap_ptr, bitpos);
    block->bitmap[word_pos] = block->bitmap[word_pos] | (b_one << word_pos_offset); 
    return BITMAP_RETVAL_SUCCESS;
}

bitmap_retval_t
bitmap_check (struct bitmap_t_ *bitmap_ptr, uint16_t bitpos)
{
    int block_count, word_pos, word_pos_offset;
    bitmap_block_t *block = NULL;
    bitmapword_t b_one = 0x01;

    block_count = word_pos_offset = word_pos = 0;
    if (!bitmap_ptr) {
        return BITMAP_RETVAL_INVALID_INPUT;
    }

    block = bitmap_get_block(bitmap_ptr, bitpos);
    if (!block) {
        // Such a bit was never set.
        return BITMAP_RETVAL_MATCH_FAIL;
    }
    word_pos = BITMAP_GET_BLOCK_WORDPOS(bitmap_ptr, bitpos);
    word_pos_offset = BITMAP_GET_BLOCK_WORDPOS_OFFSET(bitmap_ptr, bitpos);
    if (block->bitmap[word_pos] &  (b_one << word_pos_offset) ) {
        return BITMAP_RETVAL_MATCHED;
    }
    return BITMAP_RETVAL_MATCH_FAIL;
}

bitmap_retval_t
bitmap_clear (struct bitmap_t_ *bitmap_ptr, uint16_t bitpos)
{
    int block_count, word_pos, word_pos_offset;
    bitmap_block_t *block = NULL;
    bitmapword_t b_one = 0x01;

    block_count = word_pos_offset = word_pos = 0;
    if (!bitmap_ptr) {
        return BITMAP_RETVAL_INVALID_INPUT;
    }

    block = bitmap_get_block(bitmap_ptr, bitpos);
    if (!block) {
        // Such a bit was never set.
        return BITMAP_RETVAL_INVALID_INPUT;
    }
    word_pos = BITMAP_GET_BLOCK_WORDPOS(bitmap_ptr, bitpos);
    word_pos_offset = BITMAP_GET_BLOCK_WORDPOS_OFFSET(bitmap_ptr, bitpos);
    block->bitmap[word_pos] = block->bitmap[word_pos] & (~(b_one << word_pos_offset)); 
    return BITMAP_RETVAL_SUCCESS;
}

bitmap_retval_t
bitmap_destroy (struct bitmap_t_ **bitmap_ptr)
{
    struct bitmap_t_ *ptr;
    bitmap_block_t *block, *curr_block;

    if (!bitmap_ptr || !*bitmap_ptr) {
        return BITMAP_RETVAL_INVALID_INPUT;
    }
    
    ptr = *bitmap_ptr;
    block = ptr->block;

    while (block) {
        curr_block = block;
        block = block->next;
        curr_block->next = NULL;
        bitmap_free(curr_block->bitmap);
        bitmap_free(curr_block);
    }
    ptr->block = NULL;
    bitmap_free(ptr);
    *bitmap_ptr = NULL;
    return BITMAP_RETVAL_SUCCESS;
}

bitmap_retval_t
bitmap_clear_all (struct bitmap_t_ *bitmap_ptr)
{
    bitmap_block_t *block;
    if (!bitmap_ptr) {
        return BITMAP_RETVAL_INVALID_INPUT;
    }
    
    block = bitmap_ptr->block;

    while (block) {
        memset(block->bitmap, 0, bitmap_ptr->bytes_per_block);
        block = block->next;
    }
    return BITMAP_RETVAL_SUCCESS;
}

bitmap_retval_t
bitmap_get_block_details (struct bitmap_t_ *bitmap_ptr, 
                          uint32_t *bits_per_block,
                          uint32_t *bytes_per_block,
                          uint32_t *words_per_block)
{
    if (!bitmap_ptr || 
        (!bits_per_block && !bytes_per_block && !words_per_block)) {
        return BITMAP_RETVAL_INVALID_INPUT;
    }
    if (bits_per_block)
        *bits_per_block = bitmap_ptr->bits_per_block;
    if (bytes_per_block)
        *bytes_per_block = bitmap_ptr->bytes_per_block;
    if (words_per_block)
        *words_per_block = bitmap_ptr->words_per_block;

    return BITMAP_RETVAL_SUCCESS;
}

bitmap_retval_t
bitmap_get_block_count (struct bitmap_t_ *bitmap_ptr,
                        uint16_t *count)
{
    if (!bitmap_ptr || !count) {
        return BITMAP_RETVAL_INVALID_INPUT;
    }
    *count = bitmap_ptr->block_count;
    return BITMAP_RETVAL_SUCCESS;
}
