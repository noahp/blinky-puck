#include <stdio.h>

#include <ctype.h>
#include <getopt.h>

extern void initialise_monitor_handles(void);


// void _sbrk(void) {}
// void _write(void) {}
// void _close(void) {}
// void _fstat(void) {}
// void _isatty(void) {}
// void _lseek(void) {}
// void _read(void) {}


int main(void) {
#if ENABLE_SEMIHOSTING
  initialise_monitor_handles();

  // don't buffer on stdout
  setvbuf(stdout, NULL, _IOLBF, 0);
  printf("🦄 Hello there!\n");
#endif
  while (1) {
  };

  return 0;
}
