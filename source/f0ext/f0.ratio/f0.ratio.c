//		----------------------------------------------------------
//		-- fredrik olofsson 040301								--
//		-- converted from abstraction with the same name		--
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
	double valRight;
	double proportionLeft;
	double proportionRight;
	void *out;
	void *out2;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_set(t_f0ext *x, double val1, double val2);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.ratio", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_set, "set", A_FLOAT, A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.ratio v2.01-64/32bit; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valLeft= 0.0;
        x->proportionLeft= 1.0;
        x->proportionRight= 1.0;
        t_atom *ap;
        ap= argv;
        ap++;
        if(argc>2) {post("warning: f0.ratio %d extra argument(s)", argc-2);}
        switch(MIN(argc, 2)) {
            case 2:
                if(atom_gettype(ap)==A_FLOAT) {
                    x->proportionRight= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->proportionRight= (double)atom_getlong(ap);
                }
            case 1:
                ap--;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->proportionLeft= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->proportionLeft= (double)atom_getlong(ap);
                }
        }
        floatin(x, 1);
		x->out2= floatout(x);
        x->out= floatout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "width (int/float)");
				break;
			case 1:
				sprintf(s, "height (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
				sprintf(s, "width out (float)");
				break;
			case 1:
				sprintf(s, "height out (float)");
				break;
		}
	}
}
void f0ext_bang(t_f0ext *x) {
	outlet_float(x->out2, x->valRight);
	outlet_float(x->out, x->valLeft);
}
void f0ext_int(t_f0ext *x, long val) {
	x->valLeft= val;
	x->valRight= (val/x->proportionLeft)*x->proportionRight;
	f0ext_bang(x);
}
void f0ext_float(t_f0ext *x, double val) {
	x->valLeft= val;
    x->valRight= (val/x->proportionLeft)*x->proportionRight;
	f0ext_bang(x);
}
void f0ext_ft1(t_f0ext *x, double val) {
	x->valLeft= (val/x->proportionRight)*x->proportionLeft;
	x->valRight= val;
	f0ext_bang(x);
}
void f0ext_set(t_f0ext *x, double val1, double val2) {
	if(val1==0.0) {
		x->proportionLeft= 1.0;
	} else {
		x->proportionLeft= val1;
	}
	if(val2==0.0) {
		x->proportionRight= 1.0;
	} else {
		x->proportionRight= val2;
	}
}
