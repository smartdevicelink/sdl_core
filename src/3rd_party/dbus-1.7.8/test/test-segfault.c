/* This is simply a process that segfaults */
#include <config.h>
#include <stdlib.h>
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_SETRLIMIT
#include <sys/resource.h>
#endif

int
main (int argc, char **argv)
{
  char *p;  

#if HAVE_SETRLIMIT
  struct rlimit r = { 0, };
  
  getrlimit (RLIMIT_CORE, &r);
  r.rlim_cur = 0;
  setrlimit (RLIMIT_CORE, &r);
  
  raise (SIGSEGV);
#endif
  p = NULL;
  *p = 'a';
  
  return 0;
}
