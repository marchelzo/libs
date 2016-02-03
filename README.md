## libs

libs is a tiny C library which aims to make working with strings
a little more convenient without requiring any heavy machinery or dependencies.

See the example usage below to get an idea of what it does.

The only function which allocates memory is `s_matches` (to compile the regular expression).

It is important to note that these functions mutate the string that is passed to them.

This means that things like `s_words("foo bar baz")` will result in undefined behaviour.

### Examples

##### Regular expression matches

```c
#include <s.h>
#include <stdio.h>

int main(void)
{
        char string[] = "a: 12, b: 23, c: 19, k: 123, d: 123";

        s_it matches = s_take(3, s_matches(string, "[0-9]+"));

        char *match;
        while (match = s_next(matches)) {
                printf("Match: %s\n", match);
        }

        return 0;
}
```

Result:
```
$ cc -std=c99 -o example example.c -ls
$ ./example
Match: 12
Match: 23
Match: 19
$ 
```

##### Filtering with a predicate

```c
#include <s.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static bool
contains_o(char const *s)
{
    return strchr(s, 'o') != NULL;
}

int main(void)
{
        char string[] = "i only want words containing the letter 'o'";

        s_it matches = s_filter(contains_o, s_words(string));

        char *match;
        while (match = s_next(matches)) {
                printf("Match: %s\n", match);
        }

        return 0;
}
```

Result:
```
$ cc -std=c99 -o example example.c -ls
$ ./example
Match: only
Match: words
Match: containing
Match: 'o'
$ 
```
