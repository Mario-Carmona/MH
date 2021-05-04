CFLAGS = -g -c

C = adap.c ag-moch.c cross.c elitist.c evaluate.c init.c input.c\
	mutate.c select.c

O = adap.o ag-moch.c cross.o elitist.o evaluate.o init.o input.o\
	mutate.o select.o

.c.o:
	cc $(CFLAGS) $(C)

ag-moch: ag-moch.o $(O)
	cc -o ag-moch $(O) -lm
