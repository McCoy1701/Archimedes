CC = gcc
ECC = emcc
EMAR = emar rcs
CINC = -Iinclude/
CFLAGS = -Wall -Wextra -fPIC -pedantic -Iinclude/ -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcjson -lm
EFLAGS = -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2

TEM_DIR=template
SRC_DIR=src
INC_DIR=include
BIN_DIR=bin
OBJ_DIR=obj
INDEX_DIR=index
TEST_DIR=test
JSON_DIR=json

.PHONY: all
all: native

.PHONY: shared
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

$(OBJ_DIR)/aTextures.o: $(SRC_DIR)/aTextures.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/aWidgets.o: $(SRC_DIR)/aWidgets.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/libArchimedes.so: $(OBJ_DIR)/aAudio.o $(OBJ_DIR)/aDeltaTime.o $(OBJ_DIR)/aDraw.o $(OBJ_DIR)/aImage.o $(OBJ_DIR)/aInitialize.o $(OBJ_DIR)/aInput.o $(OBJ_DIR)/aText.o $(OBJ_DIR)/aTextures.o $(OBJ_DIR)/aWidgets.o
	$(CC) -shared $^ -o $@ $(CFLAGS)

.PHONY: native
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

$(OBJ_DIR)/n_aTextures.o: $(SRC_DIR)/aTextures.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_aWidgets.o: $(SRC_DIR)/aWidgets.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(BIN_DIR)/native: $(OBJ_DIR)/n_main.o $(OBJ_DIR)/n_aAudio.o $(OBJ_DIR)/n_aDeltaTime.o $(OBJ_DIR)/n_aDraw.o $(OBJ_DIR)/n_aImage.o $(OBJ_DIR)/n_aInitialize.o $(OBJ_DIR)/n_aInput.o $(OBJ_DIR)/n_aText.o $(OBJ_DIR)/n_aTextures.o $(OBJ_DIR)/n_aWidgets.o
	$(CC) $^ -ggdb $(CFLAGS) -o $@

.PHONY: test_widgets
test_widgets: always $(BIN_DIR)/test_widgets

$(OBJ_DIR)/test_widgets.o: $(TEST_DIR)/test_widgets.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(BIN_DIR)/test_widgets: $(OBJ_DIR)/test_widgets.o $(OBJ_DIR)/n_aAudio.o $(OBJ_DIR)/n_aDeltaTime.o $(OBJ_DIR)/n_aDraw.o $(OBJ_DIR)/n_aImage.o $(OBJ_DIR)/n_aInitialize.o $(OBJ_DIR)/n_aInput.o $(OBJ_DIR)/n_aText.o $(OBJ_DIR)/n_aTextures.o $(OBJ_DIR)/n_aWidgets.o
	$(CC) $^ -ggdb $(CFLAGS) -o $@


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


$(OBJ_DIR)/t_aImage.o: $(TEST_DIR)/t_aImage.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/test.o: $(TEST_DIR)/test.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(BIN_DIR)/test: $(OBJ_DIR)/n_aAudio.o $(OBJ_DIR)/n_aDeltaTime.o $(OBJ_DIR)/n_aDraw.o $(OBJ_DIR)/n_aImage.o $(OBJ_DIR)/n_aInitialize.o $(OBJ_DIR)/n_aInput.o $(OBJ_DIR)/n_aText.o $(OBJ_DIR)/n_aTextures.o $(OBJ_DIR)/n_aWidgets.o $(OBJ_DIR)/t_aImage.o $(OBJ_DIR)/test.o
	$(CC) $^ -ggdb $(CFLAGS) -o $@


.PHONY: install
install:
	sudo cp $(BIN_DIR)/libArchimedes.so /usr/lib/
	sudo cp $(INC_DIR)/Archimedes.h /usr/include/

.PHONY: uninstall
uninstall:
	sudo rm /usr/lib/libArchimedes.so
	sudo rm /usr/include/Archimedes.h

