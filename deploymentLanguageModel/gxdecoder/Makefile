# 指定编译器
CC=gcc
# 指定编译选项
CFLAGS=-O0 -g -Wall -Wextra -I./ -I./include

LIBS =-lc -lm


# 所有源文件
SRCS=
# 所有头文件
HDRS=
# 所有目标文件
OBJS=$(SRCS:.c=.o)

# 生成可执行文件
TARGET=test.elf

all: $(TARGET)

$(TARGET): $(OBJS) test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJS) test.o
