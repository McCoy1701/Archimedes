CC=gcc
ECC=emcc
CFLAGS = -Iinclude/

SRC_DIR=src
INCLUDE_DIR=include
BIN_DIR=bin
OBJ_DIR=obj

.PHONY: arch clean


arch: $(BIN_DIR)

$(BIN_DIR): $(OBJ_DIR)/aAudio.o $(OBJ_DIR)/aDraw.o $(OBJ_DIR)/aInitialize.o $(OBJ_DIR)/aInput.o $(OBJ_DIR)/aText.o
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_DIR)/aAudio.o $(OBJ_DIR)/aDraw.o $(OBJ_DIR)/aInitialize.o $(OBJ_DIR)/aInput.o $(OBJ_DIR)/aText.o $(CFLAGS) -o $(BIN_DIR)/$@


$(OBJ_DIR)/aInitialize.o: $(SRC_DIR)/aInitialize.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aDraw.o: $(SRC_DIR)/aDraw.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aText.o: $(SRC_DIR)/aText.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aInput.o: $(SRC_DIR)/aInput.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/aAudio.o: $(SRC_DIR)/aAudio.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	clear