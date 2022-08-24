#define _CRTDBG_MAP_ALLOC

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <crtdbg.h>

#pragma warning(disable: 4996)

#define SIZE 7

typedef struct {
  int row;
  int col;
} cell;

cell* getSet(int num) {
  cell* set = malloc(sizeof(cell) * SIZE);
  int counter = 0;
  
  if (num < SIZE) {
    for (; counter < SIZE; counter++) {
      set[counter].row = counter;
      set[counter].col = num;
    }
  }
  if (num >= SIZE && num < (2 * SIZE)) {
    for (; counter < SIZE; counter++) {
      set[counter].row = num - SIZE;
      set[counter].col = SIZE - 1 - counter;
    }
  }
  if (num >= (2 * SIZE) && num < (3 * SIZE)) {
    for (; counter < SIZE; counter++) {
      set[counter].row = SIZE - 1 - counter;
      set[counter].col = SIZE - 1 - (num % SIZE);
    }
  }
  if (num >= (3 * SIZE)) {
    for (; counter < SIZE; counter++) {
      set[counter].row = 4 * SIZE - 1 - num;
      set[counter].col = counter;
    }
  }

  return set;
}

void exclude(int row, int col, int val, int restrictions[SIZE][SIZE][SIZE]) {
  int counter1 = 0;

  for (; counter1 < SIZE; counter1++) {
    if(counter1 != col)
      restrictions[row][counter1][val - 1] = 0;
  }
  for (counter1 = 0; counter1 < SIZE; counter1++) {
    if (counter1 != row)
      restrictions[counter1][col][val - 1] = 0;
  }
}

int findUnique(int restrictions[SIZE][SIZE][SIZE], int field[SIZE][SIZE]) {
  int counter1 = 0;
  int counter2;
  int counter3;
  int unique = 0;
  int pos = 0;
  int res = 0;

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      for (counter3 = 0; counter3 < SIZE; counter3++)
        if (restrictions[counter1][counter3][counter2]) {
          unique++;
          pos = counter3;
        }
      if (unique == 1 && field[counter1][pos] == 0) {
        res++;
        field[counter1][pos] = ++counter2;
        exclude(counter1, pos, counter2, restrictions);
        for (counter3 = 0; counter3 < SIZE; counter3++)
          if (counter3 != (counter2 - 1))
            restrictions[counter1][pos][counter3] = 0;
      }
      unique = 0;
    }
  }

  for (counter1 = 0; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      for (counter3 = 0; counter3 < SIZE; counter3++)
        if (restrictions[counter3][counter1][counter2]) {
          unique++;
          pos = counter3;
        }
      if (unique == 1 && field[pos][counter1] == 0) {
        res++;
        field[pos][counter1] = ++counter2;
        exclude(pos, counter1, counter2, restrictions);
        for (counter3 = 0; counter3 < SIZE; counter3++)
          if(counter3 != (counter2 - 1))
            restrictions[pos][counter1][counter3] = 0;
      }
      unique = 0;
    }
  }

  return res;
}

int findAlone(int restrictions[SIZE][SIZE][SIZE], int field[SIZE][SIZE]) {
  int counter1 = 0;
  int counter2;
  int counter3;
  int alone = 0;
  int res = 0;
  int val = 0;

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      for (counter3 = 0; counter3 < SIZE; counter3++)
        if (restrictions[counter1][counter2][counter3]) {
          alone++;
          val = counter3 + 1;
        }
      if (alone == 1 && field[counter1][counter2] == 0) {
        res++;
        field[counter1][counter2] = val;
        exclude(counter1, counter2, val, restrictions);
      }
      alone = 0;
    }
  }

  return res;
}

int solved(const int field[SIZE][SIZE]) {
  int counter1 = 0;
  int counter2;
  int res = 0;

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      if (field[counter1][counter2])
        res++;
    }
  }

  return res;
}

void getSetVal(const cell* set, int matr[SIZE][SIZE], const int restrictions[SIZE][SIZE][SIZE]) {
  int counter1 = 0;
  int counter2;

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      matr[counter1][counter2] = restrictions[set[counter1].row][set[counter1].col][counter2];
    }
  }
}

