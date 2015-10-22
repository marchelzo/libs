#include <stdio.h>
#include <stdlib.h>

typedef struct s_it {
    char *(*next)(struct s_it *self);
    char *s;
    union {
        void *data;
        int (*pred)(char const *);
    };
    void *more_data;
} s_it;

char *_s_next(struct s_it *it);
#define s_next(it) _s_next(&it)

s_it s_split_on(char *s, char const *delim);
s_it s_split_every(char *s, size_t n);
s_it s_words(char *s);
s_it s_lines(char *s);
s_it s_matches(char *s, char const *);
s_it s_drop(size_t n, s_it it);
s_it s_take(size_t n, s_it it);
