#include <stdio.h>
#include <ctype.h>
#include "s.h"

static bool
all_lowercase(char const *s)
{
    while (*s)
        if (!islower(*s++))
            return false;

    return true;
}

static bool
all_digits(char const *s)
{
    while (*s)
        if (!isdigit(*s++))
            return false;

    return true;
}

int
main(void)
{
        char s[]  = "\nMy test string has\nmany\t\twords in   \t\n it... ";
        char s2[] = "abcdABCDefghEFGHijklIJKLmnopMNO";
        char s3[] = "Three numbers are 123 and 456 and finally 789";
        char s4[] = "A_HHH B_343 asd asd a_123 okd 1_000\n";
        char s5[] = "The first three words will be skipped!";
        char s6[] = "Only three words will be used!";
        char s7[] = "only lowercase words Will Be Taken";
        char s8[] = "only 123 the 5 numbers in 56134 this 9213 string will be 0001 taken";

        s_it it = s_words(s);

        char *w;
        printf("===> s_words <===\n");
        while (w = s_next(it))
                printf("Word: |%s|\n", w);

        printf("===> s_split_every 4 <===\n");
        it = s_split_every(s2, 4);
        for (unsigned i = 1; w = s_next(it); ++i) {
                if (i & 1)
                        printf("Lowercase: |%s|\n", w);
                else
                        printf("Uppercase: |%s|\n", w);
        }

        printf("===> s_matches /[0-9]+/ <===\n");
        it = s_matches(s3, "[0-9]+");
        while (w = s_next(it)) {
                printf("Number: |%s|\n", w);
        }

        printf("===> s_matches /._.../ <===\n");
        it = s_matches(s4, "._...");
        while (w = s_next(it)) {
                printf("Match: |%s|\n", w);
        }

        printf("===> s_drop <===\n");
        it = s_drop(3, s_words(s5));
        while (w = s_next(it)) {
                printf("Word: |%s|\n", w);
        }

        printf("===> s_take <===\n");
        it = s_take(3, s_words(s6));
        while (w = s_next(it)) {
                printf("Word: |%s|\n", w);
        }

        printf("===> s_take_while <===\n");
        it = s_take_while(all_lowercase, s_words(s7));
        while (w = s_next(it)) {
                printf("Word: |%s|\n", w);
        }

        printf("===> s_filter <===\n");
        it = s_filter(all_digits, s_words(s8));
        while (w = s_next(it)) {
                printf("Number: |%s|\n", w);
        }
                        
        return 0;
}
