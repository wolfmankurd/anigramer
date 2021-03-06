#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "anigramer.h"

// Uncomment line below to enable debug output i.e. verbose.
// This is typically very annoying and very slow (stdout can block)
// #define __DEBUG

void set_flags(int modes) {
    __flags = modes;
}
int get_flags(void) {
    return __flags;
}
char *trim (const char *in_string) {
    char *string = strdup(in_string);
    int i;
    for (i = strlen(string) - 1; i >= 0; --i) {
        if('\n' == string[i] || '\r' == string[i])
            string[i] = 0;
    }
    return string;
}

static int compare_char (const void *a, const void *b) {
  const char *da = (const char *) a;
  const char *db = (const char *) b;

  return (*da > *db) - (*da < *db);
}

static char *hash(const char *word){
    char *hword = strdup(word);
    int i;
    int j = 0;
    for(i = strlen(hword) - 1; i >=0; --i){
        if(hword[i] >= 'A' && hword[i] <= 'Z') {
            hword[i] -= 'A';
            hword[i] += 'a';
        }
        // Broken keyboard mode. remove dups.
        if(get_flags() & BROKEN_KEYBOARD_MODE){
            int t;
            for(t = 0; t<i; t++){
                if(hword[t] == hword[i]){
                    hword[i] = '~';
                    break;
                }
            }
        }
        if(hword[i] < 'a' || hword[i] > 'z') {
            hword[i] = '~';
            j++;
        }
    }
    qsort(hword, strlen(word), sizeof(char), compare_char);
    int len = strlen(hword) - j;
    hword[len] = 0;
    char *hashed = (char *) malloc(len + 1);
    strncpy(hashed, hword, len + 1);
    free(hword);
    #ifdef __DEBUG
    printf("hash(%s)=\"%s\"\n", word, hashed);
    #endif
    return hashed;
}

static word_list *word_list_new(const char *word) {
    #ifdef __DEBUG
    printf("word_list_new(word: %s)\n", word);
    #endif
    word_list *list = (word_list *) malloc(sizeof(word_list));
    assert(NULL != list);
    list->word = word;
    list->next = NULL;
    return list;
}

static int word_list_add(word_list *self, char *word) {
    #ifdef __DEBUG
    printf("word_list_add(self: %p, word: %s)\n", self, word);
    #endif
    if(0 == strcmp(word, self->word)){
        free(word);
        return 0;
    }
    while (NULL != self->next) {
        if(0 == strcmp(word, self->word)){
            free(word);
            return 0;
        }
        self = self->next;
    }
    word_list *new = word_list_new(word);
    self->next = new;
    return 1;
}

void word_list_print(const word_list *self) {
    while(NULL != self){
        #ifdef __DEBUG
        printf("word_list_print(self: %p, word: %s)\n", self, self->word);
        #else
        puts(self->word);
        #endif
        self = self->next;
    }
    //    puts("");
}

#ifdef __EXPERIMENTAL
void bin_tree_mmap(size_t n){
  table.cap = n;
  table.num = 0;
  table.head = (bin_tree *)malloc(sizeof(bin_tree)*n+1);
  assert(NULL != table.head);
}

static bin_tree *bin_tree_malloc(void) {
  if(table.cap > table.num + 1) {
    ++table.num;
    return (bin_tree *)table.head+(sizeof(bin_tree) * table.num);
  }else{
    bin_tree *new = (bin_tree *)malloc(sizeof(bin_tree));
    assert(NULL != new);
    return new;
  }
}
#endif

bin_tree *bin_tree_new(const char *word) {
    #ifdef __DEBUG
    printf("bin_tree_new(word: %s)\n", word);
    #endif
    char *key = hash(word);
    #ifdef __EXPERIMENTAL
    bin_tree *tree = bin_tree_malloc();
    #else
    bin_tree *tree = (bin_tree *) malloc(sizeof(bin_tree));
    #endif
    assert(NULL != tree);
    tree->key = key;
    tree->left = NULL;
    tree->right = NULL;
    tree->next = word_list_new(word);
    return tree;
}

int bin_tree_add(bin_tree *self, char *word) {
    #ifdef __DEBUG
    printf("bin_tree_add(self: %p, word: %s)\n", self, word);
    #endif
    char *hword = hash(word);
    while(NULL != self) {
        int rv = strcmp(hword, self->key);
        if (rv < 0){
            if(NULL == self->left){
                bin_tree *node = bin_tree_new(word);
                self->left = node;
                free(hword);
                return 1;
            }
            self = self->left;
            continue;
        }
        if (rv > 0){
            if(NULL == self->right){
                bin_tree *node = bin_tree_new(word);
                self->right = node;
                free(hword);
                return 1;
            }
            self = self->right;
            continue;
        }
        free(hword);
        word_list_add(self->next, word);
        return 0;
    }
}

word_list *bin_tree_find(const bin_tree *self, const char *word) {
    #ifdef __DEBUG
    printf("bin_tree_find(self: %p, word: %s)\n", self, word);
    #endif
    char *hword = hash(word);
    while(NULL != self) {
      /* bin_tree *prefetch_left, *prefetch_right; */
      /* prefetch_left = self->left; */
      /* prefetch_right = self->right; */
      int rv = strcmp(hword, self->key);
        if (rv < 0){
            if(NULL == self->left){
                free(hword);
                return NULL;
            }
            self = self->left;
            continue;
        }
        if (rv > 0){
            if(NULL == self->right){
                free(hword);
                return NULL;
            }
            self = self->right;
            continue;
        }
        free(hword);
        return self->next;
    }
}

void bin_tree_print(const bin_tree *self) {
    printf("bin_tree_node(self: %p, key: %s, left: %p, right: %p, next: %p)\n", \
    self, self->key, self->left, self->right, self->next);
}

