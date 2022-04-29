cc=g++
target = str_exe

objects = str.o main.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

%.o : %.c
	$(cc) -c -o $@ $<

str.o main.o : Str.h

.PHONY : clean
clean :
	rm $(target) $(objects)
