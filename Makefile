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
all: $(BIN_DIR)/native

NATIVE_OBJS = \
							$(OBJ_DIR)/aAudio.o\
							$(OBJ_DIR)/aAUFParser.o\
							$(OBJ_DIR)/aDeltaTime.o\
							$(OBJ_DIR)/aDraw.o\
							$(OBJ_DIR)/aImage.o\
							$(OBJ_DIR)/aInitialize.o\
							$(OBJ_DIR)/aInput.o\
							$(OBJ_DIR)/aText.o\
							$(OBJ_DIR)/aTextures.o\
							$(OBJ_DIR)/aWidgets.o

TEMPLATE_OBJS = \
							$(OBJ_DIR)/test_widgets.o\

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(BIN_DIR)/native: $(NATIVE_OBJS) $(TEMPLATE_OBJS) | $(BIN_DIR)
	$(CC) $^ -ggdb $(CFLAGS) -o $@


.PHONY: shared
shared: $(BIN_DIR)/libArchimedes.so

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/libArchimedes.so: $(NATIVE_OBJS) | $(BIN_DIR)
	$(CC) -shared $^ -o $@ $(CFLAGS)


.PHONY: test_widgets
test_widgets: $(BIN_DIR)/test_widgets

TEST_WIDGETS_OBJS = \
							$(OBJ_DIR)/test_widgets.o

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(BIN_DIR)/test_widgets: $(NATIVE_OBJS) $(TEST_WIDGETS_OBJS) | $(BIN_DIR)
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


$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)


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

# Build test-specific objects
$(OBJ_DIR)/n_aInitializeTest.o: $(SRC_DIR)/aInitializeTest.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

$(OBJ_DIR)/n_test_fonts.o: resources/fonts/test_fonts.c
	$(CC) -c $< -o $@ -ggdb $(CFLAGS)

# All required object files for input tests (including bundled cJSON)
INPUT_TEST_OBJS = $(OBJ_DIR)/n_aAudio.o $(OBJ_DIR)/n_aDeltaTime.o $(OBJ_DIR)/n_aDraw.o $(OBJ_DIR)/n_aImage.o $(OBJ_DIR)/n_aInitialize.o $(OBJ_DIR)/n_aInput.o $(OBJ_DIR)/n_aText.o $(OBJ_DIR)/n_aTextures.o $(OBJ_DIR)/n_aWidgets.o $(OBJ_DIR)/n_cJSON.o

# Test-specific object files for initialize tests
# We need both aInitialize (for a_Quit) and aInitializeTest (for a_InitTest)
# But we can't have both because they both define 'app'. So we need to extract a_Quit separately.
INIT_TEST_OBJS = $(OBJ_DIR)/n_aAudio.o $(OBJ_DIR)/n_aDeltaTime.o $(OBJ_DIR)/n_aDraw.o $(OBJ_DIR)/n_aImage.o $(OBJ_DIR)/n_aInitializeTest.o $(OBJ_DIR)/n_aInput.o $(OBJ_DIR)/n_aText.o $(OBJ_DIR)/n_aTextures.o $(OBJ_DIR)/n_aWidgets.o $(OBJ_DIR)/n_cJSON.o $(OBJ_DIR)/n_test_fonts.o

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
test-draw-basic: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_draw_basic $(TRUE_TEST_DIR)/draw/test_draw_basic.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-draw-basic
run-test-draw-basic: test-draw-basic
	@./$(BIN_DIR)/test_draw_basic

