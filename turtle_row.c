/* PROGRAM:	turtle_row.c
   AUTHOR:	Brian Thompson
   DATE:	Feb 3, 2016
   PURPOSE:	Create a PostScript file that makes a row of squares
   USAGE:	gcc -o turtle_row turtle-row.c, ./turtle_row > postscriptfile.ps
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
void turtle_row(turtle_t *, int, int);

/*************************M*A*I*N*************************/
int main(void) {
	turtle_t boo;
	pen_t pen;

	set_ps_header(HEIGHT, WIDTH);
	pen_set_rgb(&pen, 0.1, 0.1, 0.8);
	turtle_goto(&boo, 10, 30);
	turtle_row(&boo, 20, 20);
	pen_set_rgb(&pen, 0.1, 0.8, 0.1);
	turtle_goto(&boo, 10, 300);
	turtle_row(&boo, 100, 3);
	pen_set_rgb(&pen, 0.8, 0.1, 0.1);
	turtle_goto(&boo, 150, 450);
	turtle_row(&boo, 3, 100);
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

/*DRAW A ROW OF SQUARES*/
void turtle_row(turtle_t *t, int count, int size){
	while(count>0){
		fprintf(stdout, "%f %f %d %d rectfill\n", (t->xPos-(size/2)), (t->yPos-(size/2)), size, size);
		count--;
		if(((t->xPos+size+2)<595 && (t->xPos+size+2)>0)&&count>0){
			t->xPos = t->xPos+size+2;
		}
	}
	return;
}/*END turtle_row*/
