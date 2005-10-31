/* -*- linux-c -*- 
 * Memory allocation functions
 * Copyright (C) 2005 Red Hat Inc.
 *
 * This file is part of systemtap, and is free software.  You can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License (GPL); either version 2, or (at your option) any
 * later version.
 */

#ifndef _ALLOC_C_
#define _ALLOC_C_

/** @file alloc.c
 * @brief Memory functions.
 */
/** @addtogroup alloc Memory Functions
 * Basic malloc/calloc/free functions. These will be changed so 
 * that memory allocation errors will call a handler.  The default will
 * send a signal to the user-space daemon that will trigger the module to
 * be unloaded.
 * @{
 */

/** Allocates memory within a probe.
 * This is used for small allocations from within a running
 * probe where the process cannot sleep. 
 * @param len Number of bytes to allocate.
 * @return a valid pointer on success or NULL on failure.
 * @note Not currently used by the runtime. Deprecate?
 */

void *_stp_alloc(size_t len)
{
	void *ptr = kmalloc(len, GFP_ATOMIC);
	if (unlikely(ptr == NULL))
		_stp_error("_stp_alloc failed.\n");
	return ptr;
}

/** Allocates and clears memory within a probe.
 * This is used for small allocations from within a running
 * probe where the process cannot sleep. 
 * @param len Number of bytes to allocate.
 * @return a valid pointer on success or NULL on failure.
 * @note Not currently used by the runtime. Deprecate?
 */

void *_stp_calloc(size_t len)
{
	void *ptr = _stp_alloc(len);
	if (likely(ptr))
		memset(ptr, 0, len);
	return ptr;
}

/** Allocates and clears memory outside a probe.
 * This is typically used in the module initialization to
 * allocate new maps, lists, etc.
 * @param len Number of bytes to allocate.
 * @return a valid pointer on success or NULL on failure.
 */

void *_stp_valloc(size_t len)
{
	void *ptr = vmalloc(len);
	if (likely(ptr))
		memset(ptr, 0, len);
	else
		_stp_error("_stp_valloc failed.\n");
	return ptr;
}

/** Frees memory allocated by _stp_alloc or _stp_calloc.
 * @param ptr pointer to memory to free
 * @note Not currently used by the runtime. Deprecate?
 */

void _stp_free(void *ptr)
{
	if (likely(ptr))
		kfree(ptr);
}

/** Frees memory allocated by _stp_valloc.
 * @param ptr pointer to memory to free
 */

void _stp_vfree(void *ptr)
{
	if (likely(ptr))
		vfree(ptr);
}

/** @} */
#endif /* _ALLOC_C_ */
