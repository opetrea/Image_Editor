#include <stdio.h>
#include "image_functions.h"

/*Functie care returneaza o matrice de dimensiune
n x m alocata si elemente de tip intreg*/
int **alloc_matrix(int n, int m)
{
	int **a;
	a = (int **)malloc(n * sizeof(int *));
	DIE(!a, "Nu am putut aloca memorie");

	for (int i = 0; i < n; i++) {
		a[i] = malloc(m * sizeof(int));
		DIE (!a[i], "Nu am putut aloca memorie");
	}
	return a;
}

/*Functie care returneaza o matrice de dimensiune
n x m alocata si elemente de tip double*/
double **alloc_matrix_double(int n, int m)
{
	double **a;
	a = (double **)malloc(n * sizeof(double *));
	DIE (!a, "Nu am putut aloca memorie");

	for (int i = 0; i < n; i++) {
		a[i] = malloc(m * sizeof(double));
		DIE (!a[i], "Nu am putut aloca memorie");
	}

	return a;
}

/*Functie care returneaza o matrice de dimensiune
n x m alocata si elemente de tip char*/
char **alloc_matrix_char(int n, int m)
{
	char **a;
	a = (char **)malloc(n * sizeof(char *));
	DIE (!a, "Nu am putut aloca memorie");

	for (int i = 0; i < n; i++) {
		a[i] = malloc(m * sizeof(char));
		DIE (!a[i], "Nu am putut aloca memorie");
	}

	return a;
}

/*Functie care primeste ca param
o matrice alocata si numarul ei de linii;
aceasta dealoca matricea*/
void free_matrix(int ***a, int n)
{
	for (int i = 0; i < n; i++)
		free((*a)[i]);
	free(*a);
}

void free_matrix_char(char ***a, int n)
{
	for (int i = 0; i < n; i++)
		free((*a)[i]);
	free(*a);
}

void free_matrix_double(double ***a, int n)
{
	for (int i = 0; i < n; i++)
		free((*a)[i]);
	free(*a);
}

