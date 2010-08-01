#include "cwrap.ih"

extern "C" {

citar_tagger *citar_tagger_new(char const *lexicon, char const *ngrams)
{
  QFile ngramsFile(ngrams);
  if (!ngramsFile.open(QFile::ReadOnly))
    return 0;
  QTextStream ngramStream(&ngramsFile);

  QFile lexiconFile(lexicon);
  if (!lexiconFile.open(QFile::ReadOnly))
    return 0;
  QTextStream lexiconStream(&lexiconFile);

  citar_tagger *ctagger = new citar_tagger;

  try {
    QSharedPointer<Model> model(Model::readModel(lexiconStream, ngramStream));
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
  QVector<QString> sentence(2, "<START>");
  copy(words, words + len, back_inserter(sentence));
  sentence.push_back("<END>");

  QVector<QString> tagsV = tagger->tagger->tag(sentence);

  char **tags = static_cast<char **>(malloc(len * sizeof(char *)));
  for (int i = 0; i < len; ++i) {
    QByteArray tagsVData(tagsV[i + 2].toUtf8());
    tags[i] = strdup(tagsVData.constData());
  }

  return tags;
}

}
