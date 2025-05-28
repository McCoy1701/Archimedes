CC=gcc
ECC=emcc
CFLAGS = -Wall -Wextra -fPIC -pedantic -Iinclude/ -lSDL2 -lSDL2_image -lm

SRC_DIR=src
INC_DIR=include
BIN_DIR=bin
OBJ_DIR=obj

.PHONY: all
all: shared

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

$(OBJ_DIR)/n_main.o: $(SRC_DIR)/main.c
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

