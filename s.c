#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "s.h"
#include "re.h"

static bool prefix(char const *big, char const *small)
{
        while (*big && *small && *big == *small) {
                big += 1;
                small += 1;
        }

        return !*small;
}

static char *next_nothing(s_it *self)
{
        return NULL;
}

static char *next_count(s_it *self)
{
        size_t count = (uintptr_t) self->data;
        if (count == 0) {
                return NULL;
        }

        char *ret = self->s;

        if (--count != 0) {
                self->s += strlen(self->s);
                while (*++self->s == 0x1F);
        }

        self->data = (uintptr_t) count;

        return ret;
}

static char *next_matches(s_it *self)
{
        re_result m;

        if (self->s == NULL) {
                re_free(self->data);
                return NULL;
        }

        *self->s = (uintptr_t) self->more_data;

        if (!re_match(self->data, self->s, &m)) {
                re_free(self->data);
                return NULL;
        }

        if (*m.end == '\0')  {
                self->s = NULL;
        } else {
                self->more_data = (uintptr_t) *m.end;
                ((char *) m.end)[0] = '\0';
                self->s = m.end;
        }

        return m.start;
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

s_it s_lines(char *s)
{
        return s_split_on(s, "\n");
}

s_it s_matches(char *s, char const *pat)
{
        s_it it;

        it.s = s;
        it.data = re_compile(pat);
        it.more_data = (uintptr_t) *s;
        it.next = next_matches;
        
        return it;
}

s_it s_drop(size_t n, s_it it)
{
        size_t i;

        for (i = 0; i < n; ++i) {
                if (s_next(it) == NULL) {
                        it.next = next_nothing;
                        break;
                }
        }

        return it;
}

s_it s_take(size_t n, s_it it)
{
        size_t i;
        char *save = it.s;
        char *prev;
        char *current;

        if ((save = s_next(it)) == NULL) {
                it.next = next_nothing;
                return it;
        }

        prev = save;
        for (i = 1; i < n; ++i) {
                prev = prev + strlen(prev);
                current = s_next(it);
                if (current == NULL) {
                        break;
                }
                *prev = '\0';
                while (prev + 1 < current) {
                        *++prev = 0x1F;
                }
                prev = current;
        }

        it.s = save;
        it.next = next_count;
        it.data = (uintptr_t) i;

        return it;
}
