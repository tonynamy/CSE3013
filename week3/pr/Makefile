cc=g++
target = array_exe

objects = array.o rangearray.o main.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

%.o : %.c
	$(cc) -c -o $@ $<

array.o : Array.h
RangeArray.o : RangeArray.h

.PHONY : clean
clean :
	rm $(target) $(objects)
