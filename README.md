### libs

libs is a small C library which aims to make working with strings
slightly more convenient without introducing any heavy machinery.


##### Short example usage

```c
#include <s.h>
#include <stdio.h>

int main(void)
{
        char string[] = "a: 12, b: 23: c: 19, k: 123, d: 123";

        s_it matches = s_take(3, s_matches(string, "[0-9]+"));

        char *match;
        while (match = s_next(matches)) {
                printf("Match: %s\n", match);
        }

        return 0;
}
```

Here's what this will do:
```
$ cc -std=c99 -o example example.c -ls
$ ./example
Match: 12
Match: 23
Match: 19
$ 
```
