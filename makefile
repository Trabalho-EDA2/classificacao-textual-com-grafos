SPACY_MODEL = en_core_web_sm
OUT_DIR = bin
APP = $(OUT_DIR)/app.exe
PROCESSED_CSV = data/IMDB_processed.csv

SRC = src/main.cpp src/tokenizer.cpp src/dataset.cpp src/graph.cpp src/queue.cpp

WINPWD := $(shell pwd -W 2>/dev/null)

ifdef MSYSTEM
PYTHON := $(shell test -x /ucrt64/bin/python && echo /ucrt64/bin/python)
endif
ifeq ($(PYTHON),)
PYTHON := $(shell command -v python3 2>/dev/null)
endif
ifeq ($(PYTHON),)
PYTHON := $(shell command -v python 2>/dev/null)
endif
ifeq ($(PYTHON),)
PYTHON := $(shell test -x /mingw64/bin/python && echo /mingw64/bin/python)
endif
ifeq ($(PYTHON),)
PYTHON := $(shell ls /c/Users/*/AppData/Local/Programs/Python/Python*/python.exe 2>/dev/null | tail -1)
endif
ifeq ($(PYTHON),)
PYTHON := py -3
endif

CXX := $(shell command -v g++ 2>/dev/null)
ifeq ($(CXX),)
CXX := $(shell test -x /ucrt64/bin/g++ && echo /ucrt64/bin/g++)
endif
ifeq ($(CXX),)
CXX := $(shell test -x /mingw64/bin/g++ && echo /mingw64/bin/g++)
endif
ifeq ($(CXX),)
CXX := g++
endif

CXXFLAGS = -std=c++17 -Wall -Isrc

RUN_APP = ./$(APP)

.PHONY: all setup setup-spacy preprocess build run run-cpp clean check-tools

all: run

check-tools:
	@echo "PYTHON = $(PYTHON)"
	@echo "CXX    = $(CXX)"
	@echo "WINPWD = $(WINPWD)"
	@$(PYTHON) --version || echo "Python nao encontrado."
	@$(CXX) --version || echo "g++ nao encontrado."

setup:
	$(PYTHON) -m pip install -r requirements.txt

setup-spacy: setup
	$(PYTHON) -c "import spacy; spacy.load('$(SPACY_MODEL)')"
	@echo "Modelo SpaCy OK: $(SPACY_MODEL)"

download-spacy: setup
	$(PYTHON) -m spacy download $(SPACY_MODEL)

preprocess:
	$(PYTHON) scripts/preprocess.py

build:
	@bash "$(CURDIR)/build.sh"

run: preprocess build
	$(RUN_APP)

run-cpp: build
	$(RUN_APP)

clean:
	rm -rf bin build app.exe