.PHONY: test-image-advanced
test-image-advanced: always $(INPUT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_image_advanced $(TRUE_TEST_DIR)/image/test_image_advanced.c $(INPUT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-image-advanced
run-test-image-advanced: test-image-advanced
	@./$(BIN_DIR)/test_image_advanced

.PHONY: test-widgets-basic
test-widgets-basic: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_widgets_basic $(TRUE_TEST_DIR)/widgets/test_widgets_basic.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-widgets-basic
run-test-widgets-basic: test-widgets-basic
	@./$(BIN_DIR)/test_widgets_basic

.PHONY: test-widgets-advanced
test-widgets-advanced: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_widgets_advanced $(TRUE_TEST_DIR)/widgets/test_widgets_advanced.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-widgets-advanced
run-test-widgets-advanced: test-widgets-advanced
	@./$(BIN_DIR)/test_widgets_advanced

.PHONY: test-initialize-basic
test-initialize-basic: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_initialize_basic $(TRUE_TEST_DIR)/initialize/test_initialize_basic.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-initialize-basic
run-test-initialize-basic: test-initialize-basic
	@./$(BIN_DIR)/test_initialize_basic

.PHONY: test-text-basic
test-text-basic: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_text_basic $(TRUE_TEST_DIR)/text/test_text_basic.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-text-basic
run-test-text-basic: test-text-basic
	@./$(BIN_DIR)/test_text_basic

.PHONY: test-initialize-edge
test-initialize-edge: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_initialize_edge $(TRUE_TEST_DIR)/initialize/test_initialize_edge.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-initialize-edge
run-test-initialize-edge: test-initialize-edge
	@./$(BIN_DIR)/test_initialize_edge

.PHONY: test-text-edge
test-text-edge: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_text_edge $(TRUE_TEST_DIR)/text/test_text_edge.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-text-edge
run-test-text-edge: test-text-edge
	@./$(BIN_DIR)/test_text_edge

.PHONY: test-text-unified
test-text-unified: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_text_unified $(TRUE_TEST_DIR)/text/test_text_unified.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-text-unified
run-test-text-unified: test-text-unified
	@./$(BIN_DIR)/test_text_unified

.PHONY: test-init-simple
test-init-simple: always $(INIT_TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(BIN_DIR)/test_init_simple $(TRUE_TEST_DIR)/initialize/test_init_simple.c $(INIT_TEST_OBJS) $(TEST_LIBS)

.PHONY: run-test-init-simple
run-test-init-simple: test-init-simple
	@./$(BIN_DIR)/test_init_simple

# =============================================================================
# EMSCRIPTEN TESTS
# =============================================================================

# Emscripten test directories
EMSCRIPTEN_TEST_DIR = $(TRUE_TEST_DIR)/emscripten
EMSCRIPTEN_OUTPUT_DIR = $(INDEX_DIR)/emscripten

# Emscripten test flags
EMSCRIPTEN_TEST_FLAGS = -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS='["_main","_exit","_emscripten_next_scene","_emscripten_previous_scene","_emscripten_restart_scene","_emscripten_toggle_auto_advance"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","print","printErr"]' -s EXIT_RUNTIME=1 --preload-file resources/

# Emscripten test base objects
$(OBJ_DIR)/em_emscripten_test_base.o: $(EMSCRIPTEN_TEST_DIR)/emscripten_test_base.c
	$(ECC) -c $< $(CINC) $(EFLAGS) -o $@

# Emscripten test targets
.PHONY: test-emscripten-text-draw-init
test-emscripten-text-draw-init: always $(BIN_DIR)/libArchimedes.a $(OBJ_DIR)/em_emscripten_test_base.o
	mkdir -p $(EMSCRIPTEN_OUTPUT_DIR)
	$(ECC) $(CINC) $(EMSCRIPTEN_TEST_FLAGS) --shell-file $(EMSCRIPTEN_TEST_DIR)/template.html -o $(EMSCRIPTEN_OUTPUT_DIR)/test_emscripten_text_draw_init.html $(EMSCRIPTEN_TEST_DIR)/test_emscripten_text_draw_init.c $(OBJ_DIR)/em_emscripten_test_base.o $(BIN_DIR)/libArchimedes.a

.PHONY: run-test-emscripten-text-draw-init
run-test-emscripten-text-draw-init: test-emscripten-text-draw-init
	@echo "=== Starting Emscripten Test Server ==="
	@echo "Opening test_emscripten_text_draw_init.html in browser..."
	@echo "Use Ctrl+C to stop the server"
	@cd $(EMSCRIPTEN_OUTPUT_DIR) && python3 -m http.server 8080 &
	@sleep 2
	@xdg-open "http://localhost:8080/test_emscripten_text_draw_init.html" || echo "Please open http://localhost:8080/test_emscripten_text_draw_init.html in your browser"

.PHONY: test-emscripten-widgets
test-emscripten-widgets: always $(BIN_DIR)/libArchimedes.a $(OBJ_DIR)/em_emscripten_test_base.o
	mkdir -p $(EMSCRIPTEN_OUTPUT_DIR)
	$(ECC) $(CINC) $(EMSCRIPTEN_TEST_FLAGS) --shell-file $(EMSCRIPTEN_TEST_DIR)/template.html --preload-file $(EMSCRIPTEN_TEST_DIR)/../emscripten_widgets/test_emscripten_widgets.json@test_emscripten_widgets.json -o $(EMSCRIPTEN_OUTPUT_DIR)/test_emscripten_widgets.html $(EMSCRIPTEN_TEST_DIR)/../emscripten_widgets/test_emscripten_widgets.c $(OBJ_DIR)/em_emscripten_test_base.o $(BIN_DIR)/libArchimedes.a
.PHONY: run-test-emscripten-widgets
run-test-emscripten-widgets: test-emscripten-widgets
	@echo "=== Starting Sacred Chamber of Divine Automata ==="
	@echo "🏛️ Opening the Sacred Web Portal..."
	@echo "⚡ Use Ctrl+C to exit the divine realm"
	@cd $(EMSCRIPTEN_OUTPUT_DIR) && python3 -m http.server 8080 &
	@sleep 2
	@xdg-open "http://localhost:8080/test_emscripten_widgets.html" || echo "🌟 Please open http://localhost:8080/test_emscripten_widgets.html to enter the Sacred Chamber!"
.PHONY: test-emscripten-all
test-emscripten-all: test-emscripten-text-draw-init test-emscripten-widgets
	@echo "All Emscripten tests compiled successfully"

.PHONY: clean-emscripten
clean-emscripten:
	rm -rf $(EMSCRIPTEN_OUTPUT_DIR)

# Global test runner (summary output)
# Traditional approach (current - kept for compatibility)
.PHONY: test
test:
	@./run_tests.sh
