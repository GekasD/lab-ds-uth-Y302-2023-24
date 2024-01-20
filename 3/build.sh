# Compile κάνοντας link την βιβλιοθήκη "pthread.h"
echo "Compiling ask31a..."
gcc Gkekas_ask31a.c -pthread -o maila
echo "Compiling ask31b..."
gcc Gkekas_ask31b.c -pthread -o mailb
echo "Compiling ask32..."
gcc Gkekas_ask32.c -pthread -o matrix

# Μήνυμα
echo "Finished compiling, binary names:"
echo "maila (Gkekas_ask31a.c)"
echo "mailb (Gkekas_ask31b.c)"
echo "matrix (Gkekas_ask32.c)"