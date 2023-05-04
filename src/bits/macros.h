#ifndef _MACROS_H
#define _MACROS_H

// Return 1 if x is positive, -1 if x is negative or 0 if x is zero
#define SIGNUM(x) (x > 0) - (x < 0)
// Returns the difference x - y
#define DIFF(x , y) (x - y)

#define unlikely(expr) __builtin_expect(!!(expr), 0)
#define likely(expr)   __builtin_expect(!!(expr), 1)

#endif