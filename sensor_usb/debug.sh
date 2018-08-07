cd out
cmake ..
make -j4
mv ./tvpd ..
cd ..
gdb -tui ./tvpd
