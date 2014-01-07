#Simplified Makefile for demo_dmp

all: test

HDRS = MPU6050s/helper_3dmath.h MPU6050s/I2Cdev.h MPU6050s/MPU6050_6Axis_MotionApps20.h MPU6050s/MPU6050.h MPU6050s/mpu6050_dmp.h
CMN_OBJS = MPU6050s/I2Cdev.o MPU6050s/MPU6050.o MPU6050s/mpu6050_dmp.o
DMP_OBJS = main.o

# Set DMP FIFO rate to 20Hz to avoid overflows on 3d demo.  See comments in
# MPU6050_6Axis_MotionApps20.h for details.
# F = 200/(DDMP_FIFO_RATE + 1)

CXXFLAGS = -DDMP_FIFO_RATE=9 -Wall -g -O2

$(CMN_OBJS) $(DMP_OBJS) : $(HDRS)

test: $(CMN_OBJS) $(DMP_OBJS)
	$(CXX) -o $@ $^ -lm

clean:
	rm -f $(CMN_OBJS) $(DMP_OBJS) test *~ MPU6050s/*~
