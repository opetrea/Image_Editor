#include "image_functions.h"

int main(void)
{
	int **img, n = 0, m = 0, command = 0;
	char s[2000], **word = alloc_matrix_char(10, 50);

	info_current_image info;
	info.loaded = 0;

	while (1) {
		command = 0;
		/*Citesc cate un rand de la tastatura, ca mai apoi sa-l
		impart in cuvinte*/
		fgets(s, 2000, stdin);
		word_to_word(&word, s);
		if (strcmp(word[0], "LOAD") == 0) {
			if (info.loaded == 1) {
				free_matrix(&img, n);
				info.loaded = 0;
			}
			img = load_f(word, &n, &m, &info);
			info.x1 = 0;
			info.x2 = m;
			info.y1 = 0;
			info.y2 = n;
			command = 1;
		}
		if (strcmp(word[0], "SELECT") == 0) {
			select_function(word, &info, n, m);
			command = 1;
		}
		if (strcmp(word[0], "HISTOGRAM") == 0) {
			histogram_function(word, n, m, img, info);
			command = 1;
		}
		if (strcmp(word[0], "CROP") == 0) {
			crop_function(&n, &m, &img, info);
			command = 1;
			info.x1 = 0;
			info.y1 = 0;
			info.x2 = m;
			info.y2 = n;
		}
		if (strcmp(word[0], "EQUALIZE") == 0) {
			equalize_function(n, m, &img, info);
			command = 1;
		}
		if (strcmp(word[0], "APPLY") == 0) {
			apply_function(word, n, m, &img, info);
			command = 1;
		}
		if (strcmp(word[0], "SAVE") == 0) {
			save_function(word, n, m, img, info);
			command = 1;
		}
		if (strcmp(word[0], "ROTATE") == 0) {
			rotate_func(word, &n, &m, &img, &info);
			command = 1;
		}
		if (strcmp(word[0], "EXIT") == 0) {
			if (info.loaded == 0)
				printf("No image loaded\n");
			/*Opresc citirea comenzilor la introducerea comenzii EXIT*/
			break;
		}
		/*Verific daca comanda introdusa de la tastatura este una valida*/
		if (command == 0)
			printf("Invalid command\n");
	}
	/*Dealoc memorie alocata*/
	free_matrix_char(&word, 10);
	if (info.loaded == 1)
		free_matrix(&img, n);
	return 0;
}
