EXECUTABLE=opencl_practice

CFLAGS=
CFLAGS+=-D CL_TARGET_OPENCL_VERSION=210

LDFLAGS=
LDFLAGS+= -lOpenCL

SRC=
SRC+=main.c

$(EXECUTABLE): $(SRC)
	gcc -o $(EXECUTABLE) $(CFLAGS) $^ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)
