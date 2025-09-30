#ifndef platform_types_h_
#define platform_types_h_

#include <stdint.h>

#ifndef NULL
#ifndef __cplusplus
#define NULL (void *)0
#else
#define NULL 0
#endif
#endif

#ifndef __cplusplus
typedef unsigned char boolean;
#else
typedef bool boolean;
#endif

/** @brief Macro for boolean TRUE. */
#define TRUE 1

/** @brief Macro for boolean FALSE. */
#define FALSE 0

#endif // platform_types_h_
