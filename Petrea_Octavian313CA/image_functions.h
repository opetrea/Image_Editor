#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

#define A 256

typedef struct
{
	int x1;
	int x2;
	int y1;
	int y2;
	int limita;
    int loaded;
	char type[3];
} info_current_image;

int **alloc_matrix(int n, int m);
double **alloc_matrix_double(int n, int m);
char **alloc_matrix_char(int n, int m);
void free_matrix(int ***a, int n);
void free_matrix_char(char ***a, int n);
void free_matrix_double(double ***a, int n);
void swap_function(int *a, int *b);
void clamp(int *a);
void word_to_word(char ***word, char s[2000]);
int **load_f(char **word, int *n, int *m, info_current_image *info);
void select_function(char **word, info_current_image *info, int n, int m);
void histogram_function(char **word, int n, int m, int **matrix, info_current_image info);
void equalize_function(int n, int m, int ***matr, info_current_image info);
void crop_function(int *n, int *m, int ***matr, info_current_image info);
void solve_pixel(double ***cp, double aux[3][3], int i1, int i2, int j1, int j2, int ***matr);
void apply_function(char **word, int n, int m, int ***matr, info_current_image info);
void save_function(char **word, int n, int m, int **matr, info_current_image info);
void rotate_90(int ****matr, int **n, int **m, info_current_image **info);
void rotate_func(char **word, int *n, int *m, int ***matr, info_current_image *info);

#endif