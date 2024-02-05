# Καθάρισμα, αν υπάρχουν binaries
rm -rf build

# Δημιουργία build dirs
mkdir -p build/1 build/2 build/3 build/4

# 1
echo "Building 1"
gcc "1/3122289_ask11.c" -o build/1/ask11
gcc "1/3122289_ask12.c" -o build/1/ask12

# 2
echo "Building 2"
gcc "2/3122289_ask21.c" -o "build/2/ask21"
gcc "2/3122289_ask22.c" -o "build/2/ask22"
gcc "2/3122289_ask23.c" -o "build/2/ask23"
gcc "2/3122289_FileProp.c" -o "fileprop"

# 3
echo "Building 3"
gcc "3/Gkekas_ask31a.c" -pthread -o "build/3/ask31"
gcc "3/Gkekas_ask31b.c" -pthread -o "build/3/ask31b"
gcc "3/Gkekas_ask32.c" -pthread -o "build/3/ask32"

# 4
echo "Building 4"
gcc "4/Gkekas_ask41.c" -pthread -o "build/4/ask41"
gcc "4/Gkekas_ask42.c" -pthread -o "build/4/ask42"