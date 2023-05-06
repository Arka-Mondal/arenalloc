/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef ARENALLOC_H
#define ARENALLOC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct arena {
  uint8_t * buf;
  size_t capacity;
  size_t curr_offset;
  bool is_init;
};

typedef struct arena arena_t;

bool arena_init(arena_t * const restrict, uint8_t * const, size_t);
void * arena_alloc(arena_t * const restrict, size_t);
void * arena_alloc_align(arena_t * const restrict, size_t, size_t);
bool arena_deinit(arena_t * const restrict);

#endif
