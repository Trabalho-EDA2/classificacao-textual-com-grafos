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
CXX := $(shell command -v c++ 2>/dev/null)
endif
ifeq ($(CXX),)
CXX := $(shell test -x /ucrt64/bin/g++ && echo /ucrt64/bin/g++)
endif
ifeq ($(CXX),)
CXX := $(shell test -x /mingw64/bin/g++ && echo /mingw64/bin/g++)
endif
ifeq ($(CXX),)
$(error Compilador C++ nao encontrado. Instale g++ ou c++.)
endif

CXXFLAGS = -std=c++17 -Wall -Isrc

OBJS = $(OUT_DIR)/main.o \
       $(OUT_DIR)/tokenizer.o \
       $(OUT_DIR)/dataset.o \
       $(OUT_DIR)/graph.o \
       $(OUT_DIR)/queue.o

RUN_APP = ./$(APP)

.PHONY: all setup setup-spacy preprocess build run run-cpp clean check-tools

all: run

check-tools:
	@echo "PYTHON = $(PYTHON)"
	@echo "CXX    = $(CXX)"
	@$(PYTHON) --version || echo "Python nao encontrado."
	@$(CXX) --version | head -1

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/%.o: src/%.cpp | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(APP): $(OBJS)
	$(CXX) $(OBJS) -o $(APP)
	@echo "OK: $(APP)"

build: $(APP)

setup:
	$(PYTHON) -m pip install -r requirements.txt

setup-spacy: setup
	$(PYTHON) -c "import spacy; spacy.load('$(SPACY_MODEL)')"
	@echo "Modelo SpaCy OK: $(SPACY_MODEL)"

download-spacy: setup
	$(PYTHON) -m spacy download $(SPACY_MODEL)

preprocess:
	$(PYTHON) scripts/preprocess.py

run: preprocess build
	$(RUN_APP)

run-cpp: build
	$(RUN_APP)

clean:
	rm -rf $(OUT_DIR)