void excludeVal(int row, int val, int matr[SIZE][SIZE]) {
  int counter = 0;

  for (; counter < SIZE; counter++) {
    if (counter != row)
      matr[counter][val - 1] = 0;
  }
}

int factorial(int num) {
  int res = 1;

  for (; num; num--) {
    res *= num;
  }

  return res;
}

void copyMatr(int to[SIZE][SIZE], const int from[SIZE][SIZE]) {
  int counter1 = 0;
  int counter2;

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      to[counter1][counter2] = from[counter1][counter2];
    }
  }
}

void matrToArr(const int matr[SIZE][SIZE], int arr[SIZE]) {
  int counter1 = 0;
  int counter2;

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      if (matr[counter1][counter2])
        arr[counter1] = matr[counter1][counter2];
    }
  }
}

int check(int* arr, int hint) {
  int counter = 0;
  int res = 0;
  int high = 0;

  for (; counter < SIZE; counter++) {
    if (arr[counter] > high) {
      res++;
      high = arr[counter];
    }
  }

  return (res == hint);
}

void seq(int matr[SIZE][SIZE], int hint, int stage, int** sequences, int* size) {
  int counter1 = 0;
  int counter2;
  int newMatr[SIZE][SIZE];
  int* arr;
  int full = 0;

  if (stage == SIZE - 1) {
    for (; counter1 < SIZE; counter1++) {
      if (matr[stage][counter1])
        full++;
    }
    if (full) {
      arr = malloc(SIZE * sizeof(int));
      matrToArr(matr, arr);
      if (check(arr, hint))
        sequences[(*size)++] = arr;
      else
        free(arr);
    }
  }
  else {
    for (; counter1 < SIZE; counter1++) {
      copyMatr(newMatr, matr);
      if (newMatr[stage][counter1]) {
        for (counter2 = 0; counter2 < SIZE; counter2++) {
          if (counter2 != counter1)
            newMatr[stage][counter2] = 0;
        }
        excludeVal(stage, counter1 + 1, newMatr);
        seq(newMatr, hint, stage + 1, sequences, size);
      }
    }
  }
}

void select(const cell* set, int** sequences, int size, int restrictions[SIZE][SIZE][SIZE]) {
  int restr[SIZE][SIZE] = { {0},{0},{0},{0} };
  int counter1 = 0;
  int counter2;
  int val;

  for (; counter1 < size; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      val = sequences[counter1][counter2];
      restr[counter2][val - 1]++;
    }
  }
  for (counter1 = 0; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      if (!restr[counter1][counter2]) {
        restrictions[set[counter1].row][set[counter1].col][counter2] = 0;
      }
    }
  }
}

void allSeq(const int arr[], int restrictions[SIZE][SIZE][SIZE]) {
  int size = 0;
  int** sequences = calloc(factorial(SIZE), sizeof(int*));
  cell* set;
  int counter1 = 0;
  int counter2;
  int field[SIZE][SIZE];
  int hint;

  for (; counter1 < 4 * SIZE; counter1++) {
    hint = arr[counter1];
    if (hint) {
      set = getSet(counter1);
      getSetVal(set, field, restrictions);
      seq(field, hint, 0, sequences, &size);
      select(set, sequences, size, restrictions);
      free(set);
      for (counter2 = 0; counter2 < size; counter2++)
        free(sequences[counter2]);
      size = 0;
    }
  }
  free(sequences);
}

void copyRestrictions(int to[SIZE][SIZE][SIZE], const int from[SIZE][SIZE][SIZE]) {
  int counter1 = 0;
  int counter2;
  int counter3;

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      for (counter3 = 0; counter3 < SIZE; counter3++) {
        to[counter1][counter2][counter3] = from[counter1][counter2][counter3];
      }
    }
  }
}

