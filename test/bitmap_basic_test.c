/**
 * API testing of BITMAP
 */
#include <bitmap.h>
#include <CUnit/Basic.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

bitmap_handle handle1;
bitmap_handle handle2;
bitmap_handle handle3;
uint16_t wsize = 0;

void bitmap_basic_1_create ()
{
    bitmap_handle handle1_temp;
    bitmap_retval_t retval;
    bitmap_attr_t attr;
    uint32_t bits_per_block, bytes_per_block, words_per_block;
    uint16_t count, wsize;

    bits_per_block = bytes_per_block = words_per_block = 0;
    retval = bitmap_get_wordsize(&wsize);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    printf("\n Bitmap word size = %d", wsize);

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


    memset(&attr, 0, sizeof(bitmap_attr_t));
    // Set the proper values.
    strcpy(attr.short_name, "handle-3");
    attr.bits_per_block = wsize*8*2;
    retval = bitmap_create(&handle3, &attr);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);


    retval = bitmap_get_block_details(handle1, 
                                      &bits_per_block, 
                                      NULL, NULL);

    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(bits_per_block == (BITMAP_SIZE_1K_BITS *2));

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
    CU_ASSERT_TRUE(bits_per_block == (BITMAP_SIZE_1K_BITS * 2));
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
    CU_ASSERT_TRUE(bits_per_block == BITMAP_SIZE_1K_BITS);
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
    // one of the parameters should be valid.
    retval = bitmap_get_block_details(handle1,
                                      NULL,
                                      NULL,
                                      NULL);
    CU_ASSERT_TRUE(retval != BITMAP_RETVAL_SUCCESS);

    strcpy(attr.short_name, "h1_temp");
    // Bits per block should be a multiple of 8 and the word size.
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
    bitmap_retval_t retval;
    uint16_t count;
    retval = bitmap_set(handle3, 15);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_get_block_count(handle3,
                                    &count);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(count == 1);

    //Create 3 new blocks
    retval = bitmap_set(handle3, 52);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_get_block_count(handle3,
                                    &count);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(count == 4);

    // Creata a new bitblock
    retval = bitmap_set(handle3, 22);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_get_block_count(handle3,
                                    &count);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(count == 4);

    retval = bitmap_set(handle1, 20);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_get_block_count(handle1,
                                    &count);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(count == 1);

    retval = bitmap_set(handle1, 3000);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_get_block_count(handle1,
                                    &count);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(count == 2);

    retval = bitmap_set(handle2, 1024);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_set(handle2, 1025);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_get_block_count(handle2,
                                    &count);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(count == 2);

}
void bitmap_basic_1_check ()
{
    bitmap_retval_t retval;
    retval = bitmap_check(BITMAP_HANDLE_NULL, 20);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_INVALID_INPUT);
    // bit set in block1
    retval = bitmap_check(handle1, 20);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);
    // bit set in block2
    retval = bitmap_check(handle1, 3000);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);
    // Bit not set
    retval = bitmap_check(handle1, 1024);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);
    // Bit position not available.
    retval = bitmap_check(handle1, 5000);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);

    retval = bitmap_check(handle2, 1024);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);

    retval = bitmap_check(handle3, 15);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);
    retval = bitmap_check(handle3, 22);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);
    retval = bitmap_check(handle3, 52);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);
    retval = bitmap_check(handle3, 20);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);
    retval = bitmap_check(handle3, -20);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);
}

void bitmap_basic_1_clear() 
{
    bitmap_retval_t retval;

    retval = bitmap_clear(BITMAP_HANDLE_NULL, 20);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_INVALID_INPUT);

    retval = bitmap_check(handle3, 15);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);

    retval = bitmap_clear(handle3, 15);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);

    retval = bitmap_check(handle3, 15);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);

    retval = bitmap_clear(handle3, 15);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    
    retval = bitmap_check(handle3, 53);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);

    retval = bitmap_clear(handle3, 53);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);

    retval = bitmap_clear(handle1, 5000);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_INVALID_INPUT);

    retval = bitmap_clear(handle2, 1024);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_clear(handle2, 1025);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
}

void bitmap_basic_1_clear_all() 
{
    bitmap_retval_t retval;

    retval = bitmap_clear_all(BITMAP_HANDLE_NULL);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_INVALID_INPUT);

    // bit set in block1
    retval = bitmap_check(handle1, 20);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);
    // bit set in block2
    retval = bitmap_check(handle1, 3000);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCHED);
    // Bit not set
    retval = bitmap_check(handle1, 1024);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);
    // Bit position not available.

    retval = bitmap_clear_all(handle1);

    retval = bitmap_check(handle1, 20);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);
    // bit set in block2
    retval = bitmap_check(handle1, 3000);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);
    // Bit not set
    retval = bitmap_check(handle1, 1024);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_MATCH_FAIL);
}

void bitmap_basic_1_destroy() 
{
    bitmap_retval_t retval;

    retval = bitmap_destroy(BITMAP_HANDLE_NULL);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_INVALID_INPUT);

    retval = bitmap_destroy(&handle1);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_SUCCESS);
    retval = bitmap_check(handle1, 3000);
    CU_ASSERT_TRUE(retval == BITMAP_RETVAL_INVALID_INPUT);
}

int main ()
{
    int file_ext;
    char file_name[32];
    FILE *fptr = NULL;

    srand(time(0));
    file_ext = rand();
    sprintf(file_name, "/tmp/bitmap_%d.log",file_ext % 1000);
    fptr = fopen(file_name, "w");
    printf("\n writing logs to file %s \n",file_name);

    bitmap_set_logger_fd(fileno(fptr));

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
    fclose(fptr);
    return CU_get_error();
}
