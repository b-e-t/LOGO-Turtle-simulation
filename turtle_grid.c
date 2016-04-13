/* PROGRAM:	turtle_grid.c
   AUTHOR:	Brian Thompson
   DATE:	Feb 3, 2016
   PURPOSE:	Create a PostScript file that makes a grid of squares
   USAGE:	gcc -o turtle_grid turtle-grid.c, ./turtle_grid > postscriptfile.ps
*/

/*INCLUDE*/
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 595
#define WIDTH 841

/*color_t holds info about the pen color*/
typedef struct {
	float r, g, b;
}color_t;

/*turtle_t holds info on the turtle position and direction*/
typedef struct {
	double xPos;
	double yPos;
	double dir;
}turtle_t;

/*pen_t holds info about the pen color, if it is down, and if it is a filled object*/
typedef struct {
	color_t color;
	int down;
	int filled;
}pen_t;

/*FUNCTION PROTOTYPES*/
void turtle_goto(turtle_t *, float, float);
void pen_set_rgb(pen_t *, float, float, float);
void set_ps_header(int, int);
void set_ps_display();
void turtle_grid(turtle_t *, int, int, int);

/*************************M*A*I*N*************************/
int main(void) {
	turtle_t boo;
	pen_t pen;

	set_ps_header(HEIGHT, WIDTH);
	pen_set_rgb(&pen, 0.1, 0.1, 0.8);
	turtle_goto(&boo, 50, 800);
	turtle_grid(&boo, 10, 10, 30);
	pen_set_rgb(&pen, 0.1, 0.8, 0.1);
	turtle_goto(&boo, 50, 600);
	turtle_grid(&boo, 45, 45, 10);
	pen_set_rgb(&pen, 0.8, 0.1, 0.1);
	turtle_goto(&boo, 50, 150);
	turtle_grid(&boo, 3, 10, 50);
	set_ps_display();

	return 0;
}
/*************************M*A*I*N*************************/

/*GOTO SPECIFIC LOCATION*/
void turtle_goto(turtle_t *t, float x, float y){
	if((x<595 && x>0) && (y<841 && y>0)){
		t->xPos = x;
		t->yPos = y;
	}
	fprintf(stdout, "%f %f moveto\n", x, y);
	return;
}/*END turtle_goto*/

/*CHANGE PEN COLOR*/
void pen_set_rgb(pen_t *p, float r, float g, float b){
	p->color.r = r;
	p->color.g = g;
	p->color.b = b;
	fprintf(stdout, "%.2f %.2f %.2f setrgbcolor\n", r, g, b);
	return;
}/*END pen_set_rgb*/

/*SET HEADER FOR PS FILE*/
void set_ps_header(int w, int h){
	fprintf(stdout, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(stdout, "%%%%BoundingBox: 0 0 %d %d\n\n", w, h);
	fprintf(stdout, "newpath\n");
	return;
}/*END set_ps_header*/

/*SET FOOTER FOR PS FILE*/
void set_ps_display(){
	fprintf(stdout, "\nstroke\nshowpage");
	return;
}/*END set_ps_display*/

/*DRAW A GRID OF SQUARES*/
void turtle_grid(turtle_t *t, int rows, int columns, int size){
	int i, j;
	double x = t->xPos;
	for(i=0;i<rows;i++){
		for(j=0;j<columns;j++){
			fprintf(stdout, "%f %f %d %d rectfill\nstroke\n", (t->xPos-(size/2)), (t->yPos-(size/2)), size, size);
			if(((t->xPos+size+2)<595 && (t->xPos+size+2)>0)&&j<columns-1){
				t->xPos = t->xPos+size+2;
			}
		}
		if(((t->yPos-(size+2))<841 && (t->yPos-(size+2))>0)&&i<rows-1){
			t->xPos = x;
			t->yPos = t->yPos-(size+2);
		}
	}
	return;
}/*END turtle_grid*/
