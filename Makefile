CC=gcc
CFLAGS=-O2
OUT=htmc

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard ,$d, $2) $(filter $(subst *, %, $2),$d))
SRC=$(call rwildcard, src, *.c)
OBJ=$(patsubst src/%.c,obj/%.o, $(SRC))

.PHONEY:all
all: $(OUT)

$(OUT): obj $(OBJ)
	$(CC) $(OBJ) -o $(OUT)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $^ -o $@

obj:
	mkdir -p obj/
	mkdir -p tmp/

clean:
	rm -rf obj/; \
	rm -rf tmp/; \
	rm $(OUT)
