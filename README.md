# Classificação de textos com grafos

Classificador binário de sentimento para reviews do IMDB. O pré-processamento é feito em Python com SpaCy; o grafo, a fila e a busca em largura (BFS) são implementados em C++.

## Requisitos

- Python 3, `g++` com suporte a C++17 e GNU Make
- No Windows, o projeto usa `py -3` (ver [makefile](makefile))
- Dataset original em `data/IMDB_dataset.csv` com colunas `review` e `sentiment` — não versionado; coloque o arquivo manualmente antes do preprocessamento

## Comandos

```bash
make setup       # instala dependências Python e modelo SpaCy en_core_web_sm
make preprocess  # gera data/IMDB_processed.csv
make build       # compila app.exe
make run         # preprocess + build + executa
make clean       # remove .o e app.exe
```

Comandos avulsos:

```bash
py -3 scripts/preprocess.py --force   # reprocessar o dataset
.\app.exe                             # executar (após build)
```

Fluxo:

```text
IMDB_dataset.csv → preprocess.py → IMDB_processed.csv → grafo → BFS → classificação
```

## Como o algoritmo funciona

O grafo possui três tipos de nós:

- `WORD`: palavra compartilhada entre reviews
- `TEXT`: review processada
- `SENTIMENT`: `POSITIVO` ou `NEGATIVO`

As arestas ligam `TEXT ↔ WORD` e `TEXT ↔ SENTIMENT`. Não há ligação direta entre palavras e sentimentos.

Para classificar uma review (`classify_review` em [src/dataset.cpp](src/dataset.cpp)):

1. Para cada palavra da consulta, executa BFS com profundidade máxima 2 no caminho `WORD → TEXT → SENTIMENT`.
2. Cada review de treino encontrada recebe peso: `palavras em comum / palavras distintas da review`.
3. Os pesos são somados por sentimento; vence o maior score. Em empate, prevalece a classe mais frequente no treino.

```mermaid
flowchart LR
  query[Review consulta] --> words[Palavras]
  words --> bfs[BFS profundidade 2]
  bfs --> texts[Reviews de treino]
  texts --> score[Soma de scores]
  score --> result[positive ou negative]
```

**Treino e avaliação:** as primeiras 3.000 reviews de `data/IMDB_processed.csv` populam o grafo; as 1.000 seguintes são usadas para medir acurácia. O programa também exibe duas classificações de exemplo antes da avaliação.

O pré-processamento SpaCy aplica minúsculas, lematização e remoção de stopwords e tokens não alfabéticos.

## Uso interativo

Após a avaliação, digite reviews em inglês:

```text
> I loved this amazing movie
Classificacao: positive
```

A entrada passa pelo mesmo pipeline SpaCy do dataset. Digite `sair` ou uma linha vazia para encerrar. A saída é `positive` ou `negative`.

## Estrutura do projeto

```text
data/                 datasets original e processado
scripts/preprocess.py pré-processamento SpaCy
src/graph.*           grafo e BFS
src/dataset.*         treino, classificação e acurácia
src/main.cpp          fluxo principal e modo interativo
```

## Limitações

- Cada entrada interativa invoca Python/SpaCy (lento).
- O dataset original não está incluído no repositório.
