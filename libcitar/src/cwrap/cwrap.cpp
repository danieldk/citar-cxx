#include "cwrap.ih"

extern "C" {

citar_tagger *citar_tagger_new(char const *lexicon, char const *ngrams)
{
  ifstream nGramStream(ngrams);
  if (!nGramStream.good())
    return 0;

  ifstream lexiconStream(lexicon);
  if (!lexiconStream.good())
    return 0;

  citar_tagger *ctagger = new citar_tagger;

  try {
    shared_ptr<Model> model(Model::readModel(lexiconStream, nGramStream));
    ctagger->unknownWordHandler = new SuffixWordHandler(model, 2, 2, 8);
    ctagger->knownWordHandler =
      new KnownWordHandler(model, ctagger->unknownWordHandler);
    ctagger->smoother = new LinearInterpolationSmoothing(model);
    ctagger->tagger =
      new HMMTagger(model, ctagger->knownWordHandler, ctagger->smoother);
  } catch (...) {
    return 0;
  }

  return ctagger;
}

void citar_tagger_free(citar_tagger *tagger)
{
  delete tagger->tagger;
  delete tagger->smoother;
  delete tagger->knownWordHandler;
  delete tagger->unknownWordHandler;
  delete tagger;
}

char **citar_tagger_tag(citar_tagger *tagger, char const *words[], int len)
{
  vector<string> sentence(2, "<START>");
  copy(words, words + len, back_inserter(sentence));
  sentence.push_back("<END>");

  vector<string> tagsV = tagger->tagger->tag(sentence);

  char **tags = static_cast<char **>(malloc(len * sizeof(char *)));
  for (int i = 0; i < len; ++i)
    tags[i] = strdup(tagsV[i + 2].c_str());

  return tags;
}

}
