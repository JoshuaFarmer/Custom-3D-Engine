mkdir -p bin
gcc "src/example.c" -o "bin/example.exe" -lglew32 -lglu32 -lglut32 -lopengl32 -march=x86-64 -Wall -O3
cd bin
./example
cd ..