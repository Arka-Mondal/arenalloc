/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include <stdlib.h>
#include "arenalloc.h"
#define DEFAULT_ALIGN 16

static bool is_power_of_two(size_t align)
{
  return (align & (align - 1)) == 0;
}

static uintptr_t arena_align_forward(uintptr_t ptr, size_t align)
{
  uintptr_t modulo_align;

  if (!is_power_of_two(align))
  {
    fputs("Error: arena_align_forward: align should be power of two.\n", stderr);
    exit(EXIT_FAILURE);
  }

  modulo_align = ptr & ((uintptr_t) align - 1);

  if (modulo_align != 0)
    ptr += (uintptr_t) align - modulo_align;

  return ptr;
}

bool arena_init(arena_t * const restrict arena_ptr, uint8_t * const alloc_buf,
                size_t buf_cap)
{
  if ((arena_ptr == NULL) || (arena_ptr->is_init)
      || (alloc_buf == NULL) || (buf_cap == 0))
    return false;

  arena_ptr->buf = alloc_buf;
  arena_ptr->capacity = buf_cap;
  arena_ptr->curr_offset = 0;
  arena_ptr->is_init = true;

  return true;
}

void * arena_alloc(arena_t * const restrict arena_ptr, size_t size)
{
  return arena_alloc_align(arena_ptr, size, DEFAULT_ALIGN);
}

void * arena_alloc_align(arena_t * const restrict arena_ptr,
                         size_t size, size_t alignment)
{
  uint8_t * curr_ptr;
  uintptr_t offset;

  if ((arena_ptr == NULL) || (!arena_ptr->is_init)
      || (size == 0) || (alignment == 0))
    return NULL;

  curr_ptr = arena_ptr->buf + arena_ptr->curr_offset;
  offset = arena_align_forward((uintptr_t) curr_ptr, alignment);
  offset -= (uintptr_t) arena_ptr->buf;

  if ((size_t) offset + size > arena_ptr->capacity)
    return NULL;

  arena_ptr->curr_offset = (size_t) offset + size;

  return arena_ptr->buf + offset;
}

bool arena_deinit(arena_t * const restrict arena_ptr)
{
  if ((arena_ptr == NULL) || (!arena_ptr->is_init))
    return false;

  arena_ptr->buf = NULL;
  arena_ptr->capacity = 0;
  arena_ptr->curr_offset = 0;
  arena_ptr->is_init = false;

  return true;
}
