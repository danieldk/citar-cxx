#ifndef CITAR_CWRAP_HH
#define CITAR_CWRAP_HH

#ifdef __cplusplus
extern "C" {
#endif

typedef struct citar_tagger_t *citar_tagger;

citar_tagger citar_tagger_new(char const *lexicon, char const *trigrams);
void citar_tagger_free(citar_tagger tagger);
char **citar_tagger_tag(citar_tagger tagger, char const *words[], int len);

#ifdef __cplusplus
}
#endif

#endif