.PHONY: ainstall
ainstall:
	sudo cp $(BIN_DIR)/libArchimedes.a /usr/lib/
	sudo cp $(INC_DIR)/Archimedes.h /usr/include/

.PHONY: auninstall
auninstall:
	sudo rm /usr/lib/libArchimedes.a
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
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(INDEX_DIR)
	clear

.PHONY: always
always:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)
# =============================================================================
# TESTING
# =============================================================================
TRUE_TEST_DIR=true_tests
TEST_CFLAGS = -Wall -Wextra -ggdb $(CINC)

# Build cJSON object for tests
$(OBJ_DIR)/n_cJSON.o: $(JSON_DIR)/cJSON.c
	$(CC) -c $< $(CINC) -o $@ -ggdb -Wall -Wextra -fPIC -pedantic

# All required object files for input tests (including bundled cJSON)
INPUT_TEST_OBJS = $(OBJ_DIR)/n_aAudio.o $(OBJ_DIR)/n_aDeltaTime.o $(OBJ_DIR)/n_aDraw.o $(OBJ_DIR)/n_aImage.o $(OBJ_DIR)/n_aInitialize.o $(OBJ_DIR)/n_aInput.o $(OBJ_DIR)/n_aText.o $(OBJ_DIR)/n_aTextures.o $(OBJ_DIR)/n_aWidgets.o $(OBJ_DIR)/n_cJSON.o

# Updated library flags for tests (without system cjson)
TEST_LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

# Individual input test targets
.PHONY: test-input-basic
test-input-basic: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_input_basic $(TRUE_TEST_DIR)/input/test_input_basic.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-input-basic
run-test-input-basic: test-input-basic
	@./$(BIN_DIR)/test_input_basic

.PHONY: test-input-advanced
test-input-advanced: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_input_advanced $(TRUE_TEST_DIR)/input/test_input_advanced.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-input-advanced
run-test-input-advanced: test-input-advanced
	@./$(BIN_DIR)/test_input_advanced

.PHONY: test-input-edge
test-input-edge: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_input_edge $(TRUE_TEST_DIR)/input/test_input_edge.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-input-edge
run-test-input-edge: test-input-edge
	@./$(BIN_DIR)/test_input_edge

.PHONY: test-input-performance
test-input-performance: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_input_performance $(TRUE_TEST_DIR)/input/test_input_performance.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-input-performance
run-test-input-performance: test-input-performance
	@./$(BIN_DIR)/test_input_performance

.PHONY: test-input-errors
test-input-errors: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_input_errors $(TRUE_TEST_DIR)/input/test_input_errors.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-input-errors
run-test-input-errors: test-input-errors
	@./$(BIN_DIR)/test_input_errors

.PHONY: test-input-integration
test-input-integration: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_input_integration $(TRUE_TEST_DIR)/input/test_input_integration.c $(INPUT_TEST_OBJS) $(TEST_LIBS) -lm

.PHONY: run-test-input-integration
run-test-input-integration: test-input-integration
	@./$(BIN_DIR)/test_input_integration

.PHONY: test-image-basic
test-image-basic: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_image_basic $(TRUE_TEST_DIR)/image/test_image_basic.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-image-basic
run-test-image-basic: test-image-basic
	@./$(BIN_DIR)/test_image_basic

.PHONY: test-draw-basic
test-draw-basic: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_draw_basic $(TRUE_TEST_DIR)/draw/test_draw_basic.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-draw-basic
run-test-draw-basic: test-draw-basic
	@./$(BIN_DIR)/test_draw_basic

.PHONY: test-image-advanced
test-image-advanced: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_image_advanced $(TRUE_TEST_DIR)/image/test_image_advanced.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-image-advanced
run-test-image-advanced: test-image-advanced
	@./$(BIN_DIR)/test_image_advanced

# Global test runner (summary output)
# Traditional approach (current - kept for compatibility)
.PHONY: test
test:
	@./run_tests.sh
