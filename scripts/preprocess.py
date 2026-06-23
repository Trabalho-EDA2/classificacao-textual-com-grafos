from pathlib import Path
import sys

import pandas as pd
import spacy
from tqdm.auto import tqdm


SOURCE_PATH = Path("data/IMDB_dataset.csv")
TARGET_PATH = Path("data/IMDB_processed.csv")
FORCE_REPROCESS = "--force" in sys.argv

if TARGET_PATH.exists() and not FORCE_REPROCESS:
    print(f"Arquivo ja existe, pulando preprocessamento: {TARGET_PATH}")
    sys.exit(0)

tqdm.pandas()

nlp = spacy.load("en_core_web_sm")

df = pd.read_csv(SOURCE_PATH)

def preprocess(text):
    doc = nlp(text)

    lemmas = []

    for token in doc:
        if not token.is_alpha or token.is_stop:
            continue

        lemma = token.lemma_.lower().strip()
        if lemma in {"a", "an", "the"}:
            continue
        if not lemma or lemma == "-pron-":
            continue

        lemmas.append(lemma)

    return " ".join(lemmas)

processed_df = df[["review", "sentiment"]].copy()
processed_df["processed"] = processed_df["review"].progress_apply(preprocess)
processed_df = processed_df.loc[processed_df["processed"].str.len() > 0, ["processed", "sentiment"]]

processed_df.to_csv(TARGET_PATH, index=False)
