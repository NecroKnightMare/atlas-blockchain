# Makefile for libhblk_crypto.a

CC := gcc
CFLAGS := -Wall -Wextra -Werror -pedantic -I.
AR := ar rcs
SRC := sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c
OBJ := $(SRC:.c=.o)
LIB := libhblk_crypto.a

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)
