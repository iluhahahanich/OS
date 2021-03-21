g++ -c -fPIC number.cpp -o number.o
ar rc libnumber.a number.o

g++ -fPIC -shared vector.cpp libnumber.a -o libvector.so
export LD_LIBRARY_PATH=.

g++ main.cpp -L libnumber.a libvector.so -o main
./main

#rm number.o libnumber.a libvector.so main

