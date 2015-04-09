#! /bin/sh

cd ./AdvanceActions/.obj
rm *-gpu.o
cd ../
make cuda
echo "Cuda File Compiled !"
cd ../
make
echo "Binary File Created !"
