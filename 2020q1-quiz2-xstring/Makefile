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

plot: xs.c cpy_bench.c
	@$(CC) $(CFLAGS) -DPLOT -o $@ $^
	@bash ./script/driver.sh xstr.txt 0
	@bash ./script/driver.sh strcpy.txt 1
	@$(RM) $@ 
	@$(CC) $(CFLAGS) -DCOW -DPLOT -o $@ $^
	@bash ./script/driver.sh xstrcow.txt 0
	@gnuplot script/plot.gp
	@$(RM) $@ xstr.txt xstrcow.txt strcpy.txt


clean:
	$(RM) test_cpy test_tok bench plot xstr.txt xstrcow.txt strcpy.txt *.gp *.png