void solve(int cur[SIZE][SIZE], int res[SIZE][SIZE], const int arr[], int restrictions[SIZE][SIZE][SIZE]) {
  int before = 0;
  int field[SIZE][SIZE];
  int newRestrictions[SIZE][SIZE][SIZE];
  int counter1 = 0;
  int counter2;
  int counter3;
  int counter4;
  int min = SIZE;
  cell point = {0,0};

  while ((before = solved(cur)) != SIZE * SIZE) {
    while (findUnique(restrictions, cur) || findAlone(restrictions, cur));
    allSeq(arr, restrictions);
    if (before == solved(cur)) {
        break;
    }
  }

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      counter4 = 0;
      for (counter3 = 0; counter3 < SIZE; counter3++) {
        if (restrictions[counter1][counter2][counter3])
          counter4++;
      }
      if (!counter4) {
        return;
      }
      else if (counter4 > 1 && counter4 < min) {
        min = counter4;
        point.row = counter1;
        point.col = counter2;
      }
      else;
    }
  }

  if (before != SIZE * SIZE) {
    for (counter1 = 0; counter1 < SIZE; counter1++) {
      copyMatr(field, cur);
      copyRestrictions(newRestrictions, restrictions);
      if (newRestrictions[point.row][point.col][counter1]) {
        for (counter2 = 0; counter2 < SIZE; counter2++) {
          if (counter2 != counter1)
            newRestrictions[point.row][point.col][counter2] = 0;
        }
        exclude(point.row, point.col, counter1 + 1, newRestrictions);
        findUnique(newRestrictions, field);
        solve(field, res, arr, newRestrictions);
      }
    }
  }
  else {
    copyMatr(res, cur);
  }
}

int** SolvePuzzle(int arr[]) {
  int restrictions[SIZE][SIZE][SIZE];
  int** res;
  cell* set;
  int field[SIZE][SIZE];
  int counter1 = 0;
  int counter2;
  int counter3;
  int dist = 0;

  for (; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      field[counter1][counter2] = 0;
      for (counter3 = 0; counter3 < SIZE; counter3++) {
        restrictions[counter1][counter2][counter3] = counter3 + 1;
      }
    }
  }
  res = malloc(SIZE * sizeof(int*));
  for (counter1 = 0; counter1 < SIZE; counter1++) {
    res[counter1] = malloc(SIZE * sizeof(int));
  }
  for (counter1 = 0; counter1 < (4 * SIZE); counter1++) {
    if (arr[counter1]) {
      set = getSet(counter1);
      if (arr[counter1] > 1 && arr[counter1] < SIZE) {
        for (counter2 = 0, dist = 0; counter2 < SIZE; counter2++) {
          for (counter3 = SIZE - arr[counter1] + 1 + dist; counter3 < SIZE; counter3++) {
            restrictions[set[counter2].row][set[counter2].col][counter3] = 0;
          }
          dist++;
        }
      }
      else if (arr[counter1] == 1) {
        exclude(set[0].row, set[0].col, SIZE, restrictions);
      }
      else {
        for (counter2 = 0; counter2 < SIZE; counter2++) {
          exclude(set[counter2].row, set[counter2].col, counter2 + 1, restrictions);
        }
      }
      free(set);
    }
  }

  solve(field, field, arr, restrictions);
  for (counter1 = 0; counter1 < SIZE; counter1++) {
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      res[counter1][counter2] = field[counter1][counter2];
    }
  }

  return res;
}

void printField(int** field) {
  int counter1 = 0;
  int counter2;

  printf("-----------------\n");
  for (; counter1 < SIZE; counter1++) {
    printf("|");
    for (counter2 = 0; counter2 < SIZE; counter2++) {
      printf(" %d |", field[counter1][counter2]);
    }
    printf("\n-----------------\n");
  }
}

int main(void) {
  int arr[SIZE*SIZE] = { 3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3 };
  int** matr;
  int counter1 = 0;

  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

  matr = SolvePuzzle(arr);
  printf("%u\n", clock());
  printField(matr);

  for (; counter1 < SIZE; counter1++) {
    free(matr[counter1]);
  }
  free(matr);

  return 0;
}