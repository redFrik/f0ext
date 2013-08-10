//		----------------------------------------------------------
//		-- fredrik olofsson 060830								--
//		-- updated 070103 - for ub using xcode					--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	short intswitch;
	double valIn;
	double valMin;
	double valMax;
	double valOut;
	void *out;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_ft2(t_f0ext *x, double val);
double theFunction(double in, double min, double max, short intswitch);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.wrap", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft2, "ft2", A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.wrap v2.01-64/32bit; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valIn= 0.0;
        x->intswitch= 1;
        x->valMin= 0.0;
        x->valMax= 100.0;
        if(argc>2) {post("warning: f0.wrap %d extra argument(s)", argc-2);}
        t_atom *ap;
        ap= argv;
        if(argc==1) {
            if(atom_gettype(ap)==A_FLOAT) {
                x->intswitch= 0;
                x->valMax= atom_getfloat(ap);
            } else if(atom_gettype(ap)==A_LONG) {
                x->valMax= atom_getlong(ap);
            }
        }
        if(argc>=2) {
            if(atom_gettype(ap)==A_FLOAT) {
                x->intswitch= 0;
                x->valMin= atom_getfloat(ap);
            } else if(atom_gettype(ap)==A_LONG) {
                x->valMin= atom_getlong(ap);
            }
            ap++;
            if(atom_gettype(ap)==A_FLOAT) {
                x->intswitch= 0;
                x->valMax= atom_getfloat(ap);
            } else if(atom_gettype(ap)==A_LONG) {
                x->valMax= atom_getlong(ap);
            }
        }
        floatin(x, 2);
        floatin(x, 1);
		x->out= floatout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "number to wrap (int/float)");
				break;
			case 1:
				sprintf(s, "minimum (float)");
				break;
			case 2:
				sprintf(s, "maximum (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "wrapped number (float)");
	}
}
void f0ext_bang(t_f0ext *x) {
	x->valOut= theFunction(x->valIn, x->valMin, x->valMax, x->intswitch);
	outlet_float(x->out, x->valOut);
}
void f0ext_int(t_f0ext *x, long val) {
	x->valIn= val;
	f0ext_bang(x);
}
void f0ext_float(t_f0ext *x, double val) {
	x->valIn= val;
	f0ext_bang(x);
}
void f0ext_ft1(t_f0ext *x, double val) {
	x->valMin= val;
}
void f0ext_ft2(t_f0ext *x, double val) {
	x->valMax= val;
}
double theFunction(double in, double min, double max, short intswitch) {
	double a, b;
	if(min>max) {
		a= min;
		min= max;
		max= a;
	}
	if(((in>=min)&&(in<=max))||(min==max)) {
		a= in;
	} else {
		b= fabs(max-min);
		if(in<min) {
			if(intswitch==1) {
				a= max-fabs(fmod(in-min+1.0, b+1.0));	//int
			} else {
				a= max-fabs(fmod(in-min, b));           //float
			}
		} else {
			if(intswitch==1) {
				a= min+fabs(fmod(in-1.0-max, b+1.0));	//int
			} else {
				a= min+fabs(fmod(in-max, b));           //float
			}
		}
	}
	return a;
}
