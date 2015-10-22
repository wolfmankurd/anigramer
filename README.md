# anigramer

Simple program to try and find single word anagrams.

## Running

```
$ ./main /usr/share/dict/american-english
```

On GNU/Linux systems often you will find a wordlist in /usr/share/dict.

## Compiling

```
$ gcc -o main main.c anigramer.c -lreadline
```

### Dependencies

Uses readline and getline.

```
# apt-get install libreadline6
```

Or there abouts should get you required dependencies.

### ToDo

Chase any memory leaks.
