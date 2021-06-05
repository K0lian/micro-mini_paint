#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct s_mx
{
	int w;
	int h;
	char clr;
	char **map;
} t_mx;
typedef struct s_rec
{
	float x;
	float y;
	float r;
	char clr;
	char type;
} t_rec;

int get_b(FILE *f, t_mx *mx){
	if (fscanf(f, "%d %d %c\n", &mx->w, &mx->h, &mx->clr) == 3){
		if (mx->w > 300 || mx->w < 1 || mx->h > 300 || mx->h < 1)
			return 1;
		if (!(mx->map = malloc(sizeof(char*) * mx->h)))
			return 1;
		for (int i=0; i<mx->h; i++){
			if (!(mx->map[i] = malloc(sizeof(char) * mx->w)))
				return 1;
			for (int j=0; j<mx->w; j++)
				mx->map[i][j] = mx->clr;
		}
		return 0;
	}
	return 1;
}
int is_r(float x, float y, t_rec *rec){
	float d=sqrtf(powf(x-rec->x, 2.) + powf(y-rec->y, 2.));
	if (d <= rec->r){
		if (rec->r-d < 1.0F)
			return 2;
		return 1;
	}
	return 0;
}
void put_one(t_rec *rec, t_mx *mx){
	for(int i=0; i<mx->h; i++)
	for(int j=0; j<mx->w; j++){
		int is=is_r((float)j, (float)i, rec);
		if (is == 2 || (is == 1 && rec->type == 'C'))
			mx->map[i][j] = rec->clr;
	}
}
int get_r(FILE *f, t_mx *mx){
	t_rec rec;
	int d;
	while((d=fscanf(f, "%c %f %f %f %c\n", &rec.type, &rec.x, &rec.y, &rec.r, &rec.clr)) == 5){
		if ((rec.type == 'c' || rec.type == 'C') && rec.r > 0.0F)
			put_one(&rec, mx);
		else return 1;
	}
	if (d==-1)
		return 0;
	return 1;
}
void swq(t_mx *mx){
	for (int i=0; i<mx->h; i++){
		write(1, mx->map[i], mx->w);
		write(1, "\n", 1);
	}
}
int minii(FILE *file){
	t_mx mx;
	if (get_b(file, &mx))
		return 1;
	if (get_r(file, &mx))
		return 1;
	swq(&mx);
	return 0;
}
int main(int ac, char const *av[])
{
	FILE *file;
	if (ac == 2){
		file = fopen(av[1], "r");
		if (!file || minii(file)){
			write(1, "Error: Operation file corrupted\n", 32);
			return 1;
		}
		fclose(file);
		return 0;
	}
	write(1, "Error: argument\n", 16);
	return 1;
}