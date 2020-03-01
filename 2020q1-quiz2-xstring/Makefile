CFLAGS = -g -O2 

all: bench

bench: benchmark.c rawmemchr.c orig.c
	$(CC) $(CFLAGS) -o $@ $^ 

plot: bench
	bash script/plot.sh


clean:
$(RM) bench *.png