CC = g++
LD = g++

CFLAGS = -Wall -std=c++11
LFLAGS = -Wall -std=c++11

CODE_OBJS = main.o

CODE = run.exe

OBJS = $(CODE_OBJS)
EXECS = $(CODE)

#Implicit Rules
.cpp.o:
	$(CC) $(CFLAGS) -c $<

all: $(EXECS)

compile: $(CODE_OBJS)
	$(LD) $(CODE_OBJS) -o $(CODE) $(LFLAGS)
	make -s clean-objs
clean:
	rm -f $(OBJS) $(EXECS)

clean-execs:
	rm -f $(EXECS)

clean-objs:
	rm -f $(OBJS)