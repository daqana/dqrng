#ifndef MYSTDINT_H
#define MYSTDINT_H 1

/* C99 defined macros like UINT64_MAX and UINT64_C but made them unavailable
 * to C+x unless __STDC_LIMIT_MACROS and __STDC_CONSTANT_MACROS are defined.
 * This was removed with C++11 and C11, but some old libc implementations still
 * have that (e.g. the one used in RHEL/CentOS/... 6). C++ compilers from that
 * time worked around this issue in the cstdint header, but newer compilers
 * don't do that any more. This file reintroduces these tricks to support old
 * libc versions together with newer compilers.
 *
 * Limitation: This header has no effect if cstdint is included before it.
 */

#ifndef __STDC_LIMIT_MACROS
# define _UNDEF__STDC_LIMIT_MACROS
# define __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
# define _UNDEF__STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif
#include <cstdint>
#ifdef _UNDEF__STDC_LIMIT_MACROS
# undef __STDC_LIMIT_MACROS
# undef _UNDEF__STDC_LIMIT_MACROS
#endif
#ifdef _UNDEF__STDC_CONSTANT_MACROS
# undef __STDC_CONSTANT_MACROS
# undef _UNDEF__STDC_CONSTANT_MACROS
#endif

// define our own UINT64_MAX in case it is still missing
#ifndef UINT64_MAX
#include <limits>
#define UINT64_MAX std::numeric_limits<std::uint64_t>::max()
#endif

#endif // MYSTDINT_H
