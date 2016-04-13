/* PROGRAM:	turtle_B.c
   AUTHOR:	Brian Thompson
   DATE:	Feb 3, 2016
   PURPOSE:	This program uses a text file containing PostScript commands
		and outputs a PostScript file
   USAGE:	gcc -o turtle_B turtle-B.c, ./turtle_B < input.txt > postscriptfile.ps
*/

/*INCLUDE*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846
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
void turtle_reset(turtle_t *);
void pen_reset(pen_t *);
void turtle_state(turtle_t);
void process_commands(turtle_t *, pen_t *);
void turtle_goto(turtle_t *, float, float);
void pen_set_rgb(pen_t *, color_t);
int verify_rgb(color_t);
void turtle_walk(turtle_t *, float);
void turtle_turn(turtle_t *, float);
void turtle_square(turtle_t *, int);
void set_ps_header(int, int);
void set_ps_display();
void turtle_row(turtle_t *, int, int);
void pen_fill(pen_t *);
void turtle_polygon(turtle_t *, pen_t *, int, int);
void turtle_grid(turtle_t *, int, int, int);
int roll_dice(int, int);
void turtle_random_walk(turtle_t *, pen_t *, int);
void turtle_fractal(double, double, int);

/*************************M*A*I*N*************************/
int main(void) {
	turtle_t boo;
	pen_t pen;
	srand(time(NULL));

	set_ps_header(HEIGHT, WIDTH);
	process_commands(&boo, &pen);
	set_ps_display();

	return 0;
}
/*************************M*A*I*N*************************/

/*RESET TURTLE*/
void turtle_reset(turtle_t *t){
	t->xPos = t->yPos = 0.0;
	t->dir = 0.0;
	return;
}

/*RESET PEN*/
void pen_reset(pen_t *p){
	p->color.r = p->color.g = p->color.b = 0.0;
	p->down = 1;
	p->filled = 0;
	return;
}

/*DISPLAY TURTLE STATE*/
void turtle_state(turtle_t t){
	fprintf(stdout, "%%x-pos %f\ny-pos %f\ndirection %f\n", t.xPos, t.yPos, t.dir);
	return;
}

/*PROCESS COMMANDS*/
void process_commands(turtle_t *t, pen_t *p){
	char cmd;
	color_t c;
	int count, size, rows, columns;
	float x, y, r, d, s;

	while((fscanf(stdin, "%c", &cmd)) !=EOF){
		switch(cmd){
			case 'G':/*GOTO COORDINATES*/
				fscanf(stdin, "%f %f\n", &x, &y);
				fprintf(stdout, "%%Turtle goto (%f, %f)\n", x, y);
				turtle_goto(t, x, y);
					break;
			case 'C':/*CHANGE PEN COLOR*/
				fscanf(stdin, "%f %f %f\n", &c.r, &c.g, &c.b);
				fprintf(stdout, "%%Pen color changed to (%f, %f, %f)\n", c.r, c.g, c.b);
				if(verify_rgb(c)){
					pen_set_rgb(p, c);
				}
					break;
			case 'W':/*WALK FORWARD*/
				fscanf(stdin, "%f\n", &r);
				fprintf(stdout, "%%Turtle will move %f units from (%f, %f)\n", r, t->xPos, t->yPos);
				turtle_walk(t, r);
					break;
			case 'T':/*TURN CLOCKWISE*/
				fscanf(stdin, "%f\n", &d);
				fprintf(stdout, "%%Turtle has turned %f degrees\n", d);
				turtle_turn(t, d);
					break;
			case 'S':/*DRAW A SQUARE*/
				fscanf(stdin, "%f\n", &s);
				fprintf(stdout, "%%Turtle will draw a square of size %f\n", s);
				turtle_square(t, s);
					break;
			case 'R':/*DRAW ROW OF SQUARES*/
				fscanf(stdin, "%d %d/n", &count, &size);
				fprintf(stdout, "%%Turtle will draw %d squares of %d size in a row\n", count, size);
				turtle_row(t, count, size);
					break;
			case 'F':/*SWITCH PEN FILL/UNFILLED*/
				pen_fill(p);
					break;
			case 'P':/*DRAW A POLYGON*/
				fscanf(stdin, "%d %d", &count, &size);
				fprintf(stdout, "%%Turtle will draw a polygon with %d sides at %d size\n", count, size);
				turtle_polygon(t, p, count, size);
					break;
			case 'D':/*DRAW A GRID*/
				fscanf(stdin, "%d %d %d", &rows, &columns, &size);
				fprintf(stdout, "%%Turtle will draw a grid of squares with %d rows and %d columns of %d size\n", rows, columns, size);
				turtle_grid(t, rows, columns, size);
					break;
			case 'V':/*RANDOM WALK*/
				fscanf(stdin, "%d", &count);
				fprintf(stdout, "%%Turtle will walk around using random instructions\n");
				turtle_random_walk(t, p, count);
					break;
			case 'Y':/*DRAW A FRACTAL*/
				fscanf(stdin, "%d", &size);
				fprintf(stdout, "%%Create a circular fractal\n");
				turtle_fractal(t->xPos, t->yPos, size);
					break;
		}
	}
	return;
}/*END process_commands*/

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
void pen_set_rgb(pen_t *p, color_t c){
	p->color.r = c.r;
	p->color.g = c.g;
	p->color.b = c.b;
	fprintf(stdout, "%.2f %.2f %.2f setrgbcolor\n", c.r, c.g, c.b);
	return;
}/*END pen_set_rgb*/

