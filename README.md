# Classificação de textos com grafos

Classificador binário de sentimentos para reviews do IMDB. O pré-processamento é feito em Python com SpaCy; o grafo, a fila e a busca em largura (BFS) foram implementados em C++.

## Fluxo

```text
IMDB_dataset.csv
        |
        v
preprocess.py (SpaCy)
        |
        v
IMDB_processed.csv
        |
        v
Grafo TEXT <-> WORD
      TEXT <-> SENTIMENT
        |
        v
BFS: WORD -> TEXT -> SENTIMENT
```

O SpaCy aplica às reviews:

- conversão para minúsculas;
- tokenização e lematização;
- remoção de stopwords e tokens não alfabéticos.

O CSV resultante contém as colunas `processed` e `sentiment`.

## Grafo e classificação

O grafo possui três tipos de nós:

- `TEXT`: review processada;
- `WORD`: palavra compartilhada entre reviews;
- `SENTIMENT`: `POSITIVO` ou `NEGATIVO`.

Não existem arestas diretas entre palavras e sentimentos. A BFS é usada dentro de `classify_review`, em [src/dataset.cpp](src/dataset.cpp), para localizar reviews de treino relacionadas à entrada. Para cada palavra distinta da consulta, o código chama `bounded_bfs` com profundidade máxima 2 e restringe os tipos aceitos em cada nível:

```text
WORD -> TEXT -> SENTIMENT
```

O método `bounded_bfs`, implementado em [src/graph.cpp](src/graph.cpp), funciona assim:

1. coloca a palavra inicial na fila com profundidade `0`;
2. retira os nós na ordem de chegada;
3. em `WORD`, adiciona apenas vizinhos `TEXT`;
4. em `TEXT`, adiciona apenas o vizinho `SENTIMENT`;
5. interrompe a expansão ao atingir a profundidade `2`.

Um vetor de visitados impede que o mesmo nó seja enfileirado mais de uma vez durante cada busca. Os nós `TEXT` encontrados no nível `1` são contabilizados como reviews candidatas. Uma review pode receber correspondências de diferentes palavras da consulta.

### Fila da BFS

A classe `bfs_queue`, declarada em [src/graph.hpp](src/graph.hpp) e implementada em `src/graph.cpp`, substitui `std::queue`. Ela usa um vetor dinâmico circular e armazena, em cada item, o ponteiro para o nó e sua profundidade.

- `enqueue`: insere no final da fila;
- `dequeue`: remove do início;
- `head` e `count`: controlam a região ocupada sem deslocar elementos;
- `grow`: dobra a capacidade quando a fila fica cheia e preserva a ordem.

Essa fila garante a exploração por níveis característica da BFS. Como a busca é limitada a dois níveis e filtrada por tipo, ela não percorre o grafo inteiro.

Cada review candidata recebe o seguinte peso:

```text
score = palavras compartilhadas / palavras distintas da review
```

Os scores são somados por sentimento, sem TF-IDF. Em caso de empate, vence a classe mais frequente no treino.

## Treino e avaliação

O fluxo atual usa `data/IMDB_processed.csv`:

- primeiras 1.000 reviews: treinamento;
- 1.000 reviews seguintes: teste de acurácia.

Esses conjuntos não se sobrepõem. Na execução atual, o programa também apresenta duas classificações de exemplo antes da avaliação.

## Execução

Requisitos: Python 3, GNU Make e compilador C++ com suporte a C++17.

```bash
make setup       # instala pandas, spacy, tqdm e en_core_web_sm
make preprocess  # gera data/IMDB_processed.csv
make build       # compila app.exe
make run         # executa o fluxo completo
```

Para recriar o CSV processado:

```bash
py -3 scripts/preprocess.py --force
```

Para remover objetos e o executável:

```bash
make clean
```

## Entrada interativa

Depois da avaliação, o programa aceita reviews em inglês pelo teclado:

```text
> I loved this amazing movie
Classificacao: positive
```

A entrada é normalizada para minúsculas e passa pelo mesmo pipeline SpaCy usado no dataset antes de ser enviada ao classificador. Digite `sair` ou uma linha vazia para encerrar.

## Estrutura principal

```text
data/                 datasets original e processado
scripts/preprocess.py pré-processamento SpaCy
src/graph.*           grafo, fila circular e BFS
src/dataset.*         treino, classificação e acurácia
src/tokenizer.*       tokenização auxiliar em C++
src/main.cpp          fluxo principal e modo interativo
```

## Limitações

- cada entrada interativa inicia um novo processo Python para carregar o SpaCy;
- não há testes unitários automatizados.
