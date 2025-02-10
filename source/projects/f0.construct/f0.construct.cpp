//		----------------------------------------------------------
//		-- fredrik olofsson 010502 (rev.011004)					--
//		-- updated 040225 - for carbon using cw8.0				--
//		-- updated 070103 - for ub using xcode					--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	long valLeft;
	long mapping;
	long variant;
	long cols;
	long rows;
	long width;
	long height;
	double stepX;
	double stepY;
	long movementX;
	long movementY;
	long x;
	long y;
	long dx;
	long dy;
	void *out;
} t_f0ext;

void *f0ext_new(void);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long iterations);
void f0ext_mapping(t_f0ext *x, long m);
void f0ext_variant(t_f0ext *x, long v);
void f0ext_cols(t_f0ext *x, long col);
void f0ext_rows(t_f0ext *x, long row);
void f0ext_width(t_f0ext *x, long max_X);
void f0ext_height(t_f0ext *x, long max_Y);
void f0ext_stepX(t_f0ext *x, double stepX);
void f0ext_stepY(t_f0ext *x, double stepY);
void f0ext_movementX(t_f0ext *x, long movementX);
void f0ext_movementY(t_f0ext *x, long movementY);
void start(t_f0ext *x);
void drawFO(t_f0ext *x);
void variant1(t_f0ext *x);
void variant2(t_f0ext *x);
void variant3(t_f0ext *x);
void variant4(t_f0ext *x);
void variant5(t_f0ext *x);
void variant6(t_f0ext *x);
void mapping1(t_f0ext *x, int iCol, int jRow);
void mapping2(t_f0ext *x, int iCol, int jRow);
void mapping3(t_f0ext *x, int iCol, int jRow);
void mapping4(t_f0ext *x, int iCol, int jRow);
void mapping5(t_f0ext *x, int i, int iCol, int jRow);
void mapping6(t_f0ext *x, int i, int iCol, int jRow);
void mapping7(t_f0ext *x, int i, int iCol, int jRow);
void mapping8(t_f0ext *x, int j, int iCol, int jRow);
void mapping9(t_f0ext *x, int j, int iCol, int jRow);
void mapping10(t_f0ext *x, int j, int iCol, int jRow);
void mapping11(t_f0ext *x, int i, int j, int iCol, int jRow);
void mapping12(t_f0ext *x, int i, int j, int iCol, int jRow);
void mapping13(t_f0ext *x, int i, int j, int iCol, int jRow);
void mapping14(t_f0ext *x, int i, int j, int iCol, int jRow);
void mapping15(t_f0ext *x, int i, int j, int iCol, int jRow);
void mapping16(t_f0ext *x, int i, int j, int iCol, int jRow);
void slumpaRiktningX(t_f0ext *x);
void slumpaRiktningY(t_f0ext *x);
void slumpaRiktningXY(t_f0ext *x);
int mirrorX(t_f0ext *x, int ox);
int mirrorY(t_f0ext *x, int oy);
short myRandom();
void draw(t_f0ext *x, int ox, int oy);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.construct", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
	class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
	class_addmethod(c, (method)f0ext_mapping, "mapping", A_DEFLONG, 0);
	class_addmethod(c, (method)f0ext_variant, "variant", A_DEFLONG, 0);
	class_addmethod(c, (method)f0ext_cols, "cols", A_DEFLONG, 0);
	class_addmethod(c, (method)f0ext_rows, "rows", A_DEFLONG, 0);
	class_addmethod(c, (method)f0ext_width, "width", A_DEFLONG, 0);
	class_addmethod(c, (method)f0ext_height, "height", A_DEFLONG, 0);
	class_addmethod(c, (method)f0ext_stepX, "stepX", A_DEFFLOAT, 0);
	class_addmethod(c, (method)f0ext_stepY, "stepY", A_DEFFLOAT, 0);
	class_addmethod(c, (method)f0ext_movementX, "movementX", A_DEFLONG, 0);
	class_addmethod(c, (method)f0ext_movementY, "movementY", A_DEFLONG, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.construct v2.0; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(void) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valLeft= 2000;
        x->mapping= 1;
        x->variant= 1;
        x->cols= 3;
        x->rows= 2;
        x->width= 200;
        x->height= 200;
        x->stepX= 1.0;
        x->stepY= 1.0;
        x->movementX= 95;
        x->movementY= 93;
        x->x= 10;
        x->y= 10;
        x->dx= 1;
        x->dy= 1;
        x->out= listout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		sprintf(s, "anything");
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "[x, y, x+1, y+1]");
	}
}
void f0ext_bang(t_f0ext *x) {
	start(x);
}
void f0ext_int(t_f0ext *x, long iterations) {
	if(iterations>=1) {
		x->valLeft= iterations;
		f0ext_bang(x);
	}
}
void f0ext_mapping(t_f0ext *x, long m) {
	if((m>=1)&&(m<=16)) {
		x->mapping= m;
	}
}
void f0ext_variant(t_f0ext *x, long v) {
	if((v>=1)&&(v<=6)) {
		x->variant= v;
	}
}
void f0ext_cols(t_f0ext *x, long col) {
	if(col>=1) {
		x->cols= col;
	}
}
void f0ext_rows(t_f0ext *x, long row) {
	if(row>=1) {
		x->rows= row;
	}
}
void f0ext_width(t_f0ext *x, long max_X) {
	if(max_X>=1) {
		x->width= max_X;
	}
}
void f0ext_height(t_f0ext *x, long max_Y) {
	if(max_Y>=1) {
		x->height= max_Y;
	}
}
void f0ext_stepX(t_f0ext *x, double stepX) {
	x->stepX= stepX;
}
void f0ext_stepY(t_f0ext *x, double stepY) {
	x->stepY= stepY;
}
void f0ext_movementX(t_f0ext *x, long movementX) {
	if((movementX>=0)&&(movementX<=100)) {
		x->movementX= movementX;
	}
}
void f0ext_movementY(t_f0ext *x, long movementY) {
	if((movementY>=0)&&(movementY<=100)) {
		x->movementY= movementY;
	}
}
void start(t_f0ext *x) {
	int i;
	//srand(time(NULL));			//target specific - uncomment for windows
	for(i= 0; i<x->valLeft; i++) {
		drawFO(x);
		x->x= x->x+(x->dx*x->stepX);					// flytta position x
		if(x->x<1) {														// om vänster kant
			x->dx= myRandom()%2;											//   vänd eller stå still i x-led
		} else if(x->x>(x->width/x->cols)) {					// om höger kant
			x->dx= 0-(myRandom()%2);										//   vänd eller stå still i x-led
		}
		x->y= x->y+(x->dy*x->stepY);					// flytta position y
		if(x->y<1) {														// om övre kanten
			x->dy= myRandom()%2;											//   vänd eller stå still i y-led
		} else if(x->y>(x->height/x->rows)) {					// om nedre kanten
			x->dy= 0-(myRandom()%2);										//   vänd eller stå still i y-led
		}
		switch(x->variant) {
			case 1:
				variant1(x);
				break;
			case 2:
				variant2(x);
				break;
			case 3:
				variant3(x);
				break;
			case 4:
				variant4(x);
				break;
			case 5:
				variant5(x);
				break;
			case 6:
				variant6(x);
				break;
		}
	}
}
void variant1(t_f0ext *x) {											// 1. slumpa riktningar individuellt
	if(myRandom()%100>=x->movementX) {
		slumpaRiktningX(x);
	}
	if(myRandom()%100>=x->movementY) {
		slumpaRiktningY(x);
	}
}
void variant2(t_f0ext *x) {											// 2. slumpa alltid riktningar tillsammans
	slumpaRiktningXY(x);
}
void variant3(t_f0ext *x) {											// 3. slumpa riktningar individuellt
	if(myRandom()%100>=x->movementX) {
		if(x->dx==0&&x->x>=1) {								//		dra åt vänster
			x->dx= -1;
		} else {
			x->dx= 0;
		}
	}
	if(myRandom()%100>=x->movementY) {
		if(x->dy==0&&x->y>=1) {								//		dra uppåt
			x->dy= -1;
		} else {
			x->dy= 0;
		}
	}
}
void variant4(t_f0ext *x) {											// 4. slumpa riktningar individuellt
	if(myRandom()%100>=x->movementX) {
		if(x->dx==0&&x->x>=1) {								//		dra åt vänster
			x->dx= -1;
		} else {
			x->dx= 0;
		}
	}
	if(myRandom()%100>=x->movementY) {
		if(x->dy==0&&x->y<=x->height/x->rows) {	//		dra nedåt
			x->dy= 1;
		} else {
			x->dy= 0;
		}
	}
}
void variant5(t_f0ext *x) {											// 5. slumpa riktningar individuellt
	if(myRandom()%100>=x->movementX) {
		if(x->dx==0&&x->x<=x->width/x->cols) {	//		dra åt höger
			x->dx= 1;
		} else {
			x->dx= 0;
		}
	}
	if(myRandom()%100>=x->movementY) {
		if(x->dy==0&&x->y>=1) {								//		dra uppåt
			x->dy= -1;
		} else {
			x->dy= 0;
		}
	}
}
void variant6(t_f0ext *x) {											// 6. slumpa riktningar individuellt
	if(myRandom()%100>=x->movementX) {
		if(x->dx==0&&x->x<=x->width/x->cols) {	//		dra åt höger
			x->dx= 1;
		} else {
			x->dx= 0;
		}
	}
	if(myRandom()%100>=x->movementY) {
		if(x->dy==0&&x->y<=x->height/x->rows) {	//		dra nedåt
			x->dy= 1;
		} else {
			x->dy= 0;
		}
	}
}

