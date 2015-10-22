#include <stdbool.h>

struct re_nfa;
typedef struct re_nfa re_pat;

typedef struct {
        char const *start;
        char const *end;
} re_result;

re_pat *  re_compile (char const *);
bool      re_match   (re_pat const *, char const *, re_result *);
void      re_free    (re_pat *);

