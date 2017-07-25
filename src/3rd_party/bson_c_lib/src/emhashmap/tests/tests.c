#include <check.h>
#include <stdint.h>
#include "emhashmap.h"
#include <stdio.h>
#include <stdlib.h>

HashMap map;
int capacity = 128;
float load_factor = 10;
int key = 1;
void* value = (void*) 1;

void setup() {
    emhashmap_initialize(&map, capacity, load_factor);
}

void teardown() {
    emhashmap_deinitialize(&map);
}

START_TEST (test_init)
{
    emhashmap_initialize(&map, capacity, .5);
    emhashmap_deinitialize(&map);
}
END_TEST

START_TEST (test_fill_up)
{
    int i;
    for(i = 0; i < capacity; i++) {
        ck_assert(emhashmap_put(&map, i, (void*)i));
        ck_assert_int_eq(emhashmap_size(&map), i + 1);
    }
    ck_assert(!emhashmap_put(&map, i, (void*)i));
}
END_TEST

START_TEST (test_put_multiple)
{
    ck_assert(emhashmap_put(&map, key, value));
    ck_assert(emhashmap_put(&map, 2, (void*)2));
    ck_assert_int_eq(emhashmap_size(&map), 2);
}
END_TEST

START_TEST (test_get)
{
    emhashmap_put(&map, key, value);
    MapEntry* entered_value = emhashmap_get(&map, key);
    ck_assert(entered_value != NULL);
    ck_assert(entered_value->value == value);
}
END_TEST

START_TEST (test_get_missing)
{
    ck_assert(emhashmap_get(&map, key) == NULL);
}
END_TEST

START_TEST (test_put)
{
    ck_assert(emhashmap_put(&map, key, value));
    ck_assert_int_eq(emhashmap_size(&map), 1);
}
END_TEST

START_TEST (test_overwrite)
{
    ck_assert(emhashmap_put(&map, key, value));
    ck_assert_int_eq(emhashmap_size(&map), 1);
    void* another_value = (void*)2;
    ck_assert(emhashmap_put(&map, key, another_value));
    ck_assert_int_eq(emhashmap_size(&map), 1);
    ck_assert(emhashmap_get(&map, key)->value == another_value);
}
END_TEST

START_TEST (test_contains)
{
    emhashmap_put(&map, key, value);
    ck_assert(emhashmap_contains(&map, key));
}
END_TEST

START_TEST (test_contains_null_value)
{
    emhashmap_put(&map, key, NULL);
    ck_assert(emhashmap_contains(&map, key));
}
END_TEST

START_TEST (test_does_not_contain)
{
    emhashmap_put(&map, key, value);
    ck_assert(!emhashmap_contains(&map, 2));
}
END_TEST

START_TEST (test_remove)
{
    emhashmap_put(&map, key, value);
    ck_assert(emhashmap_contains(&map, key));
    ck_assert(emhashmap_remove(&map, key) == value);
    ck_assert(!emhashmap_contains(&map, key));
}
END_TEST

START_TEST (test_remove_not_in_map)
{
    emhashmap_put(&map, key, value);
    ck_assert(emhashmap_contains(&map, key));
    ck_assert(!emhashmap_remove(&map, 2));
    ck_assert(emhashmap_contains(&map, key));
}
END_TEST

START_TEST (test_is_empty)
{
    ck_assert(emhashmap_is_empty(&map));
    emhashmap_put(&map, key, value);
    ck_assert(!emhashmap_is_empty(&map));
}
END_TEST

START_TEST (test_size)
{
    ck_assert_int_eq(emhashmap_size(&map), 0);
    emhashmap_put(&map, key, value);
    ck_assert_int_eq(emhashmap_size(&map), 1);
    emhashmap_put(&map, 2, value);
    ck_assert_int_eq(emhashmap_size(&map), 2);
}
END_TEST

START_TEST (test_create)
{
    ck_assert(&map != NULL);
}
END_TEST

START_TEST (test_iterate_empty)
{
    MapIterator iterator = emhashmap_iterator(&map);
    ck_assert(emhashmap_iterator_next(&iterator) == NULL);
}
END_TEST

START_TEST (test_iterate_one)
{
    emhashmap_put(&map, key, value);
    MapIterator iterator = emhashmap_iterator(&map);
    MapEntry* entry = emhashmap_iterator_next(&iterator);
    ck_assert(entry != NULL);
    ck_assert(entry->key == key);
    ck_assert(entry->value == value);
}
END_TEST

START_TEST (test_iterate_many)
{
    emhashmap_put(&map, key, value);
    emhashmap_put(&map, 2, value);
    MapIterator iterator = emhashmap_iterator(&map);

    MapEntry* entry = emhashmap_iterator_next(&iterator);
    ck_assert(entry != NULL);
    ck_assert(entry->key == key);
    ck_assert(entry->value == value);

    entry = emhashmap_iterator_next(&iterator);
    ck_assert(entry != NULL);
    ck_assert(entry->key == 2);
    ck_assert(entry->value == value);

    entry = emhashmap_iterator_next(&iterator);
    ck_assert(entry == NULL);
}
END_TEST

Suite* suite(void) {
    Suite* s = suite_create("queue");
    TCase *tc_core = tcase_create("core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_get);
    tcase_add_test(tc_core, test_get_missing);
    tcase_add_test(tc_core, test_put);
    tcase_add_test(tc_core, test_put_multiple);
    tcase_add_test(tc_core, test_fill_up);
    tcase_add_test(tc_core, test_overwrite);
    tcase_add_test(tc_core, test_contains);
    tcase_add_test(tc_core, test_does_not_contain);
    tcase_add_test(tc_core, test_remove);
    tcase_add_test(tc_core, test_remove_not_in_map);
    tcase_add_test(tc_core, test_is_empty);
    tcase_add_test(tc_core, test_size);
    tcase_add_test(tc_core, test_create);
    tcase_add_test(tc_core, test_iterate_empty);
    tcase_add_test(tc_core, test_iterate_one);
    tcase_add_test(tc_core, test_iterate_many);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int numberFailed;
    Suite* s = suite();
    SRunner *sr = srunner_create(s);
    // Don't fork so we can actually use gdb
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    numberFailed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (numberFailed == 0) ? 0 : 1;
}