/*Functie care interschimba valorile a doua variabile de tip int*/
void swap_function(int *a, int *b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

/*Functie ce imi verifica daca valoarea primita sa afla
in intervalul [0,A)*/
void clamp(int *a)
{
	if (*a < 0)
		*a = 0;
	if (*a >= A)
		*a = A - 1;
}

/*Functie ce imi sparge un sir de caractere in subsiruri
separate prin space si mi le introduce int-o matrice de caractere*/
void word_to_word(char ***word, char s[2000])
{
	/*Initializez primul caracter de pe fiecare linie a matricii
	cu caracterul NULL, pentru a nu ramane pe linii subsiruri de la
	comenzile anterioare*/
	for (int j = 0; j < 10; j++)
		(*word)[j][0] = '\0';

	int i = 0;
	char *p = strtok(s, " ");

	while (p) {
		strcpy((*word)[i], p);
		(*word)[i][strlen(p)] = '\0';
		if ((*word)[i][strlen(p) - 1] == '\n')
			(*word)[i][strlen(p) - 1] = '\0';
		p = strtok(NULL, " ");
		i++;
	}
}

/*Functie care intorarce matricea imaginii alocata si retine in variabilele
n, m, limita si type_f, numarul de linii, de coloane, limita superioara si
tipul imaginii incarcate in memorie*/
int **load_f(char **word, int *n, int *m, info_current_image *info)
{
	char type[3], cv[10], line[50], *p;
	int x, k = 0, i = 0, val[5], **matr;
	FILE *file = fopen(word[1], "r");
	if (!file) {
		printf("Failed to load %s\n", word[1]);
		return 0;
	}
	info->loaded = 1;
	fscanf(file, "%s", type);
	fgets(cv, 10, file);
	/*Instructiune repetiva in care ignor comentariile si retin dimensiunile
	imaginii si limita superioara*/
	while (k != 2) {
		fgets(line, 50, file);
		int x = strlen(line);
		line[x - 1] = '\0';
		if (strchr("0123456789", line[0]) != 0) {
			k++;
			p = strtok(line, " ");
			while (p != 0) {
				val[i] = atoi(p);
				i++;
				p = strtok(NULL, " ");
			}
		}
	}
	(*n) = val[1];
	(*m) = val[0];
	info->limita = val[2];
	/*Pentru fiecare tip de imagine, aloc o matrice, cu dimensiunile conform
	tipului imaginii, facand citirea in mod diferit pentru imaginile ascii si
	cele binare*/
	type[2] = '\0';
	if (strcmp(type, "P2") == 0) {
		matr = alloc_matrix(*n, *m);
		for (int i = 0; i < *n; i++)
			for (int j = 0; j < *m; j++)
				fscanf(file, "%d", &matr[i][j]);
	}
	if (strcmp(type, "P3") == 0) {
		matr = alloc_matrix((*n), 3 * (*m));
		for (int i = 0; i < (*n); i++)
			for (int j = 0; j < 3 * (*m); j++)
				fscanf(file, "%d", &matr[i][j]);
	}
	/*Pentru imaginile binare, retin unde ma aflu in fisier dupa ce citesc
	limita superioara, dupa care inchid fisierul, il deschid in modul rb,
	si cu ajutorul functiei fseek, reiau citirea de unde ramasesem in fisier
	intainte de a-l inchide*/
	x = ftell(file);
	if (strcmp(type, "P5") == 0) {
		fclose(file);
		file = fopen(word[1], "rb");
		fseek(file, x, SEEK_SET);
		matr = alloc_matrix((*n), (*m));
		for (int i = 0; i < (*n); i++)
			for (int j = 0; j < (*m); j++) {
				unsigned char c;
				fread(&c, sizeof(c), 1, file);
				matr[i][j] = (int)(c);
			}
	}
	if (strcmp(type, "P6") == 0) {
		fclose(file);
		file = fopen(word[1], "rb");
		fseek(file, x, SEEK_SET);
		matr = alloc_matrix((*n), 3 * (*m));
		for (int i = 0; i < (*n); i++)
			for (int j = 0; j < 3 * (*m); j++) {
				unsigned char c;
				fread(&c, sizeof(c), 1, file);
				matr[i][j] = (int)(c);
			}
	}
	strcpy(info->type, type);
	printf("Loaded %s\n", word[1]);
	fclose(file);
	return matr;
}

/*Functie care rezolva operatiile SELECT SI SELECT ALL*/
void select_function(char **word, info_current_image *info, int n,
					 int m)
{
	int out = 1, a1, a2, b1, b2;
	if (strcmp(word[1], "ALL") != 0) {
		if (info->loaded == 0) {
			printf("No image loaded\n");
			out = 0;
		} else {
			/*Deoarece a fost citit tot randul, convertesc numerele
			citite cu ajutorul functiei atoi*/
			a1 = atoi(word[1]);
			b1 = atoi(word[2]);
			a2 = atoi(word[3]);
			b2 = atoi(word[4]);

			if (a1 > a2)
				swap_function(&a1, &a2);
			if (b1 > b2)
				swap_function(&b1, &b2);

			/*Ma asigur ca comanda data este valida*/
			for (int i = 1; i <= 4; i++) {
				for (int j = 0; j < (int)strlen(word[i]); j++) {
					if (strchr("-0123456789", word[i][j]) == 0) {
						printf("Invalid command\n");
						return;
					}
				}
				if (strcmp(word[i], "\0") == 0) {
					printf("Invalid command\n");
					return;
				}
			}

			int invalid = 0;
			if (a1 < 0 || a2 <= 0 || b1  < 0 || b2 <= 0 || b1 == b2) {
				printf("Invalid set of coordinates\n");
				out = 0;
				invalid = 1;
			}
			if (invalid == 0)
				if (a1 >= m || a2 > m || b1 >= n || b2 > n || a1 == a2) {
					printf("Invalid set of coordinates\n");
					out = 0;
				}
		}

		/*In cazul in care variabila out ramane 1, sunt sigur atat de faptul
		ca comanda data este una valida, cat si ca este vorba despre SELECT,
		si nu SELECT ALL*/
		if (out == 1) {
			info->x1 = a1;
			info->x2 = a2;
			info->y1 = b1;
			info->y2 = b2;
			printf("Selected %d %d %d %d\n", a1, b1, a2, b2);
		}
	} else {
		if (info->loaded == 0) {
			printf("No image loaded\n");
		} else {
			info->x1 = 0;
			info->y1 = 0;
			info->x2 = m;
			info->y2 = n;

			printf("Selected ALL\n");
		}
	}
}

/*Functie care realizeaza histograma*/
void histogram_function(char **word, int n, int m, int **matrix,
						info_current_image info)
{
	if (info.loaded == 0) {
		printf("No image loaded\n");
	} else {
		/*Verific foarte riguros daca comanda primita este valida*/
		for (int i = 1; i < 10; i++) {
			if (i <= 2) {
				if (strcmp(word[i], "\0") == 0) {
					printf("Invalid command\n");
					return;
				}
			} else {
				if (strcmp(word[i], "\0") != 0) {
					printf("Invalid command\n");
					return;
				}
			}
		}
		if (strcmp(word[1], "\0") == 0 || strcmp(word[2], "\0") == 0) {
			printf("Invalid command\n");
			return;
		}

		if (strcmp(info.type, "P2") != 0 && strcmp(info.type, "P5") != 0) {
			printf("Black and white image needed\n");
		} else {
			int x, y;
			x = atoi(word[1]);
			y = atoi(word[2]);
			int interval = A / y, frecv[257] = { 0 }, maxx = 0;

			/*Determin frecventa fiecarui element din matrice*/
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++)
					frecv[matrix[i][j]]++;
			}

			/*Calculez maximul conform formulei date in enunt*/
			for (int i = 0; i < A; i = i + interval) {
				int frecv_s = 0;
				for (int j = i; j < i + interval; j++)
					frecv_s = frecv_s + frecv[j];
				if (frecv_s > maxx)
					maxx = frecv_s;
			}

			/*Aplic formula din enunt si afisez histograma*/
			for (int i = 0; i < A; i = i + interval) {
				int frecv_bin = 0;
				for (int j = i; j < i + interval; j++)
					frecv_bin = frecv_bin + frecv[j];
				int stars = frecv_bin * x / maxx;
				printf("%d\t|\t", stars);
				for (int j = 0; j < stars; j++)
					printf("*");
				printf("\n");
			}
		}
	}
}

