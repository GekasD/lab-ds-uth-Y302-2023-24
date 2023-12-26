#!/bin/bash

# Καθάρισμα, αν υπάρχουν binaries
rm -f "ask21" "fileprop" "ask22" "ask23"

# Compile
gcc "3122289_ask21.c" -o "ask21"
gcc "3122289_FileProp.c" -o "fileprop"
gcc "3122289_ask22.c" -o "ask22"
gcc "3122289_ask23.c" -o "ask23"