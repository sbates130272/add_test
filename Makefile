EXE=add_test
CFLAGS+=-g

default:$(EXE)

run: $(EXE)
	./run 1000

clean:
	rm -rf *~ $(EXE) *.stat *.log *.tmp