/*Functie care realizeaza egalizarea imaginii*/
void equalize_function(int n, int m, int ***matr, info_current_image info)
{
	if (info.loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(info.type, "P2") != 0 && strcmp(info.type, "P5") != 0) {
		printf("Black and white image needed\n");
	} else {
		/*Pregatesc variabilele pentru aplicarea formulei din enunt*/
		int area = n * m, frecv[257] = { 0 };
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++)
				frecv[(*matr)[i][j]]++;
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				int sum = 0;
				for (int k = 0; k <= (*matr)[i][j]; k++)
					sum = sum + frecv[k];
				double f = A * sum * 1.0 / area;
				f = round(f);
				int a = f;
				clamp(&a);
				(*matr)[i][j] = a;
			}
		}
		printf("Equalize done\n");
	}
}

/*Functie care rezolva operatia CROP*/
void crop_function(int *n, int *m, int ***matr, info_current_image info)
{
	if (info.loaded == 0) {
		printf("No image loaded\n");
	} else {
		int m1 = info.x2 - info.x1;
		int n1 = info.y2 - info.y1;
		/*Realizez ca crop-ul se face in functie de tipul imaginii*/
		if (strcmp(info.type, "P2") == 0 || strcmp(info.type, "P5") == 0) {
			/*Aloc o matrice auxiliara in care sa copiez elementele
			care vreau sa fie pastrate in noua imagine*/
			int **aux = alloc_matrix(n1, m1);
			for (int i = 0; i < n1; i++) {
				for (int j = 0; j < m1; j++)
					aux[i][j] = (*matr)[info.y1 + i][info.x1 + j];
			}

			/*Dealoc matricea si o aloc in functie de noile ei dim*/
			free_matrix(matr, (*n));
			(*matr) = alloc_matrix(n1, m1);
			for (int i = 0; i < n1; i++) {
				for (int j = 0; j < m1; j++)
					(*matr)[i][j] = aux[i][j];
			}

			/*Dealoc matricea auxiliara*/
			free_matrix(&aux, n1);
		} else {
			int **aux = alloc_matrix(n1, 3 * m1);
			for (int i = 0; i < n1; i++) {
				for (int j = 0; j < 3 * m1; j++)
					aux[i][j] = (*matr)[info.y1 + i][3 * info.x1 + j];
			}
			free_matrix(matr, (*n));
			(*matr) = alloc_matrix(n1, 3 * m1);
			for (int i = 0; i < n1; i++) {
				for (int j = 0; j < 3 * m1; j++)
					(*matr)[i][j] = aux[i][j];
			}
			free_matrix(&aux, n1);
		}
		*n = n1;
		*m = m1;
		printf("Image cropped\n");
	}
}

