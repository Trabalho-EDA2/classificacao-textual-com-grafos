# Analise de Sentimentos de Avaliacoes de Filmes com Grafos

Projeto da disciplina que usa o dataset do IMDB para representar avaliacoes de filmes como um grafo. A ideia central e transformar reviews, palavras e rotulos de sentimento em nos conectados, para depois explorar essas relacoes na classificacao.

## Visao geral

O projeto segue um fluxo hibrido:

- Python prepara o texto do dataset.
- C++ constroi e povoa o grafo.

Essa divisao foi escolhida porque o preprocessamento linguistico com lematizacao e bem mais simples com SpaCy, enquanto a estrutura de dados principal foi implementada em C++.

## Fluxo de informacoes

```mermaid
flowchart LR
    A[data/IMDB_dataset.csv] --> B[scripts/preprocess.py]
    B --> C[data/IMDB_processed.csv]
    C --> D[src/dataset.cpp]
    D --> E[src/tokenizer.cpp]
    E --> F[src/new_graph.cpp]
    F --> G[Grafo com nos TEXT WORD SENTIMENT]
```

## O que o preprocessamento faz

O script [scripts/preprocess.py](scripts/preprocess.py) le o arquivo `data/IMDB_dataset.csv` e gera `data/IMDB_processed.csv`.

Durante esse passo, cada review passa por:

- tokenizacao com SpaCy
- remocao de stopwords
- remocao de artigos como `a`, `an` e `the`
- remocao de tokens que nao sao alfabeticos
- lematizacao
- conversao para minusculas

O arquivo final salvo pelo Python contem apenas duas colunas:

- `processed`: texto limpo e lematizado
- `sentiment`: rotulo original da review

Importante:
o script nao reprocessa o dataset se `data/IMDB_processed.csv` ja existir. Se voce quiser gerar o arquivo novamente, rode:

```bash
py -3 scripts/preprocess.py --force
```

## Como o grafo e montado

O programa em C++ cria tres tipos de nos:

- `TEXT`: uma review processada
- `WORD`: uma palavra da review
- `SENTIMENT`: um rotulo de sentimento

Hoje os nos de sentimento criados sao:

- `POSITIVO`
- `NEGATIVO`
- `NEUTRO`

Na pratica, o fluxo atual do dataset usa apenas `positive` e `negative`.

Para cada review lida em [src/dataset.cpp](src/dataset.cpp), o programa:

1. cria um no do tipo `TEXT`
2. identifica o sentimento da review
3. nos primeiros 80% (treino): conecta `TEXT -> SENTIMENT`
4. separa as palavras do texto processado
5. cria ou reaproveita um no `WORD` para cada palavra
6. conecta `TEXT -> WORD` (peso = frequencia da palavra no comentario)
7. conecta pares de palavras distintas do mesmo comentario via `WORD -> WORD` (peso = coocorrencia)

Os ultimos 20% das reviews formam o conjunto de teste: nao recebem aresta direta com sentimento e sao classificados via Dijkstra.

## Classificacao

Para cada comentario de teste, o algoritmo de Dijkstra calcula o menor caminho ate os nos `POSITIVO` e `NEGATIVO`, usando custo `1/peso` em cada aresta.

- caminho mais curto para `POSITIVO` -> classe `positive`
- caminho mais curto para `NEGATIVO` -> classe `negative`
- empate entre os dois caminhos -> classe `neutral` (no `NEUTRO`)

A fila encadeada em [src/queue.cpp](src/queue.cpp) e usada como estrutura auxiliar no Dijkstra (`graph::shortest_path` em [src/new_graph.cpp](src/new_graph.cpp)).

## Estrutura do projeto

```text
data/
  IMDB_dataset.csv
  IMDB_processed.csv
scripts/
  preprocess.py
src/
  main.cpp
  dataset.cpp
  dataset.hpp
  tokenizer.cpp
  tokenizer.hpp
  new_graph.cpp
  new_graph.hpp
  queue.cpp
  queue.hpp
makefile
requirements.txt
```

## Como executar

### Terminal recomendado (MSYS2)

Use o terminal **UCRT64** do MSYS2 (Menu Iniciar -> MSYS2 UCRT64), **nao** o MSYS puro.
O terminal MSYS nao inclui as DLLs do UCRT64 no PATH. Por isso `build.bat` e `build.sh` configuram `PATH` automaticamente antes de compilar.

Se estiver no MSYS ou PowerShell, compile com:

```bash
cmd //c build.bat
./bin/app.exe
```

Ou abra o **UCRT64** e rode:

```bash
make run-cpp
```

Dependencias (no UCRT64):

```bash
pacman -S make mingw-w64-ucrt64-gcc mingw-w64-ucrt64-python python-pip
```

Verifique se as ferramentas foram detectadas:

```bash
make check-tools
```

**Erro de DLL do SpaCy no MSYS:** o Python instalado no Windows pode falhar ao importar SpaCy quando chamado pelo terminal MSYS. Solucoes:

1. Se `data/IMDB_processed.csv` ja existir, pule o preprocessamento:
   ```bash
   make run-cpp
   ```
2. Para preprocessar, use o terminal **PowerShell** ou **UCRT64**:
   ```bash
   # PowerShell (na pasta do projeto)
   py -3 scripts/preprocess.py

   # ou UCRT64, apos instalar python pelo pacman
   make setup-spacy
   make preprocess
   ```
3. Se o SpaCy falhar ao importar, reinstale as dependencias nativas:
   ```bash
   py -3 -m pip install --force-reinstall numpy thinc spacy
   py -3 -m spacy download en_core_web_sm
   ```

### 1. Preparar dependencias Python

```bash
make setup
```

Esse alvo instala:

- `pandas`
- `spacy`
- `tqdm`
- modelo `en_core_web_sm`

### 2. Gerar o dataset processado

```bash
make preprocess
```

### 3. Compilar o codigo C++

```bash
make build
```

### 4. Rodar o projeto completo

```bash
make run
```

O alvo `run` executa o preprocessamento antes da aplicacao, mas so vai gerar o CSV processado se ele ainda nao existir.

## Implementacao atual

O estado atual do projeto cobre:

- preprocessamento do dataset em Python
- leitura do dataset processado em C++
- construcao do grafo com reviews, palavras e sentimentos
- arestas de coocorrencia entre palavras
- split 80% treino / 20% teste
- classificacao via Dijkstra com fila encadeada
- regra de empate classificando como `NEUTRO`
- reutilizacao de nos de palavras por meio de um dicionario
- acumulacao de peso nas arestas repetidas
- limite atual de leitura em 10.000 reviews

## Limitacoes atuais

- nao ha testes automatizados no repositorio
- o dataset IMDB nao possui rotulo `neutral`; predicoes neutras contam como erro na acuracia

## Bibliotecas usadas

### Python

- `pandas`
- `spacy`
- `tqdm`

### C++

- `iostream`
- `fstream`
- `vector`
- `string`
- `map`
- `sstream`
- `cctype`

