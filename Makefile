.SUFFIXES: .o .c

CC = /usr/bin/clang
#CPPFLAGS = -I/opt/local/include -I${HOME}/src/libeno -DSWAPENDIAN -DVERBOSE
CPPFLAGS = -I/opt/local/include -I${HOME}/src/libeno -DSWAPENDIAN
CFLAGS = -O2
CXXFLAGS = -O2
LDFLAGS = -L/opt/local/lib -L${HOME}/src/libeno -lgsl -leno -framework Accelerate
TARGET = tccentre
OBJS = io.o array.o minimize.o

all : headers $(TARGET)

$(TARGET) : $(TARGET).o $(OBJS)

clean :
	rm -f $(TARGET) *.o *.h

headers :
	makeheaders *.[ch]