/*Calculez pentru fiecare pixel din selectie(pentru care este posibil)
suma inmultirii elementelor matricii de 3 x 3 careia ii este kernel,
cu elementele din matricea auxiliara*/
void solve_pixel(double ***cp, double aux[3][3], int i1, int i2, int j1, int j2,
				 int ***matr)
{
	for (int i = i1; i <= i2; i++) {
		for (int j = j1; j <= j2; j++) {
			(*cp)[i][j] = (*matr)[i - 1][j - 3] * aux[0][0];
			(*cp)[i][j] = (*cp)[i][j] + (*matr)[i - 1][j] * aux[0][1];
			(*cp)[i][j] = (*cp)[i][j] + (*matr)[i - 1][j + 3] * aux[0][2];
			(*cp)[i][j] = (*cp)[i][j] + (*matr)[i][j + 3] * aux[1][2];
			(*cp)[i][j] = (*cp)[i][j] + (*matr)[i + 1][j + 3] * aux[2][2];
			(*cp)[i][j] = (*cp)[i][j] + (*matr)[i + 1][j] * aux[2][1];
			(*cp)[i][j] = (*cp)[i][j] + (*matr)[i + 1][j - 3] * aux[2][0];
			(*cp)[i][j] = (*cp)[i][j] + (*matr)[i][j - 3] * aux[1][0];
			(*cp)[i][j] = (*cp)[i][j] + (*matr)[i][j] * aux[1][1];
		}
	}
}

/*Functie care rezolva opearatia APPLY
In care, in functie de paramentrul citit,
introduc in memorie o alta matrice pentru
a realiza calculele necesare*/
void apply_function(char **word, int n, int m, int ***matr, info_current_image info)
{
	int ok = 0;
	if (info.loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(word[1], "\0") == 0) {
		printf("Invalid command\n");
		return;
	}
	if (strcmp(info.type, "P3") != 0 && strcmp(info.type, "P6") != 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	double aux[3][3];
	if (strcmp(word[1], "EDGE") == 0) {
		ok = 1;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				if (i == 1 && j == 1)
					aux[i][j] = 8;
				else
					aux[i][j] = -1;
		}
	}
	if (strcmp(word[1], "SHARPEN") == 0) {
		ok = 1;
		double aux1[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				aux[i][j] = aux1[i][j];
	}
	if (strcmp(word[1], "BLUR") == 0) {
		ok = 1;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				aux[i][j] = 1.0 / 9;
		}
	}
	if (strcmp(word[1], "GAUSSIAN_BLUR") == 0) {
		ok = 1;
		double aux1[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				aux1[i][j] = aux1[i][j] * 1.0 / 16;
		}
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				aux[i][j] = aux1[i][j];
	}
	if (ok == 0) {
		printf("APPLY parameter invalid\n");
		return;
	}
	double **cp_matr;
	cp_matr = alloc_matrix_double(n, 3 * m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3 * m; j++)
			cp_matr[i][j] = (*matr)[i][j];
	}
	int i1 = info.y1, i2 = info.y2 - 1, j1 = 3 * info.x1, j2 = 3 * info.x2 - 1;
	if (j1 == 0)
		j1 = j1 + 3;
	if (j2 == 3 * m - 1)
		j2 = j2 - 3;
	if (i1 == 0)
		i1++;
	if (i2 == n - 1)
		i2--;
	solve_pixel(&cp_matr, aux, i1, i2, j1, j2, matr);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3 * m; j++) {
			(*matr)[i][j] = round(cp_matr[i][j]);
			clamp(&(*matr)[i][j]);
		}
	}
	free_matrix_double(&cp_matr, n);
	printf("APPLY %s done\n", word[1]);
}

