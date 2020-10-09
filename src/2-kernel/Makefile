EXECUTABLE=opencl_practice

CFLAGS=
CFLAGS+=-D CL_TARGET_OPENCL_VERSION=210

LDFLAGS=
LDFLAGS+= -lOpenCL -lm

all:
	gcc -o $(EXECUTABLE) $(CFLAGS) main.c $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)
