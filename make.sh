mkdir -p bin
cd src
gcc "example.c" -o "../bin/example.exe" -lglew32 -lglu32 -lglut32 -lopengl32 -march=x86-64
cd ..