void save_function(char **word, int n, int m, int **matr, info_current_image info)
{
	char *name = malloc(50 * sizeof(char));
	name[0] = '\0';
	int color = 0;
	if (info.loaded == 0) {
		printf("No image loaded\n");
		free(name);
		return;
	}
	strcpy(name, word[1]);
	if (strcmp(word[2], "ascii") == 0) {
		/*Salvez imaginea intr-un fisier in modul text*/
		FILE *file = fopen(name, "w");
		if (strcmp(info.type, "P6") == 0 || strcmp(info.type, "P3") == 0) {
			fprintf(file, "%s\n", "P3");
			color = 1;
		} else {
			fprintf(file, "%s\n", "P2");
			color = 0;
		}
		fprintf(file, "%d %d\n%d\n", m, n, info.limita);
		if (color == 0) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++)
					fprintf(file, "%d ", matr[i][j]);
				fprintf(file, "\n");
			}
		} else {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < 3 * m; j++)
					fprintf(file, "%d ", matr[i][j]);
				fprintf(file, "\n");
			}
		}
		fclose(file);
		printf("Saved %s\n", name);
		free(name);
		return;
	}
	FILE *file = fopen(name, "w");
	/*Imi iau un contor pentru a vedea daca imaginea este color sau nu*/
	if (strcmp(info.type, "P6") == 0 || strcmp(info.type, "P3") == 0) {
		fprintf(file, "%s\n", "P6");
		color = 1;
	} else {
		fprintf(file, "%s\n", "P5");
		color = 0;
	}
	fprintf(file, "%d %d\n%d\n", m, n, info.limita);
	/*Inchid fisierul si il redeschid in modul append binary*/
	fclose(file);
	file = fopen(name, "ab");
	fseek(file, 0, SEEK_SET);
	if (color == 0) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++)
				fwrite(&matr[i][j], sizeof(unsigned char), 1, file);
		}
	} else {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < 3 * m; j++)
				fwrite(&matr[i][j], 1, sizeof(unsigned char), file);
		}
	}
	printf("Saved %s\n", name);
	free(name);
	fclose(file);
}

