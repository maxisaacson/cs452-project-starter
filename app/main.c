#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  printf("hello world\n");
  int c;
  while ((c = getopt(argc, argv, "abc:")) != -1)
    switch(c)
    {
      case 'a':
        printf("got A");
        break;
      case 'b':
        printf("got B");
        break;
      case 'c':
        printf("got C");
        break;
      case '?':
        if (isprint(optopt))
          fprintf(stderr, "Unknown option", optopt);
        else
          fprintf(stderr, "Unknown option", optopt);
      return 1;
      default:
        abort();
        break;
    }
  return 0;
}
