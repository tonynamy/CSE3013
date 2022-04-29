cc=gcc
target = calc_exe

objects = calc.o print.o main.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

%.o : %.c
	$(cc) -c -o $@ $<

calc.o print.o main.o : header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
