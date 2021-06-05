#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_matrix
{
	int width;
	int height;
	char color;
	char **map;
} t_matrix;

typedef struct s_rectangle
{
	float width;
	float height;
	float x;
	float y;
	char color;
	char type;
} t_rectangle;

int get_background(FILE *file, t_matrix *matrix)
{
	if (fscanf(file, "%d %d %c\n", &matrix->width, &matrix->height, &matrix->color) == 3)
	{
		if (matrix->width > 300 || matrix->width < 1 || matrix->height > 300 || matrix->height < 1)
			return 1;
		if (!(matrix->map = malloc(sizeof(char *) * matrix->height)))
			return 1;
		for(int i=0; i < matrix->height; i++)
		{
			if (!(matrix->map[i] = malloc(matrix->width)))
				return 1;
			for(int j=0; j < matrix->width; j++)
				matrix->map[i][j] = matrix->color;
		}
		return 0;
	}
	return 1;
}

int is_rectangle(float x, float y, t_rectangle *rectangle)
{
	if (x >= rectangle->x && y >= rectangle->y && x <= rectangle->width && y <= rectangle->height)
	{
		if (x - rectangle->x < 1.0F || y - rectangle->y < 1.0F || rectangle->width - x < 1.0F || rectangle->height - y < 1.0F)
			return 2;
		return 1;
	}
	return 0;
}

void put_one(t_rectangle *rectangle, t_matrix *matrix)
{
	rectangle->width += rectangle->x;
	rectangle->height += rectangle->y;
	for (int i = 0; i < matrix->height; i++)
	{
		for (int j = 0; j < matrix->width; j++)
		{
			int is = is_rectangle((float)j, (float)i, rectangle);
			if (is == 2 || (is == 1 && rectangle->type == 'R'))
				matrix->map[i][j] = rectangle->color;
		}
	}
}

int get_rectangles(FILE *file, t_matrix *matrix)
{
	t_rectangle rec;
	int retun;
	while ((retun = fscanf(file, "%c %f %f %f %f %c\n", &rec.type, &rec.x, &rec.y, &rec.width, &rec.height, &rec.color)) == 6)
	{
		if ((rec.type == 'r' || rec.type == 'R') && rec.width > 0 && rec.height > 0)
			put_one(&rec, matrix);
		else return 1;
	}
	if (retun == -1)
		return 0;
	return 1;
}

void draw_map(t_matrix *matrix)
{
	for (int i = 0; i < matrix->height; i++)
	{
		write(1, matrix->map[i], matrix->width);
		write(1, "\n", 1);
	}
}

int micro_paint(FILE *file)
{
	t_matrix matrix;
	if (get_background(file, &matrix))
		return 1;
	if (get_rectangles(file, &matrix))
		return 1;
	draw_map(&matrix);
	return 0;
}

int main(int ac, char const **av)
{
	FILE *file;
	if (ac == 2){
		file = fopen(av[1], "r");
		if (!file || micro_paint(file)){
			write(1, "Error: Operation file corrupted\n", 32);
			return 1;
		}
		return 0;
	}
	write(1, "Error: argument\n", 16);
	return 1;
}