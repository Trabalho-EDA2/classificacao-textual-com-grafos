import argparse
from pathlib import Path

import pandas as pd
import spacy
from tqdm.auto import tqdm


SOURCE_PATH = Path("data/IMDB_dataset.csv")
TARGET_PATH = Path("data/IMDB_processed.csv")
SPACY_MODEL = "en_core_web_sm"


def preprocess(text, nlp):
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


def preprocess_dataset(nlp, force):
    if TARGET_PATH.exists() and not force:
        print(f"Arquivo ja existe, pulando preprocessamento: {TARGET_PATH}")
        return

    tqdm.pandas()
    df = pd.read_csv(SOURCE_PATH)
    processed_df = df[["review", "sentiment"]].copy()
    processed_df["processed"] = processed_df["review"].progress_apply(
        lambda text: preprocess(text, nlp)
    )
    processed_df = processed_df.loc[
        processed_df["processed"].str.len() > 0,
        ["processed", "sentiment"],
    ]
    processed_df.to_csv(TARGET_PATH, index=False)


def preprocess_text_file(nlp, input_path, output_path):
    text = input_path.read_text(encoding="utf-8")
    output_path.write_text(preprocess(text, nlp), encoding="utf-8")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--force", action="store_true")
    parser.add_argument("--text-file", type=Path)
    parser.add_argument("--output-text-file", type=Path)
    args = parser.parse_args()

    text_mode = args.text_file is not None or args.output_text_file is not None
    if text_mode and (args.text_file is None or args.output_text_file is None):
        parser.error("--text-file e --output-text-file devem ser usados juntos")

    if not text_mode and TARGET_PATH.exists() and not args.force:
        print(f"Arquivo ja existe, pulando preprocessamento: {TARGET_PATH}")
        return

    nlp = spacy.load(SPACY_MODEL)

    if text_mode:
        preprocess_text_file(nlp, args.text_file, args.output_text_file)
    else:
        preprocess_dataset(nlp, args.force)


if __name__ == "__main__":
    main()
