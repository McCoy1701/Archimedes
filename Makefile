CC=gcc
ECC=emcc
CFLAGS = -Wall -pedantic -Iinclude/ -ggdb -lSDL2 -lSDL2_image -lm

SRC_DIR=src
INCLUDE_DIR=include
BIN_DIR=bin
OBJ_DIR=obj

.PHONY: arch clean


arch: $(BIN_DIR)

$(BIN_DIR): $(OBJ_DIR)/main.o $(OBJ_DIR)/aAudio.o $(OBJ_DIR)/aDraw.o $(OBJ_DIR)/aInitialize.o $(OBJ_DIR)/aInput.o
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_DIR)/main.o $(OBJ_DIR)/aAudio.o $(OBJ_DIR)/aDraw.o $(OBJ_DIR)/aInitialize.o $(OBJ_DIR)/aInput.o -ldaedalus $(CFLAGS) -o $(BIN_DIR)/$@


$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aAudio.o: $(SRC_DIR)/aAudio.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aDraw.o: $(SRC_DIR)/aDraw.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aImage.o: $(SRC_DIR)/aImage.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aInitialize.o: $(SRC_DIR)/aInitialize.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aInput.o: $(SRC_DIR)/aInput.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aText.o: $(SRC_DIR)/aText.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	clear