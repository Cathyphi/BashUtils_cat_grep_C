#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct options {
  int bflag;
  int eflag;
  int vflag;
  int nflag;
  int sflag;
  int tflag;
} flags;  //имя структуры

const struct option long_option[] = {
    {"number-nonblank", 0, 0,
     'b'},  // const char *name - длинное имя опции без предшествующих черточек
    {"number", 0, 0, 'n'},  // int has_arg - имеет ли опция аргумент; 0 - опция
                            // не принимает аргумент
    {"squeeze-blank", 0, 0, 's'}};  // int *flag - если 0, то getopt_long()
                                    // возвращает значение короткой опции

void parser_flags(int argc, char *argv[], flags *args);
void output(char **argv, flags args);

int main(int argc, char **argv) {
  flags args = {0};  //каждому значению присваиваем 0
  parser_flags(argc, argv, &args);
  if (args.bflag && args.nflag) args.nflag = 0;
  while (optind < argc) {  //чтение нескольких файлов
    output(argv, args);
    optind++;
  }
  return 0;
}

void parser_flags(int argc, char *argv[], flags *args) {
  int opt;
  int opindex = 0, past_optind = 1;
  while ((opt = getopt_long(argc, argv, "+benstvTE", long_option, &opindex)) !=
         -1)  // если не нашел флаг, то возвращает -1 - просто cat и файл
    if (optind - past_optind <= 1) {  //не считывает флаги после файла
      past_optind = optind;
      switch (opt) {  //сравнивает значение одной переменной с неск конст
        case 'b':
          args->bflag = 1;
          break;
        case 'e':
          args->eflag = 1;
          args->vflag = 1;
          break;
        case 'n':
          args->nflag = 1;
          break;
        case 's':
          args->sflag = 1;
          break;
        case 't':
          args->tflag = 1;
          args->vflag = 1;
          break;
        case 'T':
          args->tflag = 1;
          break;
        case 'E':
          args->eflag = 1;
          break;
        case 'v':
          args->vflag = 1;
          break;
        default:
          printf("usage: cat [-benstuv] [file ...]\n");
          exit(1);
      }
    }
  optind = past_optind;
}
void output(char **argv, flags args) {
  FILE *fp;
  fp = fopen(argv[optind], "r");
  if (fp == NULL) {
    fprintf(stderr, "No such file or directory\n");
  } else {
    int cur;
    int srt_count =
        1;  //переход на новую строку 1, если нет перехода на новую строку 0
    int n_count = 2;  //счетчик строк, 2 если вначале пустые строки
    int counter = 1;  //количество строк
    while ((cur = fgetc(fp)) != EOF) {
      if (cur != '\n') n_count = 1;
      if ((n_count >= 3) && (args.sflag))
        continue;  // continue переход к следующему условию
      if (srt_count) {
        if (((args.bflag) && (cur != '\n')) || (args.nflag)) {
          printf("%6d\t", counter);
          counter++;
        }
        srt_count = 0;
      }
      if (cur == '\n') {
        srt_count = 1;
        n_count++;
      }
      if ((args.eflag) && cur == '\n')
        printf("$\n");
      else if ((args.tflag) && cur == '\t')
        printf("^I");
      else if (args.vflag) {
        if (cur == 127)
          printf("^\?");
        else if ((cur >= 0) && (cur < 32) && (cur != '\t') && (cur != '\n'))
          printf("^%c", cur + 64);
        else {
          printf("%c", cur);
        }
      } else {
        printf("%c", cur);
      }
    }
  }
  fclose(fp);
}
