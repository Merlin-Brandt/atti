EXECUTABLE=install/atti
SOURCES=src/main.cpp src/pages/pages.cpp src/pages/TrovaLaFrase.cpp src/gui/button.cpp
HEADERS=$(wildcard src/*.hpp src/*/*.hpp src/*/*/*.hpp)
DOCS_ORDER=src/main.cpp
SFML_LIBS=lib/sfml/build/lib/libsfml
CC=g++
CFLAGS=-std=c++17 -w
#LDFLAGS=-static -static-libgcc -static-libstdc++ $(SFML_LIBS)-audio-s.a $(SFML_LIBS)-graphics-s.a $(SFML_LIBS)-window-s.a $(SFML_LIBS)-system-s.a
LDFLAGS=-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

OBJECTS_TMP=$(SOURCES:.cpp=.o)
OBJECTS=$(OBJECTS_TMP:.c=.o)

all: $(EXECUTABLE) readme.txt

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o$(EXECUTABLE) $(OBJECTS) $(LDFLAGS)
	
%.o: %.cpp $(HEADERS)
	$(CC) -o$@ -c $< $(CFLAGS)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	mkdir -p backup
	cp -tbackup $(OBJECTS) $(EXECUTABLE) $(SOURCES)
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
	
backup:
	mkdir -p backup
	cp -tbackup $(OBJECTS) $(EXECUTABLE) $(SOURCES)

docs: readme.txt

readme.txt: $(DOCS_ORDER)
	cat $(DOCS_ORDER) | sed -n '/\/\/r/p' | sed 's/\s*\/\/r\s//' > readme.txt

