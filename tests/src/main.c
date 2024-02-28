
#include <zephyr/ztest.h>
#include <toml.h>
#include <string.h>

ZTEST_SUITE(lib_toml_test, NULL, NULL, NULL, NULL, NULL);

ZTEST(lib_toml_test, test_toml_basic)
{
    int ret;
    char *config = "foo = \"bar\"\n"
                   "number = 1\n"
                   "boolean = true\n";
    uint8_t heap[1024];
    ret = toml_parser_get((uint8_t *)&heap, sizeof(heap), K_NO_WAIT);
    zassert_equal(ret, 0, "Could not get parser");
    toml_table_t *table = toml_parse_string(config);
    toml_datum_t foo = toml_string_in(table, "foo");
    toml_datum_t number = toml_int_in(table, "number");
    toml_datum_t boolean = toml_bool_in(table, "boolean");

    ret = strcmp(foo.u.s, "bar");
    zassert_equal(ret, 0, "Parsing string failed");
    zassert_equal(number.u.i, 1, "Parsing int failed");
    zassert_equal(boolean.u.b, true, "Parsing bool failed");

    toml_parser_free();
}

ZTEST_SUITE(lib_toml_helpers_test, NULL, NULL, NULL, NULL, NULL);

ZTEST(lib_toml_helpers_test, test_string_or)
{
    int ret;
    char *config = "foo = \"foo\"\n";
    uint8_t heap[1024];
    ret = toml_parser_get((uint8_t *)&heap, sizeof(heap), K_NO_WAIT);
    zassert_equal(ret, 0, "Could not get parser");
    toml_table_t *table = toml_parse_string(config);

    char ret_buf[4];

    /* Sanity check */
    toml_string_in_or(table, "foo", ret_buf, sizeof(ret_buf), "");
    ret = strcmp(ret_buf, "foo");
    zassert_equal(ret, 0, "Foo not found");

    /* Check if returns default value */

    toml_string_in_or(table, "bar", ret_buf, sizeof(ret_buf), "bar");
    ret = strcmp(ret_buf, "bar");
    zassert_equal(ret, 0, "Parsing string failed");

    toml_parser_free();
}

ZTEST(lib_toml_helpers_test, test_bool_or)
{
    int ret;
    char *config = "foo = true\n";
    uint8_t heap[1024];
    ret = toml_parser_get((uint8_t *)&heap, sizeof(heap), K_NO_WAIT);
    zassert_equal(ret, 0, "Could not get parser");
    toml_table_t *table = toml_parse_string(config);

    bool ret_val = false;

    /* Sanity check */
    toml_bool_in_or(table, "foo", &ret_val, false);
    zassert_equal(ret_val, true, "Foo not parsed");

    /* Check if returns default value */
    ret_val = false;
    toml_bool_in_or(table, "bar", &ret_val, true);
    zassert_equal(ret_val, true, "Default value not received");

    toml_parser_free();
}

ZTEST(lib_toml_helpers_test, test_int_or)
{
    int ret;
    char *config = "foo = 1\n";
    uint8_t heap[1024];
    ret = toml_parser_get((uint8_t *)&heap, sizeof(heap), K_NO_WAIT);
    zassert_equal(ret, 0, "Could not get parser");
    toml_table_t *table = toml_parse_string(config);

    int ret_val = 0;

    /* Sanity check */
    toml_int_in_or(table, "foo", &ret_val, -1);
    zassert_equal(ret_val, 1, "Foo not parsed");

    /* Check if returns default value */
    ret_val = 0;
    toml_int_in_or(table, "bar", &ret_val, 1);
    zassert_equal(ret_val, 1, "Default value not received");

    toml_parser_free();
}
