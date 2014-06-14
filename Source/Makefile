CXX = g++
RM = rm -f
CPPFLAGS = -g

SRCS = $(shell ls *.cpp)
OBJS = $(subst .cpp,.o, $(SRCS))
INCLUDES = -I./matlab_engine
LIBS = -lm -lrt

MAIN = aiplay

all:    $(MAIN)
	@echo Compilation was successful!

$(MAIN): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(INCLUDES) $(LIBS) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

