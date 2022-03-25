#ifndef __SSTK_PORTABLE_HEADERS_
#define __SSTK_PORTABLE_HEADERS_

#include "sstk_portable_define.h"
#include "sstk_portable_config.h"

#if !defined(SSTK_ARCH)
#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#define SSTK_ARCH SSTK_ARCH_UNIX
#elif defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
#define SSTK_ARCH SSTK_ARCH_WIN32
#else
#error "unknown arch"
#endif
#endif

#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#if SSTK_ARCH == SSTK_ARCH_UNIX

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#elif SSTK_ARCH == SSTK_ARCH_WIN32

#include <malloc.h>

#elif SSTK_ARCH == SSTK_ARCH_TI_OS

// posix standard api
#include <unistd.h>

#endif


#if (SSTK_THREADS)
#include <pthread.h>
#endif

#endif
