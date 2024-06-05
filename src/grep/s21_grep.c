#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct options {
  int eflag;
  int iflag;
  int vflag;
  int cflag;
  int lflag;
  int nflag;
  int hflag;
  int sflag;
  int e_case;
  int multifiles;
  char template[4086];  //размер шаблона
} flags;                //имя структуры

int parser_flags(int argc, char *argv[], flags *args);
void output(char **argv, flags args);
void o_case(char *line, regex_t regex);

int main(int argc, char **argv) {
  flags args = {0};  //каждому значению присваиваем 0

  if (argc > 2) {
    if (parser_flags(argc, argv, &args) == 0) {
      if (args.e_case == 0) {
        sprintf(args.template, "%s", argv[optind]);
        optind++;
      }
      if (argc - optind > 1) {  //если файлов больше 1
        args.multifiles = 1;
      }

      while (optind < argc) {
        output(argv, args);
        optind++;
      }
    }
  }
  // printf("%s", argv[optind]);
  return 0;
}

int parser_flags(int argc, char *argv[], flags *args) {
  int opt;
  int opindex = 0;
  int flags_res = 0;
  while ((opt = getopt_long(argc, argv, "e:ivclnhs", 0, &opindex)) != -1)
    switch (opt) {
      case 'e':
        args->eflag = 1;
        if (args->e_case > 0) {
          strcat(args->template, "|");
        }
        strcat(args->template, optarg);
        args->e_case++;
        break;
      case 'i':
        args->iflag = 1;
        break;
      case 'v':
        args->vflag = 1;
        break;
      case 'c':
        args->cflag = 1;
        break;
      case 'l':
        args->lflag = 1;
        break;
      case 'n':
        args->nflag = 1;
        break;
      case 'h':
        args->hflag = 1;
        break;
      case 's':
        args->sflag = 1;
        break;
      default:
        // printf("grep: invalid option\n");
        exit(1);
    }
  return flags_res;
}

void output(char **argv, flags args) {
  FILE *fp;
  char *line = NULL;  // динамический массив для харания строки файла
  size_t len;         // размер динамического массива
  int number = 0;     // номера строк
  int count = 0;      // количество строк
  int i;
  int cflags = 0;
  regex_t regex;  //хранениe скомпилированного регулярного выражения
  fp = fopen(argv[optind], "r");
  if (fp) {
    cflags |= REG_EXTENDED;
    if (args.iflag) cflags |= REG_ICASE;
    regcomp(&regex, args.template, cflags);
    while (getline(&line, &len, fp) != -1) {
      number++;
      int success = regexec(&regex, line, 0, NULL, 0);
      if (((success == 0) && (args.vflag == 0)) ||  //инвертирует
          ((success == 1) && (args.vflag == 1))) {
        count++;
        i = 0;
        if (args.cflag == 0) {
          if ((args.lflag) &&
              (success == 0)) {  //Выводит только совпадающие файлы
            printf("%s\n", argv[optind]);
            break;
          }
          if ((args.multifiles == 1) && (args.hflag == 0))
            printf("%s:", argv[optind]);  //не предваряя именами файлов
          if ((args.nflag) &&
              (success == 0)) {  //Предваряет каждую строку номером
            printf("%d:", number);
          }
          // printf("%s", line);
          while (line[i]) {  //для перехода новую строку в конце
            if (line[i] != '\n') printf("%c", line[i]);
            i++;
          }
          printf("\n");
        }
      }
    }
    free(line);
    regfree(&regex);
  } else {
    if (args.sflag != 1) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
    }
  }
  if (args.cflag) {  // вывести количество совпадающих строк в файле
    printf("%d\n", count);
  }
  fclose(fp);
}
