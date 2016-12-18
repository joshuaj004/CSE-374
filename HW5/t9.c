#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct node {
	char* word;
	struct node* digits[10];
};

struct node* createroot();

struct node* addword(char*, struct node*);

struct node* findword(struct node*, char*);

void printtree(struct node*);

void freeall(struct node*);

static char* readword(FILE*);
static struct node* lookup(char*, struct node*, struct node*);

// Given the root of a t9 trie, this reads user input and searches the trie for
// the given key sequence. If a matching word is found, the word is printed,
// otherwise a message is printed describing the reason a word was not found.
// Interactions is ended either by the user typing "exit" or pressing ctrl-D.
void interactions(struct node* root) {
        printf("Enter \"exit\" to quit.\n");
	printf("Enter Key Sequence (or \"#\" for next word):\n");
	char* digits = readword(stdin);
	struct node* prev = NULL;
	// Runs until the user types "exit" or EOF is hit (digits == NULL)
        while (digits != NULL && strcmp(digits, "exit") != 0) {
		prev = lookup(digits, prev, root);
		digits = readword(stdin);
	}
	if (digits != NULL) {
		free(digits);
	}
}
// Given a file name, this method creates a t9 trie of the words in the file.
// Returns the root of the trie. Assumes all characters in the file are letters.
// If file doesn't exist or isn't given, prints an error and exits.
struct node* treefromfile(char* file) {
        if (file == NULL) {
		fprintf(stderr, "No file given...\n");
		exit(1);
	}
	FILE* input = fopen(file, "r");
        if (input == NULL) {
                fprintf(stderr, "%s does not exist\n", file);
                exit(2);
        }
        struct node* root = createroot();
        // Reads and adds words until EOF is hit (word == NULL)
        // Ignores blank lines
	char* word = readword(input);
        while (word != NULL) {
		if (strcmp(word, "\n") != 0) {
			root = addword(word, root);
		}
		free(word);
		word = readword(input);
        }
	fclose(input);
        return root;
}

// Lookup method for interactions method. Given a key sequence, the root of the
// t9 trie, and the node found in the previous lookup call, this method will 
// search the trie for the sequence. If found, prints it. Otherwise, prints
// why the word was not found. Returns the trie node the word was found in, if
// found, or a NULL node if not found.
static struct node* lookup(char* digits, struct node* node, struct node* root) {
	if (strcmp(digits, "\n") != 0) { // Ignore blank lines
                if (strcmp(digits, "#") == 0) { // If a # is given, look in node
                        if (node == NULL || node->digits[0] == NULL) {
                                printf("\tThere are no more T9onyms\n");
                        } else {
                                node = node->digits[0];
                                printf("\t\t'%s'\n", node->word);
                                printf("Enter Key Sequence (or \"#\" for next word):\n");
                        }
                } else { // If no # is given, look from the root
                        node = findword(root, digits);
                        if (node == NULL) {
                                if (digits[strlen(digits) - 1] == '#') {
                                        printf("\tThere are no more T9onyms\n");
                                } else {
                                        printf("\tNot found in current dictionary.\n");
                                }
                        } else {
                                printf("\t\t'%s'\n", node->word);
                                printf("Enter Key Sequence (or \"#\" for next word):\n");
                        }
                }
	}
        free(digits);
        return node;
}



static char* readword(FILE* input) {
        int size = 1;
	int ch;
	char* word = (char*) malloc(sizeof(char));
        while ((ch = fgetc(input)) != '\0' && ch != '\n' && ch != EOF) {
                size++;
                word = (char*) realloc(word, sizeof(char)*size);
		word[size-2] = (char) ch;
        }
	if (ch != EOF && size > 1) {
		word[size-1] = '\0';
		return word;
        } else if (ch == '\n' && size == 1) {
		word = (char*) realloc(word, sizeof(char)*2);
		word[0] = '\n';
		word[1] = '\0';
		return word;
	} else { 
		free(word);
		return NULL;
	}
}

int T9_DICTIONARY[] = {2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6,
                         7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9};

static int charnum(char);
static struct node* checknode(struct node*);
static void printnode(struct node*, char*, int);
static void freethis(struct node*);

// Creates a new root of a trie. Returns this root.
struct node* createroot() {
	struct node* root = (struct node*) malloc(sizeof(struct node));
        root->word = NULL;
	return root;
}

// Given a word and the root of a trie, adds the word to the trie. Overlapping
// words are added as a linked list from the conflicting word. Returns the
// newly formed root.
struct node* addword(char* word, struct node* root) {	
	struct node* curr = root;
	for (int i = 0; i < strlen(word); i++) {
               	int digit = charnum(word[i]);
		curr->digits[digit] = checknode(curr->digits[digit]);
               	curr = curr->digits[digit];
       	}
	// This goes deeper if there is a conflict
	while (curr->word != NULL) {
		curr->digits[0] = checknode(curr->digits[0]);
		curr = curr->digits[0];
	}	
	char* temp = (char*) malloc(sizeof(word));
	temp = strcpy(temp, word);
      	curr->word = temp;
	return root;
}

// Given a node in the trie and a key sequence, searches for the word in the
// trie. Returns the node the word is in if found. Returns NULL otherwise.
struct node* findword(struct node* node, char* digits) {	
	// Traverses a character at a time
	for (int i = 0; i < strlen(digits); i++) {
		if (node != NULL) {
			int digit = digits[i] - '0';
			if (digits[i] == '#') { // For overlaps
				node = node->digits[0];
			} else if (digit >= 0 && digit <= 9) {
				node = node->digits[digit];
			}
		} else {
			return NULL;
		}
	}
	if (node == NULL) {
		return NULL;
	} else if (node->word == NULL) {
		return NULL;
	} else {
		return node;
	}	
}