void rotate_90(int ****matr, int **n, int **m, info_current_image **info)
{
	int n1, m1, **aux1, **aux2;
	n1 = (*info)->y2 - (*info)->y1;
	m1 = (*info)->x2 - (*info)->x1;
	if (strcmp((*info)->type, "P2") == 0 || strcmp((*info)->type, "P5") == 0) {
		/*Aloc 2 matrici auxiliare*/
		aux1 = alloc_matrix(n1, m1);
		aux2 = alloc_matrix(m1, n1);
		/*Copiez submatricea care ar trebui rotita*/
		for (int i = (*info)->y1; i < (*info)->y2; i++) {
			for (int j = (*info)->x1; j < (*info)->x2; j++)
				aux1[i - (*info)->y1][j - (*info)->x1] = (**matr)[i][j];
		}
		/*Aplic formula descoperita pe cazuri
		particulare si apoi generalizata*/
		for (int i = 0; i < m1; i++) {
			int k = 0;
			for (int j = n1 - 1; j >= 0; j--) {
				aux2[i][k] = aux1[j][i];
				k++;
			}
		}

		/*In cazul in care trebuie rotita toata matricea,
		o dealoc si o aloc in functie de noile dimensiuni*/
		if (n1 != m1) {
			free_matrix(&(**matr), **n);
			**matr = alloc_matrix(**m, **n);
			(*info)->y2 = **m;
			(*info)->x2 = **n;
			swap_function(&(**n), &(**m));
		}

		/*Copiez in matricea initiala, submatricea rotita*/
		for (int i = (*info)->y1; i < (*info)->y2; i++) {
			for (int j = (*info)->x1; j < (*info)->x2; j++)
				(**matr)[i][j] = aux2[i - (*info)->y1][j - (*info)->x1];
		}
		free_matrix(&aux1, n1);
		free_matrix(&aux2, m1);
		return;
	}
	if (strcmp((*info)->type, "P3") == 0 || strcmp((*info)->type, "P6") == 0) {
		/*Repet in mare parte metoda aplicata anterior*/
		aux1 = alloc_matrix(n1, 3 * m1);
		aux2 = alloc_matrix(m1, 3 * n1);
		for (int i = (*info)->y1; i < (*info)->y2; i++) {
			for (int j = 3 * (*info)->x1; j < 3 * (*info)->x2; j++)
				aux1[i - (*info)->y1][j - 3 * (*info)->x1] = (**matr)[i][j];
		}

		/*Formula este diferita, dar se poate descoperi
		la fel ca cea anterioara*/
		for (int j = 0; j < m1; j++) {
			int k = 0;
			for (int i = n1 - 1; i >= 0; i--) {
				for (int d = j * 3; d < j * 3 + 3; d++) {
					aux2[j][k] = aux1[i][d];
					k++;
				}
			}
		}

		if (n1 != m1) {
			free_matrix(&(**matr), **n);
			**matr = alloc_matrix(**m, 3 * (**n));
			(*info)->y2 = **m;
			(*info)->x2 = **n;
			swap_function(&(**n), &(**m));
		}
		for (int i = (*info)->y1; i < (*info)->y2; i++) {
			for (int j = 3 * (*info)->x1; j < 3 * (*info)->x2; j++)
				(**matr)[i][j] = aux2[i - (*info)->y1][j - 3 * (*info)->x1];
		}
		free_matrix(&aux1, n1);
		free_matrix(&aux2, m1);
	}
}

void rotate_func(char **word, int *n, int *m, int ***matr, info_current_image *info)
{
	if (info->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (info->x2 - info->x1 != info->y2 - info->y1) {
		if (info->x1 != 0 && info->x2 != *m && info->y1 != 0 && info->y2 != *n) {
			printf("The selection must be square\n");
			return;
		}
	}

	/*Determin de cate ori trebuie sa intorc matricea cu 90 de grade*/
	if (strcmp(word[1], "0") == 0) {
		printf("Rotated %s\n", word[1]);
		return;
	}
	if (strcmp(word[1], "360") == 0 || strcmp(word[1], "-360") == 0) {
		printf("Rotated %s\n", word[1]);
		return;
	}
	if (strcmp(word[1], "90") == 0 || strcmp(word[1], "-270") == 0) {
		rotate_90(&matr, &n, &m, &info);
		printf("Rotated %s\n", word[1]);
		return;
	}
	if (strcmp(word[1], "180") == 0 || strcmp(word[1], "-180") == 0) {
		rotate_90(&matr, &n, &m, &info);
		rotate_90(&matr, &n, &m, &info);
		printf("Rotated %s\n", word[1]);
		return;
	}
	if (strcmp(word[1], "-90") == 0 || strcmp(word[1], "270") == 0) {
		rotate_90(&matr, &n, &m, &info);
		rotate_90(&matr, &n, &m, &info);
		rotate_90(&matr, &n, &m, &info);
		printf("Rotated %s\n", word[1]);
		return;
	}
	printf("Unsupported rotation angle\n");
}
