/*
  MIT License

  Copyright (c) Kacper Dalach
  https://github.com/Dalachowsky/zephyr-tomlc99

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to permit
  persons to whom the Software is furnished to do so, subject to the
  following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
  NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
  USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "include/toml.h"
#include <errno.h>
#include <string.h>

/**
 * @brief Find string in toml table or return default value
 *
 * @param arr toml table
 * @param key key to retreive
 * @param out output buffer for retreived string
 * @param outsz size of out
 * @param default_value
 *
 * @retval 0 ok
 * @retval -ENOBUFS not enough space in out buffer
 * @retval -EINVAL could not parse
 */
int toml_string_in_or(const toml_table_t *arr,
                      const char *key,
                      char *out,
                      size_t outsz,
                      const char *default_value)
{
    const char *ret_string;
    int ret_string_len;
    toml_datum_t datum;

    if (!toml_key_exists(arr, key)) {
        ret_string = default_value;
    } else {
        datum = toml_string_in(arr, key);
        if (!datum.ok) {
            return -EINVAL;
        }
        ret_string = datum.u.s;
    }

    ret_string_len = strlen(ret_string);
    if (ret_string_len >= outsz) {
        return -ENOBUFS;
    }

    memcpy(out, ret_string, ret_string_len);
    out[ret_string_len] = '\0';

    return 0;
}

/**
 * @brief Find bool in toml table or return default value
 *
 * @param arr toml table
 * @param key key to retreive
 * @param out
 * @param default_value
 *
 * @retval 0 ok
 * @retval -EINVAL could not parse
 */
int toml_bool_in_or(const toml_table_t *arr,
                    const char *key,
                    bool *out,
                    bool default_value)
{
    toml_datum_t datum;

    if (!toml_key_exists(arr, key)) {
        *out = default_value;
    } else {
        datum = toml_bool_in(arr, key);
        if (!datum.ok) {
            return -EINVAL;
        }
        *out = datum.u.b;
    }
    return 0;
}

/**
 * @brief Find int in toml table or return default value
 *
 * @param arr toml table
 * @param key key to retreive
 * @param out
 * @param default_value
 *
 * @retval 0 ok
 * @retval -EINVAL could not parse
 */
int toml_int_in_or(const toml_table_t *arr,
                   const char *key,
                   int *out,
                   int default_value)
{
    toml_datum_t datum;

    if (!toml_key_exists(arr, key)) {
        *out = default_value;
    } else {
        datum = toml_int_in(arr, key);
        if (!datum.ok) {
            return -EINVAL;
        }
        *out = datum.u.i;
    }
    return 0;
}
