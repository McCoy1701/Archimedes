# ====================================================================
# PROJECT CONFIGURATION
# ====================================================================

# Compiler and Tools
CC = gcc
ECC = emcc
EMAR = emar rcs

# Directories
SRC_DIR   = src
OBJ_DIR   = obj
INC_DIR   = include
BIN_DIR   = bin
TEST_DIR  = test
TEM_DIR   = template
JSON_DIR  = json
INDEX_DIR = index

# Object Directories (Separated for different build types)
OBJ_DIR_NATIVE = obj/native
OBJ_DIR_SHARED = obj/shared
OBJ_DIR_EM     = obj/em

#Flags
CINC = -I$(INC_DIR)/
LDLIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcjson -lm
EFLAGS = -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2

C_FLAGS = -Wall -Wextra $(CINC)
NATIVE_C_FLAGS = $(C_FLAGS) -ggdb
SHARED_C_FLAGS = $(C_FLAGS) -fPIC -pedantic


# ====================================================================
# ARCHIMEDES LIBRARY OBJECTS (Core C Files)
# ====================================================================

ARCHIMEDES_SRCS = \
    aAudio.c \
    aAUF.c \
    aAUFParser.c \
    aDeltaTime.c \
    aDraw.c \
    aImage.c \
    aInitialize.c \
    aInput.c \
    aText.c \
    aTextures.c \
    aWidgets.c

SHARED_OBJS = $(patsubst %.c, $(OBJ_DIR_SHARED)/%.o, $(ARCHIMEDES_SRCS))
NATIVE_LIB_OBJS = $(patsubst %.c, $(OBJ_DIR_NATIVE)/%.o, $(ARCHIMEDES_SRCS))

MAIN_OBJ = $(OBJ_DIR_NATIVE)/test_widgets.o

NATIVE_EXE_OBJS = $(NATIVE_LIB_OBJS) $(MAIN_OBJ)

# ====================================================================
# PHONY TARGETS
# ====================================================================

.PHONY: all shared EM EMARCH test clean install uninstall ainstall auninstall updateHeader bear bearclean
all: $(BIN_DIR)/native
shared: $(BIN_DIR)/libArchimedes.so

# Emscripten Targets
EM: $(INDEX_DIR)/index.html
EMARCH: $(BIN_DIR)/libArchimedes.a

# ====================================================================
# DIRECTORY & UTILITY RULES
# ====================================================================

# Ensure the directories exist before attempting to write files to them
$(BIN_DIR) $(OBJ_DIR_NATIVE) $(OBJ_DIR_SHARED):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_NATIVE) $(OBJ_DIR_SHARED) $(BIN_DIR)
	@clear

bear:
	bear -- make
bearclean:
	rm compile_commands.json

install: $(BIN_DIR)/libArchimedes.so
	sudo cp $< /usr/lib/
	sudo cp $(INC_DIR)/Archimedes.h /usr/include/

uninstall:
	sudo rm /usr/lib/libArchimedes.so
	sudo rm /usr/include/Archimedes.h

ainstall: $(BIN_DIR)/libArchimedes.a
	sudo cp $< /usr/lib/
	sudo cp $(INC_DIR)/Archimedes.h /usr/include/

auninstall:
	sudo rm /usr/lib/libArchimedes.a
	sudo rm /usr/include/Archimedes.h

updateHeader:
	sudo cp $(INC_DIR)/Archimedes.h /usr/include/Archimedes.h

# ====================================================================
# COMPILATION RULES
# ====================================================================

$(OBJ_DIR_SHARED)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_SHARED)
	$(CC) -c $< -o $@ $(SHARED_C_FLAGS)

$(OBJ_DIR_NATIVE)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_NATIVE)
	$(CC) -c $< -o $@ $(NATIVE_C_FLAGS)

$(OBJ_DIR_NATIVE)/test_widgets.o: $(TEST_DIR)/test_widgets.c | $(OBJ_DIR_NATIVE)
	$(CC) -c $< -o $@ $(NATIVE_C_FLAGS)

# ====================================================================
# COMPILATION RULES (Emscripten - ECC)
# ====================================================================

.PHONY: EMARCH
EMARCH: always $(BIN_DIR)/libArchimedes.a

$(OBJ_DIR)/em_cJSON.o: $(JSON_DIR)/cJSON.c
	$(ECC) -c $< $(CINC) -o $@

$(OBJ_DIR)/em_aAudio.o: $(SRC_DIR)/aAudio.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(OBJ_DIR)/em_aDeltaTime.o: $(SRC_DIR)/aDeltaTime.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(OBJ_DIR)/em_aDraw.o: $(SRC_DIR)/aDraw.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(OBJ_DIR)/em_aImage.o: $(SRC_DIR)/aImage.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(OBJ_DIR)/em_aInitialize.o: $(SRC_DIR)/aInitialize.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(OBJ_DIR)/em_aInput.o: $(SRC_DIR)/aInput.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(OBJ_DIR)/em_aText.o: $(SRC_DIR)/aText.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(OBJ_DIR)/em_aTextures.o: $(SRC_DIR)/aTextures.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(OBJ_DIR)/em_aWidgets.o: $(SRC_DIR)/aWidgets.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(BIN_DIR)/libArchimedes.a: $(OBJ_DIR)/em_cJSON.o $(OBJ_DIR)/em_aAudio.o $(OBJ_DIR)/em_aDeltaTime.o $(OBJ_DIR)/em_aDraw.o $(OBJ_DIR)/em_aImage.o $(OBJ_DIR)/em_aInitialize.o $(OBJ_DIR)/em_aInput.o $(OBJ_DIR)/em_aText.o $(OBJ_DIR)/em_aTextures.o $(OBJ_DIR)/em_aWidgets.o
	$(EMAR) $@ $^


.PHONY: EM
EM: always $(INDEX_DIR)/index

$(OBJ_DIR)/em_main.o: $(TEM_DIR)/main.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

$(INDEX_DIR)/index: $(OBJ_DIR)/em_main.o $(BIN_DIR)/libArchimedes.a
	mkdir -p $(INDEX_DIR)
	$(ECC) $^ -s WASM=1 $(EFLAGS) --shell-file htmlTemplate/template.html --preload-file resources/ -o $@.html

# ====================================================================
# LINKING RULES
# ====================================================================

# Target: Shared Library (.so)
$(BIN_DIR)/libArchimedes.so: $(SHARED_OBJS) | $(BIN_DIR)
	$(CC) -shared $^ -o $@ $(LDLIBS)

# Target: Native Executable
$(BIN_DIR)/native: $(NATIVE_EXE_OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(NATIVE_C_FLAGS) $(LDLIBS)

