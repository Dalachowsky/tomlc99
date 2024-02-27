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

#include "include/toml.h"
#include <zephyr/init.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(toml, CONFIG_TOMLC99_LOG_LEVEL);

#define TOML_INIT_PRIO 0

K_MUTEX_DEFINE(toml_ctx_lock);

typedef struct {
  struct k_mutex *lock;
  struct k_heap heap;
} toml_ctx_t;

static toml_ctx_t ctx;

toml_table_t *toml_parse_string(char *conf) {
  toml_table_t *table;

  char errbuff[200];
  table = toml_parse(conf, errbuff, 200);
  if (table == 0) {
    LOG_ERR("Error during parsing: %s", errbuff);
  }

  return table;
}

/**
 * @brief   Initialize toml parser and take context lock
 *
 * @param heap pointer to heap buffer
 * @param heap_size size of heap
 * @param timeout amount of time to wait for context lock
 *
 * @warning Only one thread can have access to toml parser.
 *          Call toml_parser_free() after parsing is finished
 *
 * @retval 0 parser ready to use by this thread
 * @retval -EACCESS another thread uses toml context
 */
int toml_parser_get(uint8_t *heap, size_t heap_size, k_timeout_t timeout) {
  if (k_mutex_lock(ctx.lock, timeout) != 0) {
    return -EACCES;
  }

  k_heap_init(&ctx.heap, heap, heap_size);

  return 0;
}

/**
 * @brief Unlock context lock
 * @details @see toml_parser_get()
 */
int toml_parser_free() {
  k_mutex_unlock(ctx.lock);
  return 0;
}

/**
 * Wrapper for malloc()
 */
static void *_toml_malloc(size_t size) {
  return k_heap_alloc(&ctx.heap, size, K_NO_WAIT);
}

/**
 * Wrapper for free()
 */
static void _toml_free(void *ptr) { k_heap_free(&ctx.heap, ptr); }

int toml_sys_init(void) {
  ctx.lock = &toml_ctx_lock;
  toml_set_memutil(_toml_malloc, _toml_free);
  return 0;
}

SYS_INIT(toml_sys_init, APPLICATION, TOML_INIT_PRIO);

/* end of group toml */
/** @} */