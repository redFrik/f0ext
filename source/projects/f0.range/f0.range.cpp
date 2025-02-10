//		----------------------------------------------------------
//		-- fredrik olofsson 040329 (rev.060511)					--
//		-- updated 070103 - for ub using xcode					--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	double valLeft;
	double min;
	double max;
	double setMin;
	double setMax;
	short setFlag;
	void *out;
	void *out2;
	void *out3;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_set(t_f0ext *x, double val1, double val2);
void f0ext_theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.range", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_set, "set", A_FLOAT, A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.range v2.01; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->setMin= LONG_MAX;
        x->setMax= LONG_MIN;
        x->setFlag= MIN(argc, 2);
        t_atom *ap;
        ap= argv;
        ap++;
        if(argc>2) {post("warning: f0.range %d extra argument(s)", argc-2);}
        switch(MIN(argc, 2)) {
            case 2:
                if(atom_gettype(ap)==A_FLOAT) {
                    x->setMax= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->setMax= (double)atom_getlong(ap);
                }
            case 1:
                ap--;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->setMin= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->setMin= (double)atom_getlong(ap);
                }
        }
        x->min= x->setMin;
        x->max= x->setMax;
        x->out3= floatout(x);
		x->out2= floatout(x);
        x->out= floatout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		sprintf(s, "values to check (int/float)");
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
				sprintf(s, "minimum value (float)");
				break;
			case 1:
				sprintf(s, "middle value (float)");
				break;
            case 2:
                sprintf(s, "maximum value (float)");
                break;
		}
	}
}
void f0ext_bang(t_f0ext *x) {
	if(x->setFlag==0) {
		x->min= LONG_MAX;
		x->max= LONG_MIN;
	} else if(x->setFlag==1) {
		x->min= x->setMin;
		x->max= LONG_MIN;
	} else {
		x->min= x->setMin;
		x->max= x->setMax;
	}
}
void f0ext_int(t_f0ext *x, long val) {
	x->valLeft= val;
	f0ext_theFunction(x);
}
void f0ext_float(t_f0ext *x, double val) {
	x->valLeft= val;
	f0ext_theFunction(x);
}
void f0ext_set(t_f0ext *x, double val1, double val2) {
	if(val1<val2) {
		x->min= val1;
		x->max= val2;
		x->setMin= val1;
		x->setMax= val2;
	} else {
		x->min= val2;
		x->max= val1;
		x->setMin= val2;
		x->setMax= val1;
	}
	x->setFlag= 2;
}
void f0ext_theFunction(t_f0ext *x) {
	if(x->valLeft>x->max) {
		x->max= x->valLeft;
	}
	if(x->valLeft<x->min) {
		x->min= x->valLeft;
	}
	outlet_float(x->out3, x->max);
	outlet_float(x->out2, (x->max-x->min)/2.0+x->min);
	outlet_float(x->out, x->min);
}
