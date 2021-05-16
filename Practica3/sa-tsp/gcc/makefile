CFLAGS = -g -c

C = anneal.c sa-tsp.c
    

O = anneal.o sa-tsp.o

.c.o:
	cc $(CFLAGS) $(C)

sa-tsp: sa-tsp.o $(O)
	cc -o sa-tsp $(O) -lm
