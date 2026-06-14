CXX = g++
PYTHON = python
SPACY_MODEL = en_core_web_sm
APP = app.exe

SRC = src/main.cpp src/tokenizer.cpp
OBJ = $(SRC:.cpp=.o)

.PHONY: all setup preprocess build run clean

all: run

setup:
	$(PYTHON) -m pip install -r requirements.txt
	@$(PYTHON) -c "import spacy; spacy.load('$(SPACY_MODEL)')" >/dev/null 2>&1 || $(PYTHON) -m spacy download $(SPACY_MODEL)

preprocess: setup
	$(PYTHON) scripts/preprocess.py

build: preprocess $(OBJ)
	$(CXX) $(OBJ) -o $(APP)

run: build
	./$(APP)

%.o: %.cpp
	$(CXX) -std=c++17 -Wall -c $< -o $@

clean:
	rm -f src/*.o $(APP) app