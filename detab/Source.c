#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#pragma warning(disable: 4996)

#define MAX_NAME_SIZE 100
#define MAX_FORMAT_SIZE 50
#define DEFAULT_TAB_SIZE 2
//error type
typedef enum {
  IN_ERROR_T,
  OUT_ERROR_T,
  OPEN_ERROR_T,
  CREATE_ERROR_T,
  NAME_ERROR_T,
  START_ERROR_T,
  CLOSE_ERROR_T
} error_t;
//language type
typedef enum {
  RU,
  EN
} language_t;
/*function to get a path to a file from the string - 
deleting a name of the file*/
void _getPath(char* path) {
  register int counter = 0;
  char character = '\0';

  while (*path != '\0') {
    counter++;
    path++;
  }
  while (*path != '\\' && counter >= 0) {
    *path-- = character;
    counter--;
  }
}
//function to get a format of a file from the string
void _getFormat(const char* path, char* format, int lengthFormat) {
  register int formatCounter = 0;

  while (*path != '.' && *path != '\0') {
    path++;
  }
  while (*path != '\0' && formatCounter < lengthFormat - 1) {
    formatCounter++;
    *format++ = *path++;
  }
  *format = '\0';
}
//function to get a name of a file from the string
void _getName(const char* path, char* name, int lengthName) {
  register int counter = 0, nameCounter = 0;

  while (*path != '.' && *path != '\0') {
    path++;
    counter++;
  }
  while (*path != '\\' && counter > 0) {
    counter--;
    path--;
  }
  if(*path == '\\')
    path++;
  while (*path != '.' && *path != '\0' && nameCounter < lengthName - 1) {
    *name++ = *path++;
    nameCounter++; 
  }
  *name = '\0';
}
//function to display a message of error
void _error(error_t err, const char* name, language_t language) {
  switch (err) {
    case IN_ERROR_T:
      if (language == RU) {
        printf_s("\nФайл %s:\nОшибка при вводе.\n", name);
      }
      else {
        printf_s("\nFile %s:\nInput error.\n", name);
      }
      break;
    case OUT_ERROR_T:
      if (language == RU) {
        printf_s("\nФайл %s:\nОшибка при выводе.\n", name);
      }
      else {
        printf_s("\nFile %s:\nOutput error.\n", name);
      }
      break;
    case OPEN_ERROR_T:
      if (language == RU) {
        printf_s("\nНевозможно открыть %s.\n", name);
      }
      else {
        printf_s("\nUnable to open %s.\n", name);
      }
      break;
    case CREATE_ERROR_T:
      if (language == RU) {
        printf_s("\nНевозможно создать %s.\n", name);
      }
      else {
        printf_s("\nUnable to create %s.\n", name);
      }
      break;
    case NAME_ERROR_T:
      if (language == RU) {
        printf_s("\nФайл %s:\nОшибка работы с файлом: возможная причина - очень длинный путь к файлу и/или имя файла.\n", name);
        puts("Максимальная длина(путь + имя) - 250 символов.");
      }
      else {
        printf_s("\nFile %s:\nError working with a file: a possible reason - a very long file path and/or filename.\n", name);
        puts("Maximum length (path + name) - 250 characters.");
      }
      break;
    case CLOSE_ERROR_T:
      if (language == RU) {
        printf_s("\nОшибка при закрытии файла %s.\n", name);
      }
      else {
        printf_s("\nFile %s:\nFile closing error.\n", name);
      }
      break;
    default:
      if (language == RU) {
        puts("\nСинтаксис: <путь к detab.exe> <входной файл> ... <входной файл>");
      }
      else {
        puts("\nSyntax: <path to detab.exe> <input file> ... <input file>");
      }
      break;
  }
}
//function to work with file
void _run(FILE* in, FILE* out, const char* name, language_t language) {
  char character;
  int counter;
  int tabSize = DEFAULT_TAB_SIZE;
  char space = ' ';
  int isErr = 0;
  //select equivalent to tab in spaces
  if (language == RU) {
    printf_s("\nФайл %s:\nТабуляция эквивалентна <введите число> пробелам: ", name);
  }
  else {
    printf_s("\nFile %s:\nTab is equivalent to <enter number> spaces: ", name);
  }
  scanf_s("%d", &tabSize);
  scanf_s("%*[^\n]");
  //get character by character until the file ends
  for (;;) {
    //handle possible error
    if ((character = (char)fgetc(in)) == EOF) {
      if (ferror(in)) {
        _error(IN_ERROR_T, name, language);
        break;
      }
      else if (feof(in)) {
        break;
      }
      else {
        if (fputc(character, out) == EOF) {
          if (ferror(out)) {
            _error(OUT_ERROR_T, name, language);
            break;
          }
        }
      }
    }
    //change tab to spaces
    else if (character == '\t') {
      for (counter = 0; counter < tabSize; counter++) {
        if (fputc(space, out) == EOF) {
          if (ferror(out)) {
            _error(OUT_ERROR_T, name, language);
            isErr = 1;
            break;
          }
        }
      }
      if (isErr) {
        break;
      }
    }
    //put a character in the output file
    else {
      if (fputc(character, out) == EOF) {
        if (ferror(out)) {
          _error(OUT_ERROR_T, name, language);
          break;
        }
      }
    }
  }
}
//main function
int main(int argc, char* argv[]) {
  FILE* in, * out;
  int counter;
  char defaultFormat[] = ".txt", defaultName[] = "file";
  char prefix[] = "detab_";
  language_t language = EN;
  char* locale;
  //select language
  locale = setlocale(LC_ALL, "");
  if (locale) {
    if (!strcmp(locale, "Russian_Russia.1251")) {
      language = RU;
    }
  }
  //check launch conditions
  if (argc < 2) {
    _error(START_ERROR_T, "", language);
    exit(1);
  }
  //repeat until files end
  for (counter = 1; counter < argc; counter++) {
    char path[_MAX_PATH];
    char format[MAX_FORMAT_SIZE], name[MAX_NAME_SIZE];
    /*get a path to the file
    to create a new file for output*/
    if (strcpy_s(path, _MAX_PATH, argv[counter])) {
      _error(NAME_ERROR_T, argv[counter], language);
      continue;
    }
    _getPath(path);
    //add special prefix
    if (strcat_s(path, _MAX_PATH, prefix)) {
      _error(NAME_ERROR_T, argv[counter], language);
      continue;
    }
    //add name of the file
    _getName(argv[counter], name, MAX_NAME_SIZE);
    if (*name) {
      if (strcat_s(path, _MAX_PATH, name)) {
        _error(NAME_ERROR_T, argv[counter], language);
        continue;
      }
    }
    else {
      if (strcat_s(path, _MAX_PATH, defaultName)) {
        _error(NAME_ERROR_T, argv[counter], language);
        continue;
      }
    }
    //add format of the file
    _getFormat(argv[counter], format, MAX_FORMAT_SIZE);
    if (*format) {
      if (strcat_s(path, _MAX_PATH, format)) {
        _error(NAME_ERROR_T, argv[counter], language);
        continue;
      }
    }
    else {
      if (strcat_s(path, _MAX_PATH, defaultFormat)) {
        _error(NAME_ERROR_T, argv[counter], language);
        continue;
      }
    }
    //open files
    if ((in = fopen(argv[counter], "rb")) == NULL) {
      _error(OPEN_ERROR_T, argv[counter], language);
      continue;
    }
    if ((out = fopen(path, "wb")) == NULL) {
      _error(CREATE_ERROR_T, path, language);
      fclose(in);
      continue;
    }
    //start working with files
    _run(in, out, argv[counter], language);
    //close files
    if (fclose(in))
      _error(CLOSE_ERROR_T, argv[counter], language);
    if (fclose(out))
      _error(CLOSE_ERROR_T, path, language);
  }

  return 0;
}