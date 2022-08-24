#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <locale.h>

#define STATISTIC 2
#define START 0
#define END 0
#define CONTINUE 1
#define SIZE 1
#define DEEP 5

typedef enum {
	TWO_PLAYERS,
	ONE_PLAYER
} mode_t;
typedef enum {
	NO,
	PROPOSED,
	CONFIRMED
} draw_t;
//������������� ������� ����
static void _initMatrix(char matr[], int size) {
	int i = 0;

	for (; i < size * size; i++) {
		matr[i] = ' ';
	}
}
//�������� �������� ����������
static char _check(char matr[], int size) {
	int i, j, count;
	//������
	for (i = 0; i < size; i++) {
		for (count = 0, j = 1; j < size; j++)
			if (matr[i * size + j - 1] == matr[i * size + j]
				&& matr[i * size + j] != ' ')
				count++;
		if (count == size - 1)
			return matr[i * size];
	}
	//�������
	for (i = 0; i < size; i++) {
		for (count = 0, j = 1; j < size; j++)
			if (matr[i + (j - 1) * size] == matr[i + j * size]
				&& matr[i + j * size] != ' ')
				count++;
		if (count == size - 1)
			return matr[i];
	}
	//������� ���������
	for (count = 0, j = 1; j < size; j++)
		if (matr[(j - 1) * (size + 1)] == matr[j * (size + 1)]
			&& matr[j * (size + 1)] != ' ')
			count++;
	if (count == size - 1)
		return matr[0];
	//�������� ���������
	for (count = 0, j = 1; j < size; j++)
		if (matr[size - 1 + (j - 1) * (size - 1)] == matr[size - 1 + j * (size - 1)]
			&& matr[size - 1 + j * (size - 1)] != ' ')
			count++;
	if (count == size - 1)
		return matr[size - 1];

	return ' ';
}
//�������� ���� �� �������������
static int _isFull(char matr[], int size) {
	int i = 0;

	for (; i < size * size; i++) {
		if (matr[i] == ' ') break;
	}
	if (i == size * size) {
		return 1;
	}

	return 0;
}
//����� ��������� ���� �� ����
static int* _giveFreePositions(char matr[], int size, int* numOfPositions) {
	int* freePosition = malloc(sizeof(int) * SIZE);
	int i = 0;
	int* helper = NULL;
	
	*numOfPositions = 0;
	if (freePosition) {
		for (; i < size * size; i++) {
			if (matr[i] == ' ') {
				freePosition[*numOfPositions] = i;
				(*numOfPositions)++;
				helper = realloc(freePosition, sizeof(int) * (*numOfPositions + 1));
				if (helper) {
					freePosition = helper;
				}
				else {
					break;
				}
			}
		}
	}

	return freePosition;
}
//����������� �������
static char* _copyMatrix(char matr[], int size) {
	char* newMatr = malloc(sizeof(char) * size * size);
	int i = 0;

	if (newMatr) {
		for (; i < size * size; i++) {
			newMatr[i] = matr[i];
		}
	}

	return newMatr;
}
//���������� ������������
static int _minimise(char matr[], int size, int deep);
//��� �������������
static int _maximise(char matr[], int size, int deep) {
	int bestMove = INT_MIN;
	int move = 0;
	int* freePositions = NULL;
	int numOfPositions = 0;
	int i = 0;
	char* newMatr = NULL;
	
	if (_check(matr, size) == '�') {
		return 4;
	}
	else if(_check(matr, size) == '�') {
		return 1;
	}
	else if (_isFull(matr, size)) {
		return 3;
	}
	else if (deep >= DEEP) {
		return 2;
	}
	else {
		freePositions = _giveFreePositions(matr, size, &numOfPositions);
		if (freePositions) {
			for (; i < numOfPositions; i++) {
				newMatr = _copyMatrix(matr, size);
				if (newMatr) {
					newMatr[freePositions[i]] = '�';
					move = _minimise(newMatr, size, deep + 1);
					if (move > bestMove)
						bestMove = move;
					free(newMatr);
				}
				else {
					free(freePositions);
					if (bestMove != INT_MIN)
						return bestMove;
					else
						return 2;
				}
			}
			free(freePositions);
			return bestMove;
		}
		else {
			return 2;
		}
	}
}
//��� ������������
static int _minimise(char matr[], int size, int deep) {
	int bestMove = INT_MAX;
	int move = 0;
	int* freePositions = NULL;
	int numOfPositions = 0;
	int i = 0;
	char* newMatr = NULL;

	if (_check(matr, size) == '�') {
		return 4;
	}
	else if (_check(matr, size) == '�') {
		return 1;
	}
	else if (_isFull(matr, size)) {
		return 3;
	}
	else if (deep >= DEEP) {
		return 2;
	}
	else {
		freePositions = _giveFreePositions(matr, size, &numOfPositions);
		if (freePositions) {
			for (; i < numOfPositions; i++) {
				newMatr = _copyMatrix(matr, size);
				if (newMatr) {
					newMatr[freePositions[i]] = '�';
					move = _maximise(newMatr, size, deep + 1);
					if (move < bestMove)
						bestMove = move;
					free(newMatr);
				}
				else {
					free(freePositions);
					if (bestMove != INT_MAX)
						return bestMove;
					else
						return 2;
				}
			}
			free(freePositions);
			return bestMove;
		}
		else {
			return 2;
		}
	}
}
//����� ������� ����
static int _findBestMove(char matr[], int size) {
	int bestMove = 0;
	int previous = INT_MIN;
	int* freePositions = NULL;
	int numOfPositions = 0;
	int i = 0, j = 0;
	int current = 0;
	char* newMatr = NULL;
	int deep = 0;

	freePositions = _giveFreePositions(matr, size, &numOfPositions);
	if (freePositions) {
		for (; i < numOfPositions; i++) {
			newMatr = _copyMatrix(matr, size);
			if (newMatr) {
				newMatr[freePositions[i]] = '�';
				current = _minimise(newMatr, size, deep + 1);
				if (current > previous) {
					previous = current;
					bestMove = freePositions[i];
				}
				free(newMatr);
			}
			else {
				free(freePositions);
				if (bestMove != INT_MIN)
					return bestMove;
				else {
					for (j = 0; j < size * size; j++) {
						if (matr[j] == ' ') break;
					}
					return j;
				}
			}
		}
		free(freePositions);
		return bestMove;
	}
	else {
		for (i = 0; i < size * size; i++) {
			if (matr[i] == ' ') break;
		}
		return i;
	}
}
//��� ����������
static int _getComputerMove(char matr[], int size, draw_t* draw) {
	if (_isFull(matr, size)) {
		return END;
	}
	else {
		if (*draw == PROPOSED) {
			*draw = CONFIRMED;
			printf_s("����� �������!\n");
		}
		else {
			matr[_findBestMove(matr, size)] = '�';
		}
		return CONTINUE;
	}
}
//��� ������
static int _getPlayerMove(char matr[], int size, int secondPlayer, draw_t* draw) {
	int x = -1, y = -1;
	//�������� ����������� ����
	if (_isFull(matr, size)) {
		return END;
	}
	//��� ������
	else {
		printf_s("������� ���������� ����, � � � (����� ����������/����������� �� �����, ������� 0 0): ");
		scanf_s("%d %d", &x, &y);
		scanf_s("%*[^\n]");
		x--;
		y--;
		if (x >= size || y >= size || x < 0 || y < 0 || matr[x * size + y] != ' ') {
			//��������� �����
			if (*draw == PROPOSED && (x != -1 || y != -1)) {
				printf_s("����� ���������!\n");
				*draw = NO;
				return CONTINUE;
			}
			else {
				//������� �����
				if (x == -1 && y == -1 && *draw == PROPOSED) {
					*draw = CONFIRMED;
					printf_s("����� �������!\n");
				}
				//���������� �����
				else if (x == -1 && y == -1) {
					*draw = PROPOSED;
					printf_s("���������� �����!\n");
				}
				//�������� ���
				else {
					printf_s("�������� ���, ���������� �����.\n");
					_getPlayerMove(matr, size, secondPlayer, draw);
				}
				return CONTINUE;
			}
		}
		else {
			if (!secondPlayer)
				matr[x * size + y] = '�';
			else
				matr[x * size + y] = '�';
			return CONTINUE;
		}
	}
}
//����� �������� ��������� ����
static void _dispMatrix(char matr[], int size) {
	int i = 0, j;

	for (; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf_s("|---");
		}
		printf_s("|\n");
		for (j = 0; j < size; j++) {
			printf_s("| %c ", matr[i * size + j]);
		}
		printf_s("|\n");
	}
	for (j = 0; j < size; j++) {
		printf_s("|---");
	}
	printf_s("|\n\n");
}
//����
static int _menu(int param, mode_t* mode) {
	int state = 1;
	int helper = 0;
	//����� ����������
	if (param) {
		do {
			printf_s("\n\t����������(2).\n\t����� ����(1).\n\t�����(0).\n\n");
			scanf_s("%d", &state);
			scanf_s("%*[^\n]");
		} while (state < 0 || state > 2);
	}
	else {
		//����� ������
		do {
			printf_s("�������� ����� (������ ����������(1) ��� ��� ������(0)): ");
			scanf_s("%d", &helper);
			scanf_s("%*[^\n]");
		} while (helper < 0 || helper > 1);
		*mode = (mode_t)helper;
		//����� ������� ����
		do {
			printf_s("�������� ������ ���� (3�3, 4�4 ��� 5�5): ");
			scanf_s("%d", &state);
			scanf_s("%*[^\n]");
		} while (state < 3 || state > 5);
	}

	return state;
}
//����
static void _game(char matr[], int size, char* done, mode_t mode) {
	draw_t draw = NO;

	_initMatrix(matr, size);
	//������ ����������
	if (mode == ONE_PLAYER) {
		do {
			if (!_getComputerMove(matr, size, &draw))
				break;
			else {
				*done = _check(matr, size);
				if (!draw)
					_dispMatrix(matr, size);
				if (*done != ' ' || draw == CONFIRMED) break;
			}
			if (!_getPlayerMove(matr, size, 0, &draw))
				break;
			else {
				*done = _check(matr, size);
				if (!draw)
					_dispMatrix(matr, size);
			}
		} while (*done == ' ' && draw != CONFIRMED);
	}
	//��� ������
	else {
		do {
			if (!_getPlayerMove(matr, size, 0, &draw))
				break;
			else {
				*done = _check(matr, size);
				if (!draw)
					_dispMatrix(matr, size);
				if (*done != ' ' || draw == CONFIRMED) break;
			}
			if (!_getPlayerMove(matr, size, 1, &draw))
				break;
			else {
				*done = _check(matr, size);
				if (!draw)
					_dispMatrix(matr, size);
			}
		} while (*done == ' ' && draw != CONFIRMED);
	}
}

