#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "s.h"

static bool prefix(char const *big, char const *small)
{
        while (*big && *small && *big == *small) {
                big += 1;
                small += 1;
        }

        return !*small;
}

static char *pattern_match(char const *pattern, char const *s)
{
        enum {
                TODO
        };
}

static char *next_split_on(s_it *self)
{
        char *s = self->s;

        while (*s && !prefix(s, self->data))
                s += 1;

        while (*s && prefix(s + 1, self->data))
                s += 1;

        bool last = !*s;
        *s = 0;

        char *result;

        if (s == self->s) {
                result = NULL;
        } else  {
                result = self->s;
                self->s = s + !last;
        }

        return result;
}

static char *next_split_every(s_it *self)
{
        char *s = self->s;

        int saved = (intptr_t) self->more_data;
        if (saved != -1)
                *s = saved;

        size_t n = (uintptr_t) self->data;
        while (*s && n --> 0)
                s += 1;

        self->more_data = (intptr_t) *s;
        *s = 0;

        char *result;

        if (s == self->s) {
                result = NULL;
        } else  {
                result = self->s;
                self->s = s;
        }

        return result;
}

static char *next_words(s_it *self)
{
        if (((uintptr_t) self->data) == 2)
                return NULL;

        char *s = self->s;

        if (((uintptr_t) self->data) == 1)
                s += 1;

        while (*s && isspace(*s))
                s += 1;

        char *word = s;

        while (*s && !isspace(*s))
                s += 1;

        if (word == s)
                return NULL;

        if (!*s)
                self->data = (uintptr_t) 2;
        else
                self->data = (uintptr_t) 1;

        *s = 0;

        self->s = s;

        return word;
}

char *_s_next(s_it *it)
{
        return it->next(it);
}

s_it s_split_on(char *s, char const *delim)
{
        s_it it;

        it.s = s;
        it.data = delim;
        it.next = next_split_on;

        return it;
}

s_it s_split_every(char *s, size_t n)
{
        s_it it;

        it.s = s;
        it.data = (uintptr_t) n;
        it.more_data = (intptr_t) -1;
        it.next = next_split_every;

        return it;
}

s_it s_words(char *s)
{
        s_it it;

        it.s = s;
        it.data = (uintptr_t) 0;
        it.next = next_words;

        return it;
}
