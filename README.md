# anigramer v1.0.1

Simple program to try and find single word anagrams. Making use of binary heap trees and linked lists, this program uses libreadline and GNU getline.

GPLv2

## Project files
```
ali@ali-K53E:~/Code/C/anigramer$ tree .
.
├── anigramer
├── anigramer.c
├── anigramer.h
├── anigramer-upx
├── LICENSE
├── main.c
├── Makefile
└── README.md

0 directories, 8 files

```
## Running

```
$ ./anigramer /usr/share/dict/american-english
```

On GNU/Linux systems often you will find a wordlist in /usr/share/dict.

You may pass options to alter it's operation, currently all that is supported is -b which will make the program ignore how many copies of each letter you pass it "broken keyboard mode". Thus elope and people will also match.

```
ali@ali-K53E:~/Code/C/anigramer$ ./anigramer 
Usage:
	./anigramer word-file [OPTIONS]

Options:
	-b	Broken keyboard mode.
```

### Sample output

```
ali@ali-K53E:~/Code/C/anigramer$ ./anigramer /usr/share/dict/american-english 
Ctrl+D to quit.
Enter search word: anigramer
margarine
```

## Compiling

```
$ make
```

```
$ gcc -o anigramer main.c anigramer.c -O3 -lreadline
```

### Dependencies

Uses readline and getline.

```
# apt-get install libreadline6
```

Or there abouts should get you required dependencies on GNU/Debian systems.

UPX is an optional dependencies should you want smaller binaries (typically 5-6kB v 11kB), upx may be installed from the repos on most distros.



### Installing

Copy binary files anigramer and anigramer-upx (if present) into a directory which is part of your $PATH. I used ~/.local/bin for system wide installation you may require root priviledges.

### ToDo

Chase any memory leaks.
