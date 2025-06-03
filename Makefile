CC = gcc
ECC = emcc
CINC = -Iinclude/
CFLAGS = -Wall -Wextra -fPIC -pedantic -Iinclude/ -lSDL2 -lSDL2_image -lSDL2_ttf -lm
EFLAGS = -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'

TEM_DIR=template
SRC_DIR=src
INC_DIR=include
BIN_DIR=bin
OBJ_DIR=obj
INDEX_DIR=index

.PHONY: all
all: native

shared: always $(BIN_DIR)/libArchimedes.so

$(OBJ_DIR)/aAudio.o: $(SRC_DIR)/aAudio.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/aDeltaTime.o: $(SRC_DIR)/aDeltaTime.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/aDraw.o: $(SRC_DIR)/aDraw.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/aImage.o: $(SRC_DIR)/aImage.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/aInitialize.o: $(SRC_DIR)/aInitialize.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/aInput.o: $(SRC_DIR)/aInput.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/aText.o: $(SRC_DIR)/aText.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/libArchimedes.so: $(OBJ_DIR)/aAudio.o $(OBJ_DIR)/aDeltaTime.o $(OBJ_DIR)/aDraw.o $(OBJ_DIR)/aImage.o $(OBJ_DIR)/aInitialize.o $(OBJ_DIR)/aInput.o $(OBJ_DIR)/aText.o
	$(CC) -shared $^ -lDaedalus -o $@ $(CFLAGS)


native: always $(BIN_DIR)/native

$(OBJ_DIR)/n_main.o: $(TEM_DIR)/main.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_aAudio.o: $(SRC_DIR)/aAudio.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_aDeltaTime.o: $(SRC_DIR)/aDeltaTime.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_aDraw.o: $(SRC_DIR)/aDraw.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_aImage.o: $(SRC_DIR)/aImage.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_aInitialize.o: $(SRC_DIR)/aInitialize.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_aInput.o: $(SRC_DIR)/aInput.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_aText.o: $(SRC_DIR)/aText.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(BIN_DIR)/native: $(OBJ_DIR)/n_main.o $(OBJ_DIR)/n_aAudio.o $(OBJ_DIR)/n_aDeltaTime.o $(OBJ_DIR)/n_aDraw.o $(OBJ_DIR)/n_aImage.o $(OBJ_DIR)/n_aInitialize.o $(OBJ_DIR)/n_aInput.o $(OBJ_DIR)/n_aText.o
	$(CC) $^ -ggdb -lDaedalus $(CFLAGS) -o $@


EM: always $(INDEX_DIR)

$(OBJ_DIR)/em_main.o: $(TEM_DIR)/main.c
	$(ECC) -c $< $(CINC) -o $@

$(OBJ_DIR)/em_aAudio.o: $(SRC_DIR)/aAudio.c
	$(ECC) -c $< $(CINC) -o $@

$(OBJ_DIR)/em_aDeltaTime.o: $(SRC_DIR)/aDeltaTime.c
	$(ECC) -c $< $(CINC) -o $@

$(OBJ_DIR)/em_aDraw.o: $(SRC_DIR)/aDraw.c
	$(ECC) -c $< $(CINC) -o $@

$(OBJ_DIR)/em_aImage.o: $(SRC_DIR)/aImage.c
	$(ECC) -c $< $(CINC) -o $@

$(OBJ_DIR)/em_aInitialize.o: $(SRC_DIR)/aInitialize.c
	$(ECC) -c $< $(CINC) -o $@

$(OBJ_DIR)/em_aInput.o: $(SRC_DIR)/aInput.c
	$(ECC) -c $< $(CINC) -o $@

$(OBJ_DIR)/em_aText.o: $(SRC_DIR)/aText.c
	$(ECC) -c $< $(CINC) -o $@


$(BIN_DIR)/em_native: $(OBJ_DIR)/em_main.o $(OBJ_DIR)/em_aAudio.o $(OBJ_DIR)/em_aDeltaTime.o $(OBJ_DIR)/em_aDraw.o $(OBJ_DIR)/em_aImage.o $(OBJ_DIR)/em_aInitialize.o $(OBJ_DIR)/em_aInput.o $(OBJ_DIR)/em_aText.o
	$(ECC) $^ -ggdb -lDaedalus $(CFLAGS) -o $@

$(INDEX_DIR): $(OBJ_DIR)/em_main.o $(OBJ_DIR)/em_aAudio.o $(OBJ_DIR)/em_aDeltaTime.o $(OBJ_DIR)/em_aDraw.o $(OBJ_DIR)/em_aImage.o $(OBJ_DIR)/em_aInitialize.o $(OBJ_DIR)/em_aInput.o $(OBJ_DIR)/em_aText.o
	mkdir -p $(INDEX_DIR)
	$(ECC) $^ -s WASM=1 $(EFLAGS) --shell-file htmlTemplate/template.html --preload-file assets -o $(INDEX_DIR)/$@.html


.PHONY: install
install:
	sudo cp $(BIN_DIR)/libArchimedes.so /usr/lib/libArchimedes.so
	sudo cp $(INC_DIR)/Archimedes.h /usr/include/Archimedes.h

.PHONY: uninstall
uninstall:
	sudo rm /usr/lib/libArchimedes.so
	sudo rm /usr/include/Archimedes.h

.PHONY: updateHeader
updateHeader:
	sudo cp $(INC_DIR)/Archimedes.h /usr/include/Archimedes.h

.PHONY: bear
bear:
	bear -- make

.PHONY: bearclean
bearclean:
	rm compile_commands.json

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	clear

.PHONY: always
always:
	mkdir $(OBJ_DIR) $(BIN_DIR)