// Prints the given tree.
void printtree(struct node* root) {
	printf("Root...\n");
	char digits[50];
	for (int i = 2; i < 10; i++) {
		char digit[2];
		sprintf(digit, "%d", i);
		strcpy(digits, digit);
		printnode(root->digits[i], digits, 1);
		digits[0] = '\0';
	}
}

// Frees the given tree recursively
void freeall(struct node* root) {
	freethis(root);
}

static void printnode(struct node* node, char* digits, int level) {
	if (node != NULL) {
		if (node->word != NULL) {
			printf("%s: %s\n", digits, node->word);
                        strcat(digits, "#");
                        printnode(node->digits[0], digits, level+1);
                        digits[level] = '\0';
		}
		for (int i = 2; i < 10; i++) {
			char digit[2];
			sprintf(digit, "%d", i);
			strcat(digits, digit);
			printnode(node->digits[i], digits, level+1);
			digits[level] = '\0';
		}
	}
}

static void freethis(struct node* curr) {
        if (curr != NULL) {
                for (int i = 0; i < 10; i++) {
                        freethis(curr->digits[i]);
                }
		if (curr->word != NULL) {
			free(curr->word);
        	}
	        free(curr);
        }
}

// Converts a letter to a key
static int charnum(char ch) {
        return T9_DICTIONARY[tolower(ch) - 'a'];
}

// Checks a node to make sure it's not null
static struct node* checknode(struct node* node) {
        if (node == NULL) {
        	node = (struct node*) malloc(sizeof(struct node));
        	node->word = NULL;
        }
        return node;
}

int main(int argc, char** argv) {
	struct node* root = treefromfile(argv[1]);
	interactions(root);
	freeall(root);
	return 0;	
}

/*
struct node* file_handler(char* file_name);
char* word_convert(char* word);
char* line_to_word(char* word);

struct node {
  char* word;                 // Store the actual word
  struct node* digits[10];    // Store the children of the node. Technically, only 8 are needed, aside from the '#' for conflicts.
};

struct node* createroot() {
  // Create a root.
  struct node* root = (struct node*) malloc(sizeof(struct node));
  root->word = NULL;
  return root;
}

struct node* checknode(struct node* node) {
  if (node == NULL) {
    node = (struct node*) malloc(sizeof(struct node));
    node->word = NULL;
  }
  return node;
}

struct node* addword(char* word, char* real_word, struct node* root) {
  // Set current to root so we always start at the top.
  struct node* curr = root;
  // Iterate through the word.
  for (int i = 0; i < strlen(word); i++) {
    // 
    //int digit = charnum(word[i]);
    // Saved so we don't have to type word[i] every time.
    int digit = word[i];
    // Makes sure a node isn't null.
    curr->digits[digit] = checknode(curr->digits[digit]);
    // Advances current to its new position.
    curr = curr->digits[digit];
  }
  // Handles any conflicts.
  while (curr->word != NULL) {
    curr->digits[0] = checknode(curr->digits[0]);
    curr = curr->digits[0];
  }
  char* temp = (char*) malloc(sizeof(word));
  temp = strcpy(temp, real_word);
  curr->word = temp;
  return root;
}

struct node* findword(struct node* node, char* digits) {
  for (int i = 0; i < strlen(digits); i++) {
    if (node != NULL) {
      // may need - '0' added to the end of this line???
      int digit = digits[i];
      if (digits[i] == '#') {
	node = node->digits[0];
      } else if (digit >= 0 && digit <= 9) {
	node = node->digits[digit];
      }
    } else {
      return NULL;
    }
  }
  if (node == NULL) {
    return NULL;
  } else if (node->word == NULL) {
    return NULL;
  } else {
    return node;
  }
}

void printnode(struct node* node, char* digits, int level) {
  if (node != NULL) {
    if (node->word != NULL) {
      printf("%s: %s\n", digits, node->word);
      strcat(digits, "#");
      printnode(node->digits[0], digits, level+1);
      digits[level] = '\0';
    }
    for (int i = 2; i < 10; i++) {
      char digit[2];
      sprintf(digit, "%d", i);
      strcat(digits, digit);
      printnode(node->digits[i], digits, level+1);
      digits[level] = '\0';
    }
  }
}

void printtree(struct node* root) {
  printf("Root...\n");
  char digits[50];
  for (int i = 2; i < 10; i++) {
    char digit[2];
    sprintf(digit, "%d", i);
    strcpy(digits, digit);
    printnode(root->digits[i], digits, 1);
    digits[0] = '\0';
  }
}


int main(int argc, char** argv) {
  struct node* root = file_handler(argv[1]);
  //addword("573", root);
  return 0;
}

struct node* file_handler(char* file_name) {
  FILE* file = fopen(file_name, "r");
  char line[500];

  struct node* root = createroot();
  while (fgets(line, sizeof(line), file)) {
    char* t9_word = word_convert(line);
    char* new_word = line_to_word(line);
    root = addword(t9_word, new_word, root);
    //printf("%s%s\n\n", line, t9_word);
  }

  fclose(file);
}

char* line_to_word(char* word) {
  char* new_word = malloc(strlen(word) * sizeof(char));
  for (int i = 0; i < strlen(word); i++) {
    new_word[i] = word[i];
  }
  new_word[strlen(word) - 1] = '\0';

  return new_word;
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

*/
