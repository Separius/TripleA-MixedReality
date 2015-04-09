TOPDIR = ..

include $(TOPDIR)/Makefile.common

BINS = $(LIBDIR)/libMain.a

cuda : .obj/Shoot-gpu.o .obj/Clear-gpu.o

.obj/Shoot-gpu.o :
	nvcc -c -O2 -g `sdl-config --cflags` -I/usr/local/include/pv-headers/ -I. -I../Geometry -I../AdvanceActions -I../AdvancePlans -I../Connection -I../Properties -I../Utilities -I../WorldData -o .obj/Shoot-gpu.o Shoot-gpu.cu

.obj/Clear-gpu.o :
	nvcc -c -O2 -g `sdl-config --cflags` -I/usr/local/include/pv-headers/ -I. -I../Geometry -I../AdvanceActions -I../AdvancePlans -I../Connection -I../Properties -I../Utilities -I../WorldData -o .obj/Clear-gpu.o Clear-gpu.cu
	

all:: $(BINS)

OBJS += .obj/Shoot-gpu.o
OBJS += .obj/Clear-gpu.o

$(BINS): $(OBJS)
	ar -r $@ $^
