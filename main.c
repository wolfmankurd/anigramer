#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <readline/readline.h>

#include "anigramer.h"

#define INITIAL_LINE_SIZE 128

int main(int argc, char **argv){
    if(2 > argc || 3 < argc){
        printf("Usage:\n\t%s word-file [OPTIONS]\n\nOptions:\n\t-b\tBroken keyboard mode.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *fp = NULL;
    fp = fopen(argv[1], "r");
    if(NULL == fp) {
      printf("Failed to open file: \"%s\", is it really a wordlist?\n", argv[1]);
      exit(EXIT_FAILURE);
    }
    int flags = 0;
    if(3 == argc)
        if(0 == strcmp(argv[2], "-b"))
            flags |= BROKEN_KEYBOARD_MODE;
    set_flags(flags);
    #ifdef __EXPERIMENTAL
    bin_tree_mmap(1000);
    #endif
    bin_tree *tree = bin_tree_new("");
    size_t n = INITIAL_LINE_SIZE;
    char *line = (char *) malloc(n);
    while( EOF != getline(&line, &n, fp)) {
        bin_tree_add(tree, trim(line));
    }
    fclose(fp);
    fp = NULL;
    puts("Ctrl+D to quit.");
    for(;;){
        printf("Enter search word: ");
        free(line);
	//        line = strdup("bumnight");
        scanf("%ms", &line);
        if(NULL == line)
            break;
        word_list *list = bin_tree_find(tree, line);
        if(NULL != list)
            word_list_print(list);
        else
            puts("No matches.");
    }
    exit(EXIT_SUCCESS);
}
