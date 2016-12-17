#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINE_LEN 500
#define PATTERN_LEN 100

void case_handler(bool ignore_case, char* input_word);
void file_handler(bool ignore_case, bool line_nums, char* word, char* file_name);

int main(int argc, char**argv) {
  bool ignore_case = false;
  bool line_nums = false;
  bool word_set = false;
  char* word;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0) {
      ignore_case = true;
    } else if (strcmp(argv[i], "-n") == 0) {
      line_nums = true;
    } else if (!word_set) {
      word_set = true;
      case_handler(ignore_case, argv[i]);
      word = argv[i];
    } else {
      file_handler(ignore_case, line_nums, word, argv[i]);
    }
  }
  return 0;
}

void file_handler(bool ignore_case, bool line_nums, char* word, char* file_name) {
  FILE* file = fopen(file_name, "r");
  char line[LINE_LEN];
  int line_num = 0;

  while (fgets(line, sizeof(line), file)) {
    bool found = false;
    line_num++;
    if (ignore_case) {
      char temp_line[LINE_LEN];
      strncpy(temp_line, line, LINE_LEN);
      case_handler(ignore_case, temp_line);
      found = (strstr(temp_line, word) != NULL);
    } else {
      found = (strstr(line, word) != NULL);
    }
    if (found) {
      printf("%s ", file_name);
      if (line_nums) {
        printf("%i ", line_num);
      }
      printf("%s", line);
    }
  }

  fclose(file);
}

void case_handler(bool ignore_case, char* input_string) {
  if (ignore_case) {
    int i = 0;
    char c;

    while (input_string[i]) {
      input_string[i] = tolower(input_string[i]);
      i++;
    }
  }
}

