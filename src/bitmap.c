#include "bitmap_internal.h"
#include <string.h>

void* bitmap_malloc (size_t size) 
{
    return (malloc(size));
}
void bitmap_free (void *mem)
{
    free(mem);
}

void
bitmap_set_default_sname (struct bitmap_t_ bitmap_ptr)
{
    char *def_sname = "Bmap-App";
    if (bitmap_ptr) {
        strcpy(bitmap_ptr->short_name, def_sname);
    }
}

void 
bitmap_set_default_values (struct bitmap_t_ bitmap_ptr)
{
    if (!bitmap_ptr) {
        return;
    }
    bitmap_ptr->block_size = BITMAP_SIZE_1K_BITS;
    bitmap_set_default_sname(bitmap_ptr);
}

bitmap_retval_t
bitmap_add_block (struct bitmap_t_ bitmap_ptr)
{
    bitmap_block_t *block_ptr, *new_block;
    uint32_t bytes_required;

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
    bytes_required = bitmap_ptr->block_bits_size/(sizeof(bitmapword_t) * BYTE);
    new_block->bitmap = (bitmapword_t *) bitmap_malloc(bytes_required);

    if (!new_block->bitmap) {
        bitmap_free(new_block);
        return BITMAP_RETVAL_INTERNAL_FAILURE;
    }

    if (block_ptr == NULL) {
        // This is the first block in the bitmap.
        bitmap_ptr->count = 1;

    }

    while (block_ptr && block_ptr->next) {

    }

}

bitmap_retval_t 
bitmap_create (bitmap_handle *handle,
               bitmap_attr_t *attr)
{
    struct bitmap_t_ *bitmap_ptr;
    bitmap_retval_t retval;

    if (!handle) {
        // If the input handle is null.
        return (BITMAP_RETVAL_INVALID_INPUT);
    }

    if (attr) {
        if (attr->block_bits_size % BYTE != 0) {
            return (BITMAP_RETVAL_INVALID_INPUT);
        }
    }

    bitmap_ptr = (struct bitmap_t_ *) 
              bitmap_malloc(sizeof(struct bitmap_t));

    if (!bitmap_ptr) {
        // Memory allocation to the bitmap ptr failed.
        return (BITMAP_RETVAL_INTERNAL_FAILURE);
    }

    bitmap_ptr->block_count = 0;

    if (attr) {
        size_t len;
        bitmap_ptr->block_size = attr->block_size;
        len = strnlen(attr->short_name, BITMAP_SNAME_LEN);
        if ((len < BITMAP_SNAME_LEN) && 
            (attr->short_name[len] == '\0')) {
            strncpy(bitmap_ptr->short_name, attr->short_name, len);
        } else {
            bitmap_set_default_sname(bitmap);
        }
    } else {
        bitmap_set_default_values(bitmap);
    }


    *handle = bitmap_ptr;
    retval = bitmap_add_block(bitmap_ptr);

    if (retval != BITMAP_RETVAL_SUCCESS) {
        *handle = NULL;
        bitmap_free(bitmap_ptr);
    } 

    return retval;
}
