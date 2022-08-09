#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 256

int check(int len, const char *ans, const char *input);
int check_sub(int len, const char *ans, const char *input, int i);
void reset_buf(int len, char *buf);

int main(int argc, char const *argv[]) {
  printf("*type a word and press return.\n");

  char *datafile = "data.dat";
  FILE *fp;
  int len = 5;  // length of words
  int max_try = 6;  // times allowed to try
  int n = 0;
  char *line = (char*)malloc(sizeof(char) * MAX_LEN);

  fp = fopen(datafile, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open %s\n", datafile);
    return EXIT_FAILURE;
  }

  fscanf(fp, "%d\n", &n);  // get the total number of questions
  char **q = malloc(sizeof(char*)*n);  // set questions data
  for (int i = 0; i < n; ++i) {
    q[i] = malloc(sizeof(char)*len);
    if (fscanf(fp, "%[^\n]%*1[\n]", q[i]) == EOF) {
      n = i;  // update n
    }
  }

  srand((unsigned int)time(NULL));
  int k = rand() % n;  // pick a question number

  for (int i = 0; i < max_try; ++i) {
    reset_buf(MAX_LEN, line);
    int s = scanf("%[^\n]%*1[\n]", line);
    //fflush(stdin);  // lenより長い文字列だとバッファに残ってしまう...なのでクリアする（望ましくはない）
    if (s == 0) {  // if the input is only "return"
      fflush(stdin);
      reset_buf(len, line);
      printf("\e[1A");
      --i;
    }
    else if (strlen(line) != len) {  // if the input length is not equal to len
      printf("\e[1A");
      for (int j = 0; j < strlen(line); ++j) {
        printf(" ");
      }
      printf("\r");
      --i;
      continue;
    }
    int result = check(len, q[k], line);
    if (result) {
      printf("\nnicely done! %d/%d\n", i+1, max_try);
      break;
    }
    else if (i == max_try-1) {
      printf("\n*the answer is \"%s\". try again!\n", q[k]);
    }
  }

  for (int i = 0; i < n; ++i) {
    free(q[i]);
  }
  free(q);
  free(line);
  fclose(fp);
  return 0;
}

int check(int len, const char *ans, const char *input) {
  int match = 0;
  printf("\e[1A");  // set cursur 1 line back
  for (int i = 0; i < len; ++i) {
    if (input[i] == ans[i]) {
      printf("\e[42m%c\e[0m", input[i]);  // green
      match++;
    }
    else if (check_sub(len, ans, input, i)) {
      printf("\e[43m%c\e[0m", input[i]);  // yellow
    }
    else {
      printf("%c", input[i]);
    }
  }
  printf("\n");
  //fflush(stdout);
  return match == len;
}

int check_sub(int len, const char *ans, const char *input, int i) {
  for (int j = 0; j < len; ++j) {
    if (ans[j] == input[i]) {
      return 1;
    }
  }
  return 0;
}

void reset_buf(int len, char *buf) {
  for (int i = 0; i < len; ++i) {
    buf[i] = 0;
  }
}
