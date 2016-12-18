#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void file_handler(char* file_name);
char* word_convert(char* word);

typedef struct node {
  char* word;
  struct node* next_word;
  struct node* children[10];
} node;

void addword(char* word, node root) {
  printf("So far so good\n");
  node curr = root;
  for (int i = 0; i < strlen(word); i++) {
    printf("%c ", word[i]);
  }
  printf("\n");
}


int main(int argc, char** argv) {
  file_handler(argv[1]);
  node root;
  addword("573", root);
  return 0;
}

void file_handler(char* file_name) {
  FILE* file = fopen(file_name, "r");
  char line[500];

  while (fgets(line, sizeof(line), file)) {
    char* t9_word = word_convert(line);
    //printf("%s%s\n\n", line, t9_word);
  }

  fclose(file);
}

char* word_convert(char* word) {
  char* t9_word = malloc(strlen(word) * sizeof(char));
  for (int i = 0; i < strlen(word); i++) {
    if (word[i] <= 'c') {
      t9_word[i] = '2';
    } else if (word[i] <= 'f') {
      t9_word[i] = '3';
    } else if (word[i] <= 'i') {
      t9_word[i] = '4';
    } else if (word[i] <= 'l') {
      t9_word[i] = '5';
    } else if (word[i] <= 'o') {
      t9_word[i] = '6';
    } else if (word[i] <= 's') {
      t9_word[i] = '7';
    } else if (word[i] <= 'v') {
      t9_word[i] = '8';
    } else if (word[i] <= 'z') {
      t9_word[i] = '9';
    } else {
      // non-alphabet char
    }
  }
  t9_word[strlen(word) - 1] = '\0';

  return t9_word;
}

