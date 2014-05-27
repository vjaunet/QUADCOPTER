#Simplified Makefile for demo_dmp

all: test_ESC

HDRS = servo.h kbhit.h
CMN_OBJS =
DMP_OBJS = main.o servo.o kbhit.o

# Set DMP FIFO rate to 20Hz to avoid overflows on 3d demo.  See comments in
# MPU6050_6Axis_MotionApps20.h for details.
# F = 200/(DDMP_FIFO_RATE + 1)

CXXFLAGS = -DDMP_FIFO_RATE=9 -Wall -g -O2

$(CMN_OBJS) $(DMP_OBJS) : $(HDRS)

test_ESC: $(CMN_OBJS) $(DMP_OBJS)
	$(CXX) -o $@ $^ -lm

clean:
	rm -f $(CMN_OBJS) $(DMP_OBJS) test *~