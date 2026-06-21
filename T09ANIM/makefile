CC = i686-w64-mingw32-gcc

CFLAGS = -std=gnu89 -fms-extensions -O2 -Wall \
         -Wno-implicit-function-declaration \
         -Wno-incompatible-pointer-types \
         -Wno-int-conversion \
         -Wno-unknown-pragmas \
	 -Isrc -I/usr/i686-w64-mingw32/include/GL

# Библиотеки Windows: GLEW, OpenGL, GDI, User32, математика
LIBS = -lglew32 -lopengl32 -lgdi32 -luser32 -lwinmm -lm

# Директории
SRC_DIR = src
BUILD_DIR = build
OUT_DIR = out
TARGET = $(OUT_DIR)/T09ANIM.exe

# Автоматический поиск всех .c файлов, ИСКЛЮЧАЯ glew.c
# (чтобы избежать конфликта "multiple definition" с библиотекой -lglew32)
SRCS := $(shell find $(SRC_DIR) -name '*.c' ! -name 'glew.c')
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(OUT_DIR)
	$(CC) $(OBJS) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

.PHONY: all clean
