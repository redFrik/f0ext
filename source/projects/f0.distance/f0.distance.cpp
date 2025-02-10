//		----------------------------------------------------------
//		-- fredrik olofsson 010522 (rev.011004)					--
//		-- updated 040225 - for carbon using cw8.0				--
//		-- updated 051016 - for wmax using cygwin				--
//		-- updated 070103 - for ub using xcode					--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

#define MAXSIZE 3

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
    t_atom list[MAXSIZE];
    long size;
	double x1;
    double y1;
    double z1;
    double x2;
    double y2;
    double z2;
    double valOut;
	void *out;
} t_f0ext;

void *f0ext_new(long val);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_ft2(t_f0ext *x, double val);
void f0ext_list(t_f0ext *x, t_symbol *s, long argc, t_atom *argv);
void beraknaAvstand(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.distance", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_DEFLONG, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft2, "ft2", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_list, "list", A_GIMME, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.distance v2.0; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(long val) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->x1= 0.0;
        x->y1= 0.0;
        x->z1= 0.0;
        x->x2= 0.0;
        x->y2= 0.0;
        x->z2= 0.0;
        if(val==2) {
            floatin(x, 1);
        } else if(val==3) {
            floatin(x, 2);
            floatin(x, 1);
        }
        x->out= floatout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "x (float)");
				break;
			case 1:
				sprintf(s, "y (float)");
				break;
			case 2:
				sprintf(s, "z (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "distance between delta (float)");
	}
}
void f0ext_bang(t_f0ext *x) {
	beraknaAvstand(x);
	outlet_float(x->out, x->valOut);
}
void f0ext_int(t_f0ext *x, long val) {
	x->x1= val;
	f0ext_bang(x);
}
void f0ext_float(t_f0ext *x, double val) {
	x->x1= val;
	f0ext_bang(x);
}
void f0ext_ft1(t_f0ext *x, double val) {
	x->y1= val;
}
void f0ext_ft2(t_f0ext *x, double val) {
	x->z1= val;
}
void f0ext_list(t_f0ext *x, t_symbol *s, long argc, t_atom *argv) {
	if(argc>MAXSIZE) {
		argc= MAXSIZE;
	}
    long i;
    t_atom *ap;
	for(i= 0, ap= argv; i<argc; i++, ap++) {
		if(atom_gettype(ap)==A_LONG) {
			switch(i) {
				case 0:
					x->x1= atom_getlong(ap);
					break;
				case 1:
					x->y1= atom_getlong(ap);
					break;
				case 2:
					x->z1= atom_getlong(ap);
					break;
			}
		} else if(atom_gettype(ap)==A_FLOAT) {
			switch(i) {
				case 0:
					x->x1= atom_getfloat(ap);
					break;
				case 1:
					x->y1= atom_getfloat(ap);
					break;
				case 2:
					x->z1 = atom_getfloat(ap);
					break;
			}
        }
	}
	f0ext_bang(x);
}
void beraknaAvstand(t_f0ext *x) {
	double xx, yy, zz;
	xx= x->x1-x->x2;
	yy= x->y1-x->y2;
	zz= x->z1-x->z2;
	x->valOut= fabs(sqrt(pow(xx, 2)+pow(yy, 2)+pow(zz, 2)));
	x->x2= x->x1;
	x->y2= x->y1;
	x->z2= x->z1;
}
