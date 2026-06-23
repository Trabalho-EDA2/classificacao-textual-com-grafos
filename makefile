CXX = g++
PYTHON = py -3
SPACY_MODEL = en_core_web_sm
APP = app.exe

SRC = src/main.cpp src/tokenizer.cpp src/dataset.cpp src/graph.cpp
OBJ = $(SRC:.cpp=.o)

.PHONY: all setup preprocess build run clean

all: run

setup:
	$(PYTHON) -m pip install -r requirements.txt
	@$(PYTHON) -c "import spacy; spacy.load('$(SPACY_MODEL)')" >NUL 2>&1 || $(PYTHON) -m spacy download $(SPACY_MODEL)

preprocess: setup
	$(PYTHON) scripts/preprocess.py

build: $(OBJ)
	$(CXX) $(OBJ) -o $(APP)

run: preprocess build
	.\$(APP)

%.o: %.cpp
	$(CXX) -std=c++17 -Wall -c $< -o $@

clean:
	powershell -Command "Remove-Item -Force -ErrorAction SilentlyContinue 'src\\*.o', '$(APP)'"
