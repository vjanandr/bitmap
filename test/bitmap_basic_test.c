/**
 * API testing of BITMAP
 */
#include <bitmap.h>
#include <CUnit/Basic.h>

bitmap_handle handle1;
bitmap_handle handle2;
bitmap_handle handle3;

void bitmap_basic_1_create ()
{
    bitmap_handle handle1_temp;
    bitmap_retval_t retval;
    bitmap_attr_t attr;
    uint32_t bits_per_block, bytes_per_block, words_per_block;
    uint16_t count;

    bits_per_block = bytes_per_block = words_per_block = 0;

    memset(&attr, 0, sizeof(bitmap_attr_t));
    // Set the proper values.
    strcpy(attr.short_name, "handle-1");
    attr.bits_per_block = BITMAP_SIZE_1K_BITS * 2;
    retval = bitmap_create(&handle1, &attr);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);

    memset(&attr, 0, sizeof(bitmap_attr_t));
    // Set the proper values.
    strcpy(attr.short_name, "handle-2");
    attr.bits_per_block = BITMAP_SIZE_1K_BITS;
    retval = bitmap_create(&handle2, &attr);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_get_block_details(handle1, 
                                      &bits_per_block, 
                                      NULL, NULL);

    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(bits_per_block == 2048);

    retval = bitmap_get_block_details(handle1, 
                                      NULL,
                                      &bytes_per_block, NULL);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(bytes_per_block == 256);

    retval = bitmap_get_block_details(handle1, 
                                      NULL,
                                      NULL, &words_per_block);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(words_per_block == 256);

    bits_per_block = bytes_per_block = words_per_block = 0;

    retval = bitmap_get_block_details(handle1, 
                                      &bits_per_block,
                                      &bytes_per_block, 
                                      &words_per_block);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(bits_per_block == 2048);
    CU_ASSERT_TRUE(bytes_per_block == 256);
    CU_ASSERT_TRUE(words_per_block == 256);
    count = 100;
    retval = bitmap_get_block_count(handle1,
                                    &count);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(count == 1);

    bits_per_block = bytes_per_block = words_per_block = 0;

    // Check default parameters.
    retval = bitmap_create(&handle1_temp, NULL);
    retval = bitmap_get_block_details(handle1_temp, 
                                      &bits_per_block,
                                      &bytes_per_block, 
                                      &words_per_block);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(bits_per_block == 1024);
    CU_ASSERT_TRUE(bytes_per_block == 128);
    CU_ASSERT_TRUE(words_per_block == 128);
    retval = bitmap_get_block_count(handle1_temp,
                                    &count);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(count == 1);
    retval = bitmap_destroy(&handle1_temp);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    // Negative test case
    retval = bitmap_get_block_details(BITMAP_HANDLE_NULL,
                                      &bits_per_block,
                                      &bytes_per_block, 
                                      &words_per_block);
    CU_ASSERT_TRUE(retval != BITMAP_RETVAL_SUCCESS);
    retval = bitmap_get_block_details(handle1,
                                      NULL,
                                      NULL,
                                      NULL);
    CU_ASSERT_TRUE(retval != BITMAP_RETVAL_SUCCESS);

    strcpy(attr.short_name, "h1_temp");
    attr.bits_per_block = 12;
    retval = bitmap_create(&handle1_temp, &attr);
    CU_ASSERT_TRUE(retval != BITMAP_RETVAL_SUCCESS);

    strcpy(attr.short_name, "h1_tempabcdef");
    attr.bits_per_block = 1024;
    retval = bitmap_create(&handle1_temp, &attr);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_destroy(&handle1_temp);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
}

void bitmap_basic_1_set ()
{
}
void bitmap_basic_1_check ()
{
   // bitmap_retval_t retval;
}

void bitmap_basic_1_clear() 
{
}

void bitmap_basic_1_clear_all() 
{
}
void bitmap_basic_1_destroy() 
{
}
int main ()
{
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    CU_pSuite  psuite = NULL;

    psuite = CU_add_suite("Bitmap basic test", NULL, NULL);
    if (psuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(psuite, "bitmap_basic_1_create",
                    bitmap_basic_1_create) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(psuite, "bitmap_1_set",
                bitmap_basic_1_set) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_1_check",
                bitmap_basic_1_check) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_1_clear",
                bitmap_basic_1_clear) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_1_clear_all",
                bitmap_basic_1_clear_all) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_1_destroy",
                bitmap_basic_1_destroy) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /*
    if (CU_add_test(psuite, "bitmap_1_check_negative",
                bitmap_1_set) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_1_clear_negative",
                bitmap_1_set) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_1_clear",
                bitmap_1_set) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_1_set_grow",
                bitmap_1_set) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(psuite, "bitmap_1_check_grow",
                bitmap_1_set) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_1_check_grow_negative",
                bitmap_1_set) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "bitmap_2_create",
                    bitmap_create_2) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
