# starter makefile, based on: https://www.partow.net/programming/makefile/index.html

CXX      := clang
SRC_EXT  := c
CXXFLAGS := -pedantic-errors -Wall -Wextra
LDFLAGS  := 
BUILD    := ./build
DOC_DIR  := ./doc
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := pi
INCLUDE  := -Iinclude/
SRC      := $(shell find src/  -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
TEST_SRC := $(shell find src/ test/ -not -path src/main.c -a -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)

OBJECTS      := $(SRC:%.$(SRC_EXT)=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.$(SRC_EXT)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info doc

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O3 -Werror
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Tests:           ${TEST_SRC}    "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"

run:
	@$(APP_DIR)/$(TARGET)

test: $(TEST_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(APP_DIR)/test $(TEST_SRC) $(LDFLAGS)
	@$(APP_DIR)/test

doc:
	pandoc $(DOC_DIR)/documentation.md -o $(BUILD)/documentation.pdf --resource-path $(DOC_DIR)
