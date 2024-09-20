#pragma once

#include <stdarg.h>
#include <stddef.h>

#include "libhtmc/libhtmc.h"

int   impl_debug_vprintf(htmc_handover_t *handover,
                         const char      *fmt,
                         va_list          args);
int   impl_debug_query_vscanf(htmc_handover_t *handover,
                              const char      *fmt,
                              va_list          args);
int   impl_debug_form_vscanf(htmc_handover_t *handover,
                             const char      *fmt,
                             va_list          args);
void *impl_debug_alloc(htmc_handover_t *handover, size_t nbytes);
void  impl_debug_free(htmc_handover_t *handover, void *ptr);
void  impl_debug_cleanup(htmc_handover_t *handover);
