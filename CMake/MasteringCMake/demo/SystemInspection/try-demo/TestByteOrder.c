// ---- TestByteOrder.c ------
#include <stdlib.h>

int main () {
  /* Are we most significant byte first or last */
  union
  {
    long l;
    char c[sizeof (long)];
  } u;
  u.l = 1;
  exit (u.c[sizeof (long) - 1] == 1);
}
