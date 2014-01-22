#Simplified Makefile for demo_dmp

all: comA2Rpi

HDRS = net.h # null
CMN_OBJS =   # null
DMP_OBJS = main.o

$(CMN_OBJS) $(DMP_OBJS) : $(HDRS)

comA2Rpi: $(CMN_OBJS) $(DMP_OBJS)
	$(CXX) -o $@ $^ -lm

clean:
	rm -f $(CMN_OBJS) $(DMP_OBJS) comA2Rpi *~
