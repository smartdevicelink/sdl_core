#ifndef _UNISTD_H
#define _UNISTD_H    1
/* This file intended to serve as a drop-in replacement for
 *  unistd.h on Windows
 *  Please add functionality as neeeded 
 */

#include<stdlib.h>
#include<io.h>
#define srandom srand
#define random rand

const int W_OK =2;
const int R_OK =4;

#define access _access
#define ftruncate _chsize

#define ssize_t int

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
/* should be in some equivalent to <sys/types.h> */
#include <stdint.h>

#endif /* unistd.h */