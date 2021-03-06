#ifndef __ANIGRAMMER_H
#define _ANIGRAMMER_H

enum {
    BROKEN_KEYBOARD_MODE = 1 << 0,
};
int __flags;
void set_flags(int);
int get_flags(void);

struct linked_list_s {
    const char *word;
    struct linked_list_s *next;
};
typedef struct linked_list_s word_list;

struct binary_tree_s {
    const char *key;
    struct binary_tree_s *left;
    struct binary_tree_s *right;
    word_list *next;
};
typedef struct binary_tree_s bin_tree;

#ifdef __EXPERIMENTAL
struct bin_tree_head_s {
  size_t cap;
  size_t num;
  bin_tree *head;
};
typedef struct bin_tree_head_s bin_tree_head;

static bin_tree_head table;
void bin_tree_mmap(size_t);
static bin_tree *bin_tree_malloc(void);
#endif

char *trim (const char *);
static char *hash(const char *);
static int compare_char(const void *, const void *);

static word_list *word_list_new(const char *);
static int word_list_add(word_list *, char *);
void word_list_print(const word_list *);

bin_tree *bin_tree_new(const char *);
int bin_tree_add(bin_tree *, char *);
void bin_tree_print(const bin_tree *);
word_list *bin_tree_find(const bin_tree *, const char *);

#endif
