CC=gcc
OBJS= series.o user.o mtmflix.o utilities.o mtm_ex3.o mtmFlixTestEx.o
EXEC=mtmflix
CFLAG=-std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
LIB=-L. -lmtm

$(EXEC) : $(OBJS)
	$(CC) $(CFLAG) $(OBJS) -o $@ $(LIB)

mtmFlixTestEx.o: tests/mtmFlixTestEx.c tests/test_utilities.h  mtmflix.h mtm_ex3.h list.h
	 $(CC) -c tests/$*.c $(CFLAG) $(LIB)
mtmflix.o: mtmflix.c mtmflix.h mtm_ex3.h list.h set.h  map.h user.h series.h utilities.h
	 $(CC) -c $*.c $(CFLAG) $(LIB)
series.o: series.c series.h utilities.h list.h map.h mtm_ex3.h
	 $(CC) -c $*.c $(CFLAG) $(LIB)
user.o:  user.c user.h series.h utilities.h list.h map.h
	 $(CC) -c $*.c $(CFLAG) $(LIB)
utilities.o: utilities.c list.h map.h 
	 $(CC) -c $*.c $(CFLAG) $(LIB)
clean:
	rm -f $(OBJS) $(EXEC)
	
