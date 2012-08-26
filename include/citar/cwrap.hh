#ifndef CITAR_CWRAP_HH
#define CITAR_CWRAP_HH

#include <citar/tagger/hmm/HMMTagger.hh>
#include <citar/tagger/hmm/Smoothing.hh>
#include <citar/tagger/hmm/Model.hh>
#include <citar/tagger/wordhandler/WordHandler.hh>

extern "C"
{

struct citar_tagger {
  citar::tagger::WordHandler *knownWordHandler;
  citar::tagger::WordHandler *unknownWordHandler;
  citar::tagger::Smoothing *smoother;
  citar::tagger::HMMTagger *tagger;
};

citar_tagger *citar_tagger_new(char const *lexicon, char const *trigrams);
void citar_tagger_free(citar_tagger *tagger);
char **citar_tagger_tag(citar_tagger *tagger, char const *words[], int len);
}

#endif
