#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

#pragma warning(disable: 4996)

#define LIST 100
#define NAME 35
//address type
typedef struct {
  char name[NAME];
  char street[NAME];
  char city[NAME];
  char state[NAME];
  unsigned long int zip;
} addr_t;
//menu point type
typedef enum {
  ENTER_MENU_T = 1,
  DEL_MENU_T,
  DISPLAY_MENU_T,
  SAVE_MENU_T,
  LOAD_MENU_T,
  EXIT_MENU_T
} menu_t;
//error type
typedef enum {
  INPUT_ERROR_T,
  LOAD_ERROR_T,
  SAVE_ERROR_T,
  OPEN_ERROR_T,
  CLOSE_ERROR_T,
  CREATE_ERROR_T,
  PATH_ERROR_T
} error_t;
//language type
typedef enum {
  EN,
  RU
} language_t;
//function to initialise the list of addresses
void _initList(addr_t addrList[], int size) {
  register int counter = 0;

  for (; counter < size; counter++) {
    addrList[counter].name[0] = '\0';
  }
}
//function to display a message of error
void _error(error_t err, const char* name, language_t language) {
  switch (err) {
  case INPUT_ERROR_T:
    if (language == RU) {
      puts("\nОшибка при вводе.");
    }
    else {
      puts("\nInput error.");
    }
    break;
  case LOAD_ERROR_T:
    if (language == RU) {
      puts("\nОшибка при загрузке.");
    }
    else {
      puts("\nLoading error.");
    }
    break;
  case SAVE_ERROR_T:
    if (language == RU) {
      puts("\nОшибка при сохранении.");
    }
    else {
      puts("\nSaving error.");
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
      printf_s("\nФайл %s:\nОшибка работы с файлом: возможная причина - очень длинный путь к файлу.\n", name);
      puts("Максимальная длина - 240 символов.");
    }
    else {
      printf_s("\nFile %s:\nError working with the file: a possible reason - a very long file path.\n", name);
      puts("Maximum length - 240 characters.");
    }
    break;
  }
}
//function to display menu
menu_t _menu(language_t language) {
  int point = 0;
  char str[NAME];

  if (language == RU) {
    puts("\n\t1. Ввести имя");
    puts("\t2. Удалить имя");
    puts("\t3. Вывести список");
    puts("\t4. Сохранить файл");
    puts("\t5. Загрузить файл");
    puts("\t6. Выход");
  }
  else {
    puts("\n\t1. Enter name");
    puts("\t2. Delete name");
    puts("\t3. Display the list");
    puts("\t4. Save the file");
    puts("\t5. Load the file");
    puts("\t6. Exit");
  }
  //select a menu item
  do {
    if (language == RU) {
      printf_s("\nВведите номер нужного пункта: ");
    }
    else {
      printf_s("\nEnter the number of the desired menu item: ");
    }
    if (fgets(str, NAME, stdin)) {
      point = atoi(str);
    }
    else {
      _error(INPUT_ERROR_T, "", language);
      return EXIT_MENU_T;
    }
  } while (point < ENTER_MENU_T || point > EXIT_MENU_T);

  return point;
}
//function to find a free position in the list
int _findFree(addr_t addrList[], int size) {
  register int counter = 0;

  for (; counter < size && addrList[counter].name[0]; counter++);

  if (counter == size)
    return -1;

  return counter;
}
//function to add a new name in the list
void _enter(addr_t addrList[], int size, language_t language) {
  int slot;
  unsigned long int zip = 0;
  char str[NAME];
  //find a free position in the list
  slot = _findFree(addrList, size);
  if (slot == -1) {
    if (language == RU) {
      puts("\nСписок заполнен");
    }
    else {
      puts("\nThe list is full");
    }
    return;
  }
  //add a new name
  else {
    //enter name
    if (language == RU) {
      printf_s("Введите имя: ");
    }
    else {
      printf_s("Enter name: ");
    }
    if (!fgets(addrList[slot].name, NAME, stdin)) {
      _error(INPUT_ERROR_T, "", language);
      addrList[slot].name[0] = '\0';
      return;
    }
    //enter street
    if (language == RU) {
      printf_s("Введите улицу: ");
    }
    else {
      printf_s("Enter street: ");
    }
    if(!fgets(addrList[slot].street, NAME, stdin)) {
      _error(INPUT_ERROR_T, "", language);
      addrList[slot].name[0] = '\0';
      return;
    }
    //enter city
    if (language == RU) {
      printf_s("Введите город: ");
    }
    else {
      printf_s("Enter city: ");
    }
    if(!fgets(addrList[slot].city, NAME, stdin)) {
      _error(INPUT_ERROR_T, "", language);
      addrList[slot].name[0] = '\0';
      return;
    }
    //enter state
    if (language == RU) {
      printf_s("Введите государство: ");
    }
    else {
      printf_s("Enter state: ");
    }
    if(!fgets(addrList[slot].state, NAME, stdin)) {
      _error(INPUT_ERROR_T, "", language);
      addrList[slot].name[0] = '\0';
      return;
    }
    //enter zip
    if (language == RU) {
      printf_s("Введите почтовый индекс: ");
    }
    else {
      printf_s("Enter zip: ");
    }
    if (fgets(str, NAME, stdin)) {
      zip = strtoul(str, '\0', 10);
      addrList[slot].zip = zip;
    }
    else {
      _error(INPUT_ERROR_T, "", language);
      addrList[slot].name[0] = '\0';
      return;
    }
  }
}
//function to delete a name from the list
void _delete(addr_t addrList[], int size, language_t language) {
  int slot = -1;
  char str[NAME];
  //enter entry No.
  if (language == RU) {
    printf_s("Введите № записи: ");
  }
  else {
    printf_s("Enter entry №: ");
  }
  if (fgets(str, NAME, stdin)) {
    slot = atoi(str);
    slot--;
  }
  else {
    _error(INPUT_ERROR_T, "", language);
    return;
  }
  //delete entry
  if (slot > -1 && slot < size) {
    addrList[slot].name[0] = '\0';
  }
}
//function to display the list of addresses
void _display(addr_t addrList[], int size) {
  register int counter = 0;

  for (; counter < size; counter++) {
    if (addrList[counter].name[0]) {
      printf_s("%3d. %s\n", counter + 1, addrList[counter].name);
      printf_s("     %s\n", addrList[counter].street);
      printf_s("     %s\n", addrList[counter].city);
      printf_s("     %s\n", addrList[counter].state);
      printf_s("     %lu\n\n", addrList[counter].zip);
    }
  }
}
//function to save the list of addresses in the file
void _save(char* path, addr_t addrList[], int size, language_t language) {
  register int counter = 0;
  FILE* stream = NULL;
  //open file
  if ((stream = fopen(path, "wb")) == NULL) {
    _error(CREATE_ERROR_T, path, language);
    return;
  }
  //save file
  for (; counter < size; counter++) {
    if (addrList[counter].name[0])
      if (fwrite(&addrList[counter], sizeof(addr_t), 1, stream) != 1) {
        _error(SAVE_ERROR_T, "", language);
        break;
      }
  }
  //close file
  if (fclose(stream))
    _error(CLOSE_ERROR_T, path, language);
}
//function to load the list of addresses from the file
void _load(char* path, addr_t addrList[], int size, language_t language) {
  register int counter = 0;
  FILE* stream = NULL;
  //open file
  if ((stream = fopen(path, "rb")) == NULL) {
    _error(OPEN_ERROR_T, path, language);
    return;
  }
  //initialise the list of addresses
  _initList(addrList, size);
  //load file
  for (; counter < size; counter++) {
    if (fread(&addrList[counter], sizeof(addr_t), 1, stream) != 1) {
      if (feof(stream))
        break;
      _error(LOAD_ERROR_T, "", language);
      break;
    }
  }
  //close file
  if (fclose(stream))
    _error(CLOSE_ERROR_T, path, language);
}
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
//main function
int main(int argc, char* argv[]) {
  addr_t addrList[LIST];
  menu_t choice;
  char path[_MAX_PATH], name[] = "vault.txt";
  language_t language = EN;
  char* locale;
  //select language
  locale = setlocale(LC_ALL, "");
  if (locale) {
    if (!strcmp(locale, "Russian_Russia.1251")) {
      language = RU;
      SetConsoleCP(1251);
      SetConsoleOutputCP(1251);
    }
  }
  /*get a path to the file
    to create a new file for output*/
  if (strcpy_s(path, _MAX_PATH, argv[0])) {
    _error(PATH_ERROR_T, argv[0], language);
    exit(7);
  }
  _getPath(path);
  //add name of the file
  if (strcat_s(path, _MAX_PATH, name)) {
    _error(PATH_ERROR_T, argv[0], language);
    exit(7);
  }
  //initialise the list of addresses
  _initList(addrList, LIST);
  //do until a "choice" is an "EXIT"
  for (;;) {
    choice = _menu(language);
    switch (choice) {
      //add a new name
      case ENTER_MENU_T:
        _enter(addrList, LIST, language);
        break;
      //delete a name
      case DEL_MENU_T:
        _delete(addrList, LIST, language);
        break;
      //display list of addresses
      case DISPLAY_MENU_T:
        _display(addrList, LIST);
        break;
      //save the list of addresses in the file
      case SAVE_MENU_T:
        _save(path, addrList, LIST, language);
        break;
      //load the list of addresses from the file
      case LOAD_MENU_T:
        _load(path, addrList, LIST, language);
        break;
      //close program
      case EXIT_MENU_T:
        exit(0);
    }
  }

  return 0;
}