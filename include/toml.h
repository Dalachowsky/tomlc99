/*
  MIT License

  Copyright (c) Kacper Dalach
  https://github.com/Dalachowsky/zephyr-tomlc99

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef __TOML_H_
#define __TOML_H_

#include "../toml.h"
#include <zephyr/kernel.h>

#define toml_double_in(x, y) error - parsing doubles not supported
#define toml_double_at(x, y) error - parsing doubles not supported

int toml_parser_get(uint8_t *heap, size_t heap_size, k_timeout_t timeout);
int toml_parser_free(void);

toml_table_t *toml_parse_string(char *conf);

#endif /* __TOML_H_ */

/* end of group toml */
/** @} */