int main(void) {
	int size = 1;
	char* matrix;
	char done = ' ';
	int state = 1;
	int statistic[3] = { 0, 0, 0 };
	mode_t mode = ONE_PLAYER;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

	setlocale(LC_CTYPE, "Russian");
	do {
		if (state == STATISTIC) {
			printf_s("\n\t������ ������: %d", statistic[0]);
			printf_s("\n\t������ ����������/������� ������: %d", statistic[1]);
			printf_s("\n\t�����: %d\n", statistic[2]);
			state = _menu(CONTINUE, &mode);
		}
		else {
			size = _menu(START, &mode);
			matrix = malloc(size * size * sizeof(char));
			if (matrix) {
				_game(matrix, size, &done, mode);
				switch (done) {
				case '�':
					printf_s("������ ������!\n");
					statistic[0]++;
					break;
				case '�':
					printf_s("������ ����������/������� ������!\n");
					statistic[1]++;
					break;
				default:
					printf_s("�����!\n");
					statistic[2]++;
				}
				free(matrix);
				state = _menu(CONTINUE, &mode);
			}
			else {
				printf_s("�������� ������, ��������� � ��������� ������.\n");
				if (size > 3)
					printf_s("���������� ���� �������� �������.\n");
				state = _menu(CONTINUE, &mode);
			}
		}
	} while (state);
}