/*VERIFY COLOR*/
int verify_rgb(color_t c){
	if(c.r<0 || c.r>1 || c.g<0 || c.g>1 || c.b<0 || c.b>1){return 0;}
	else{return 1;}
}/*END verify_rgb*/

/*DRAW LINE IN DIRECTION TURTLE IS FACING*/
void turtle_walk(turtle_t *t, float r){
	if((t->xPos+(r*cos(t->dir*PI/180)))<595 && (t->xPos+(r*cos(t->dir*PI/180)))>0)
	t->xPos = t->xPos+(r*cos(t->dir*PI/180));
	if((t->yPos+(r*sin(t->dir*PI/180)))<841 && (t->yPos+(r*sin(t->dir*PI/180)))>0)
	t->yPos = t->yPos+(r*sin(t->dir*PI/180));
	fprintf(stdout, "%f %f lineto\n", t->xPos, t->yPos);
	return;
}/*END turtle_walk*/

/*TURN TURTLE COUNTER CLOCKWISE*/
void turtle_turn(turtle_t *t, float d){
	while(d>360){
		d = d-360;
	}
	while(d<(-360)){
		d = d+360;
	}
	if(t->dir+d>360){
		t->dir = d-(360-t->dir);
	}
	else if(t->dir+d<0){
		t->dir = 360+(t->dir+d);
	}
	else{t->dir += d;}
	return;
}/*END turtle_turn*/

/*DRAW A SQUARE*/
void turtle_square(turtle_t *t, int size){
	fprintf(stdout, "%f %f %d %d rectfill\n", (t->xPos-(size/2)), (t->yPos-(size/2)), size, size);
	return;
}/*END turtle_square*/

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
		if(((t->xPos+size+2)<595&&(t->xPos+size+2)>0)&&count>0){
			t->xPos = t->xPos+size+2;
		}
	}
	return;
}/*END turtle_row*/

/*TOGGLE PEN FILLED/UNFILLED*/
void pen_fill(pen_t *p){
	if(p->filled==1){
		p->filled = 0;
	}
	else{
		p->filled = 1;
	}
	return;
}/*END pen_fill*/

/*DRAW A POLYGON WITH X SIDES*/
void turtle_polygon(turtle_t *t, pen_t *p, int count, int size){
	if(size>2){
		int i;
		for(i=0;i<count;i++){
			turtle_walk(t, size);
			turtle_turn(t, 360/count);
		}
		fprintf(stdout, "closepath\n");
		if(p->filled==1){
			fprintf(stdout, "fill\n");
		}
	}
	return;
}/*END turtle_polygon*/

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

/*GET RANDOM NUMBER*/
int roll_dice(int min, int max){
	return min+(rand()%max);
}/*END roll_dice*/

/*TRACE A RANDOM PATH AROUND THE SCREEN*/
void turtle_random_walk(turtle_t *t, pen_t *p, int steps){
	int i;
	color_t cc;
	for(i=0; i<steps; i++){
		int rndm = roll_dice(1, 4);
		if(rndm==1){
			turtle_walk(t, 10);
		}
		else if(rndm==2){
			turtle_turn(t, 90);
		}
		else if(rndm==3){
			turtle_turn(t, -90);
		}
		else{
			cc.r = (rand()%10)*0.1;
			cc.g = (rand()%10)*0.1;
			cc.b = (rand()%10)*0.1;
			pen_set_rgb(p, cc);
		}
	}
	return;
}/*END turtle_random_walk*/

/*DRAW A CIRCULAR FRACTAL*/
void turtle_fractal(double x, double y, int size){
	if(size>4){
		fprintf(stdout, "%f %f %d 0 360 arc\nstroke\n", x, y, size);
		turtle_fractal(x+(size/2), y, (size/2));
		turtle_fractal(x-(size/2), y, (size/2));
		turtle_fractal(x, y+(size/2), (size/2));
		turtle_fractal(x, y-(size/2), (size/2));
	}
	return;
}/*END turtle_fractal*/
