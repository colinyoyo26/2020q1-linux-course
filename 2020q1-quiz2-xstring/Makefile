CC = gcc
CFLAGS = -std=gnu11

ifeq ("$(COW)","1")
	CFLAGS += -DCOW
endif

test_cpy: xs.c test_output_cpy.c
	$(CC) $(CFLAGS) -o $@ $^
	./$@
	@rm $@

test_tok: xs.c test_output_tok.c
	$(CC) $(CFLAGS) -o $@ $^
	./$@
	@rm $@

clean:
	$(RM) test_cpy test_tok