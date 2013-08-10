//		----------------------------------------------------------
//		-- fredrik olofsson 040301								--
//		-- converted from abstraction with the same name		--
//		-- updated 060507 - added set							--
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
	double valMiddle;
	double valRight;
	double min;
	double max;
	short flag;
	void *out;
	void *out2;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_ft2(t_f0ext *x, double val);
void f0ext_factor(t_f0ext *x);
void f0ext_range(t_f0ext *x);
void f0ext_set(t_f0ext *x, double min, double max);
void f0ext_theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.auto_scale", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft2, "ft2", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_factor, "factor", 0);
    class_addmethod(c, (method)f0ext_range, "range", 0);
    class_addmethod(c, (method)f0ext_set, "set", A_FLOAT, A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.auto_scale v2.01-64/32bit; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->min= 0.0;
        x->max= 0.0;
        x->flag= 0;
        x->valMiddle= 0.0;
        x->valRight= 1.0;
        t_atom *ap;
        ap= argv;
        ap++;
        if(argc>2) {post("warning: f0.auto_scale %d extra argument(s)", argc-2);}
        switch(MIN(argc, 2)) {
            case 2:
                if(atom_gettype(ap)==A_FLOAT) {
                    x->valRight= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->valRight= (double)atom_getlong(ap);
                }
            case 1:
                ap--;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->valMiddle= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->valMiddle= (double)atom_getlong(ap);
                }
        }
        floatin(x, 2);
        floatin(x, 1);
		x->out2= listout(x);
        x->out= floatout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "values to autoscale (int/float)");
				break;
			case 1:
				sprintf(s, "low output value (int/float)");
				break;
			case 2:
				sprintf(s, "high output value (int/float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
				sprintf(s, "scaled output (float)");
				break;
			case 1:
				sprintf(s, "info (list)");
				break;
		}
	}
}
void f0ext_bang(t_f0ext *x) {
	x->min= 0.0;
	x->max= 0.0;
	x->flag= 0;
}
void f0ext_int(t_f0ext *x, long val) {
	x->valLeft= val;
	f0ext_theFunction(x);
}
void f0ext_float(t_f0ext *x, double val) {
	x->valLeft= val;
	f0ext_theFunction(x);
}
void f0ext_ft1(t_f0ext *x, double val) {
	x->valMiddle= val;
}
void f0ext_ft2(t_f0ext *x, double val) {
	x->valRight= val;
}
void f0ext_factor(t_f0ext *x) {
	double rangeIn, rangeOut;
	t_atom daList[3];
	rangeIn= fabs(x->max - x->min);
	rangeOut= fabs(x->valRight - x->valMiddle);
	atom_setlong(daList, 0);				//index for routing
	if(rangeIn==0.0) {
		atom_setfloat(daList+1, 0.0);
		atom_setfloat(daList+2, rangeOut);
	} else if(rangeOut==0.0) {
		atom_setfloat(daList+1, rangeIn);
		atom_setfloat(daList+2, 0.0);
	} else if(rangeIn<=rangeOut) {
		atom_setfloat(daList+1, 1.0);
		atom_setfloat(daList+2, 1.0/(rangeIn/rangeOut));
	} else {
		atom_setfloat(daList+1, 1.0/(rangeOut/rangeIn));
		atom_setfloat(daList+2, 1.0);
	}
	outlet_list(x->out2, 0L, 3, daList);
}
void f0ext_range(t_f0ext *x) {
	t_atom daList[3];
	atom_setlong(daList, 1);				//index for routing
	if(x->min<=x->max) {
		atom_setfloat(daList+1, x->min);
		atom_setfloat(daList+2, x->max);
	} else {
		atom_setfloat(daList+1, x->max);
		atom_setfloat(daList+2, x->min);
	}
	outlet_list(x->out2, 0L, 3, daList);
}
void f0ext_set(t_f0ext *x, double min, double max) {
	x->min= min;
	x->max= max;
}
void f0ext_theFunction(t_f0ext *x) {
	double v, o, rangeIn, rangeOut;
	v= x->valLeft;
	if((x->flag==0)&&(x->min==x->max)) {
		x->flag= 1;
		x->min= v;
		x->max= v;
	}
	if(v<x->min) {
		x->min= v;
	}
	if(v>x->max) {
		x->max= v;
	}
	rangeIn= fabs(x->max - x->min);
	rangeOut= fabs(x->valRight - x->valMiddle);
	if(rangeIn==0.0) {
		if(x->valMiddle<=x->valRight) {
			o= x->valMiddle;
		} else {
			o= x->valRight;
		}
	} else if(x->valMiddle<=x->valRight) {
		o= fabs((v-x->min)/rangeIn*rangeOut)+x->valMiddle;
	} else {
		o= fabs((v-x->max)/rangeIn*rangeOut)+x->valRight;
	}
	outlet_float(x->out, o);
}
