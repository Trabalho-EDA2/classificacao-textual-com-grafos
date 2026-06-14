import pandas as pd
import spacy
from tqdm.auto import tqdm


tqdm.pandas()

nlp = spacy.load("en_core_web_sm")

df = pd.read_csv("data/IMDB_dataset.csv")

def preprocess(text):
    doc = nlp(text)

    lemmas = []

    for token in doc:
        if not token.is_stop and token.is_alpha:
            lemmas.append(token.lemma_.lower())

    return " ".join(lemmas)

df["processed"] = df["review"].progress_apply(preprocess)

df.to_csv("data/IMDB_processed.csv", index=False)