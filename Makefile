CC=gcc
CFLAGS=-Wall
CXX=g++
CXXFLAGS=$(CFLAGS)

LD=$(CC)
LDFLAGS=-Wall

OBJS-lib = sched.o
OBJS-exec = test_sched.o

all: clean test_lib.exec libsched.so
	@echo "Build"

test_lib.exec: $(OBJS-exec)
	@echo "Build test"
	@$(LD) -o test_lib.exec $(OBJS-exec) -ldl

libsched.so: $(OBJS-lib)
	@echo "Build lib"
	@$(LD) -o libsched.so -shared -fPIC $(OBJS-lib)

%.o:%.c
	@echo "(CC) $@"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o:%.cxx
	@echo "(CXX) $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Clean project"
	@rm -vf $(OBJS-lib)   $(OBJS-exec) test_lib.exec libsched.so
