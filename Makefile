CC = cc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude -Werror
TARGET = vmr5
CORE_DIR = src/core
UTILS_DIR = src/utils
PLAT_DIR = src/plat
CORE_SOURCES = $(wildcard $(CORE_DIR)/*.c)
UTILS_SOURCES = $(wildcard $(UTILS_DIR)/*.c)
PLAT_SOURCES = $(wildcard $(PLAT_DIR)/*.c)

SOURCES = $(CORE_SOURCES) $(UTILS_SOURCES) $(PLAT_SOURCES)
OBJECTS = $(SOURCES:.c=.o)
MAIN_OBJ = src/main.o

.PHONY: all clean debug release

all: $(TARGET)

debug: CFLAGS += -DDEBUG -O0 -g
debug: $(TARGET)

release: CFLAGS += -O2
release: $(TARGET)

$(TARGET): $(OBJECTS) $(MAIN_OBJ)
	$(CC) $(OBJECTS) $(MAIN_OBJ) -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJECTS) $(MAIN_OBJ) $(TARGET) || rm -f $(OBJECTS) $(MAIN_OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)
