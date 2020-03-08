CC = gcc
CFLAGS = -g -std=gnu11
TEXT = "foo bar bar barbarbarbar foooooobarrrrrrarrrrrrfoo bar bar barbarbarbar foooooobarrrrrrarrrrrrfoo bar bar barbarbarbar foooooobarrrrrrarrrrrrfoo bar bar barbarbarbar foooooobarrrrrrarrrrrr"
MODE ?= 0

ifeq ("$(COW)","1")
	CFLAGS += -DCOW
endif

test_cpy: xs.c test_output_cpy.c
	$(CC) $(CFLAGS) -o $@ $^
	./$@
	@$(RM) $@

test_tok: xs.c test_output_tok.c
	$(CC) $(CFLAGS) -o $@ $^
	./$@
	@$(RM) $@

bench: xs.c cpy_bench.c
	@$(CC) $(CFLAGS) -o $@ $^
	@sudo perf stat --repeat 1 -e cache-misses,cache-references ./$@ $(TEXT) 100000 $(MODE)
	@$(RM) $@

clean:
	$(RM) test_cpy test_tok bench