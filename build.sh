#!/usr/bin/env bash
set -eu

cd "$(dirname "$0")"

UCRT="/ucrt64/bin"
if [ ! -x "$UCRT/g++" ]; then
  UCRT="/mingw64/bin"
fi
if [ ! -x "$UCRT/g++" ]; then
  echo "g++ nao encontrado." >&2
  exit 1
fi

export PATH="$UCRT:$PATH"
CXX="$UCRT/g++"

mkdir -p bin
LOG=bin/compile.log
: > "$LOG"

compile_one() {
  local src="$1"
  local obj="$2"
  echo "=== $src ===" | tee -a "$LOG"
  if ! "$CXX" -std=c++17 -Wall -Isrc -c "$src" -o "$obj" >>"$LOG" 2>&1; then
    echo "Falhou: $src" >&2
    cat "$LOG" >&2
    exit 1
  fi
}

echo "Compilando com: $CXX" | tee -a "$LOG"

compile_one src/main.cpp bin/main.o
compile_one src/tokenizer.cpp bin/tokenizer.o
compile_one src/dataset.cpp bin/dataset.o
compile_one src/graph.cpp bin/graph.o
compile_one src/queue.cpp bin/queue.o

echo "=== link ===" | tee -a "$LOG"
if ! "$CXX" bin/main.o bin/tokenizer.o bin/dataset.o bin/graph.o bin/queue.o -o bin/app.exe >>"$LOG" 2>&1; then
  echo "Falhou ao linkar." >&2
  cat "$LOG" >&2
  exit 1
fi

rm -f bin/*.o
echo "OK: bin/app.exe"
