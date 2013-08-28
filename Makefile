CC= gcc
CFLAGS= -W -Wall -O2 -c -Ispine-runtimes/spine-c/include -Isrc
ARCHIVER= ar
ARFLAGS= crufs

TARGET= spine-allegro.a

SOURCES_1= $(wildcard src/spine/*.c)
OBJECTS_1= $(SOURCES_1:.c=.o)

SOURCES_2= $(wildcard spine-runtimes/spine-c/src/spine/*.c)
OBJECTS_2= $(SOURCES_2:.c=.o)

all: $(OUTDIR) $(TARGET)

$(TARGET): $(OBJECTS_1) $(OBJECTS_2)
	$(ARCHIVER) $(ARFLAGS) $(TARGET) $(OBJECTS_1) $(OBJECTS_2)

%.o: %.c
	$(CC) -o $@ $(CFLAGS) $^

.PHONY: clean mrproper

clean:
	rm $(OBJECTS_1) $(OBJECTS_2)

mrproper: clean
	rm $(TARGET)

