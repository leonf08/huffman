CFLAGS = -Wall -Os ${INCLUDES}

SOURCES = 
SOURCES += MainProgram/src/main.c
SOURCES += HuffmanAlgorithm/src/HuffmanAlgorithm.c
SOURCES += TextFileHandler/src/TextFileHandler.c

INCLUDES = 
INCLUDES += -I"MainProgram/include"
INCLUDES += -I"HuffmanAlgorithm/include"
INCLUDES += -I"TextFileHandler/include"

OBJS = ${SOURCES:.c=.o}

EXECUTABLE = huff

all: ${SOURCES} ${EXECUTABLE}

${EXECUTABLE}: ${OBJS}
	${CC} ${OBJS} -o $@

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm ${OBJS} ${EXECUTABLE}