//----------------------------------------------------------------------------------------------
void slumpaRiktningXY(t_f0ext *x) {
	while((x->dx==0)&&(x->dy==0)) {							// kolla att inte båda riktningar blir 0
		x->dx= (myRandom()%3)-1;
		x->dy= (myRandom()%3)-1;
	}
}
void slumpaRiktningX(t_f0ext *x) {
	x->dx= (myRandom()%3)-1;
}
void slumpaRiktningY(t_f0ext *x) {
	x->dy= (myRandom()%3)-1;
}

//----------------------------------------------------------------------------------------------
void drawFO(t_f0ext *x) {
	int i, j, iCol, jRow;
	for(i= 0; i<x->cols; i++) {
		for(j= 0; j<x->rows; j++) {
			iCol= (x->width/x->cols)*i;
			jRow= (x->height/x->rows)*j;
			switch(x->mapping) {									// kolla mapping
				case 1:
					mapping1(x, iCol, jRow);
					break;
				case 2:
					mapping2(x, iCol, jRow);
					break;
				case 3:
					mapping3(x, iCol, jRow);
					break;
				case 4:
					mapping4(x, iCol, jRow);
					break;
				case 5:
					mapping5(x, i, iCol, jRow);
					break;
				case 6:
					mapping6(x, i, iCol, jRow);
					break;
				case 7:
					mapping7(x, i, iCol, jRow);
					break;
				case 8:
					mapping8(x, j, iCol, jRow);
					break;
				case 9:
					mapping9(x, j, iCol, jRow);
					break;
				case 10:
					mapping10(x, j, iCol, jRow);
					break;
				case 11:
					mapping11(x, i, j, iCol, jRow);
					break;
				case 12:
					mapping12(x, i, j, iCol, jRow);
					break;
				case 13:
					mapping13(x, i, j, iCol, jRow);
					break;
				case 14:
					mapping14(x, i, j, iCol, jRow);
					break;
				case 15:
					mapping15(x, i, j, iCol, jRow);
					break;
				case 16:
					mapping16(x, i, j, iCol, jRow);
					break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void mapping1(t_f0ext *x, int iCol, int jRow) {										// no flip
	draw(x, x->x+iCol, x->y+jRow);
}
void mapping2(t_f0ext *x, int iCol, int jRow) {										// flip all x
	draw(x, mirrorX(x, x->x)+iCol, x->y+jRow);
}
void mapping3(t_f0ext *x, int iCol, int jRow) {										// flip all y
	draw(x, x->x+iCol, mirrorY(x, x->y)+jRow);
}
void mapping4(t_f0ext *x, int iCol, int jRow) {										// flip all xy
	draw(x, mirrorX(x, x->x)+iCol, mirrorY(x, x->y)+jRow);
}
//--
void mapping5(t_f0ext *x, int i, int iCol, int jRow) {								// flip odd col x
	if(i%2==1) {
		draw(x, mirrorX(x, x->x)+iCol, x->y+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
void mapping6(t_f0ext *x, int i, int iCol, int jRow) {								// flip odd col y
	if(i%2==1) {
		draw(x, x->x+iCol, mirrorY(x, x->y)+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
void mapping7(t_f0ext *x, int i, int iCol, int jRow) {								// flip odd col xy
	if(i%2==1) {
		draw(x, mirrorX(x, x->x)+iCol, mirrorY(x, x->y)+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
//--
void mapping8(t_f0ext *x, int j, int iCol, int jRow) {								// flip odd row x
	if(j%2==1) {
		draw(x, mirrorX(x, x->x)+iCol, x->y+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
void mapping9(t_f0ext *x, int j, int iCol, int jRow) {								// flip odd row y
	if(j%2==1) {
		draw(x, x->x+iCol, mirrorY(x, x->y)+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
void mapping10(t_f0ext *x, int j, int iCol, int jRow) {								// flip odd row xy
	if(j%2==1) {
		draw(x, mirrorX(x, x->x)+iCol, mirrorY(x, x->y)+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
//--
void mapping11(t_f0ext *x, int i, int j, int iCol, int jRow) {						// flip odd col & even row x, flip even col & odd row x
	if(i%2==1&&j%2==0) {
		draw(x, mirrorX(x, x->x)+iCol, x->y+jRow);
	} else if(i%2==0&&j%2==1) {
		draw(x, mirrorX(x, x->x)+iCol, x->y+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
void mapping12(t_f0ext *x, int i, int j, int iCol, int jRow) {						// flip odd col & even row y, flip even col & odd row y
	if(i%2==1&&j%2==0) {
		draw(x, x->x+iCol, mirrorY(x, x->y)+jRow);
	} else if(i%2==0&&j%2==1) {
		draw(x, x->x+iCol, mirrorY(x, x->y)+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
void mapping13(t_f0ext *x, int i, int j, int iCol, int jRow) {						// flip odd col & even row xy, flip even col & odd row xy
	if(i%2==1&&j%2==0) {
		draw(x, mirrorX(x, x->x)+iCol, mirrorY(x, x->y)+jRow);
	} else if(i%2==0&&j%2==1) {
		draw(x, mirrorX(x, x->x)+iCol, mirrorY(x, x->y)+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
//--
void mapping14(t_f0ext *x, int i, int j, int iCol, int jRow) {						// flip even col & even row x, flip odd col & odd row x
	if(i%2==0&&j%2==0) {
		draw(x, mirrorX(x, x->x)+iCol, x->y+jRow);
	} else if(i%2==1&&j%2==1) {
		draw(x, mirrorX(x, x->x)+iCol, x->y+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
void mapping15(t_f0ext *x, int i, int j, int iCol, int jRow) {						// flip even col & even row y, flip odd col & odd row y
	if(i%2==0&&j%2==0) {
		draw(x, x->x+iCol, mirrorY(x, x->y)+jRow);
	} else if(i%2==1&&j%2==1) {
		draw(x, x->x+iCol, mirrorY(x, x->y)+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
void mapping16(t_f0ext *x, int i, int j, int iCol, int jRow) {						// flip even col & even row xy, flip odd col & odd row xy
	if(i%2==0&&j%2==0) {
		draw(x, mirrorX(x, x->x)+iCol, mirrorY(x, x->y)+jRow);
	} else if(i%2==1&&j%2==1) {
		draw(x, mirrorX(x, x->x)+iCol, mirrorY(x, x->y)+jRow);
	} else {
		draw(x, x->x+iCol, x->y+jRow);
	}
}
int mirrorX(t_f0ext *x, int ox) {
	return round((x->width/x->cols)-ox);
}
int mirrorY(t_f0ext *x, int oy) {
	return round((x->height/x->rows)-oy);
}
short myRandom() {
	//return (short)fabs(Random());		//target specific
	return (short)rand();			//target specific
}
void draw(t_f0ext *x, int ox, int oy) {
	t_atom lista[4];
	atom_setlong(lista, ox);
	atom_setlong(lista+1, oy);
	atom_setlong(lista+2, ox+1);
	atom_setlong(lista+3, oy+1);
	outlet_list(x->out, 0L, 4, lista);
}
