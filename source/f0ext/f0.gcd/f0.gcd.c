//		----------------------------------------------------------
//		-- fredrik olofsson 010505 (rev.011004)					--
//		-- updated 040225 - for carbon using cw8.0				--
//		-- divide by 0 bugfix 060507 (051228)					--
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
	long valRight;
	long valOut;
	void *out;
} t_f0ext;

void *f0ext_new();
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_list(t_f0ext *x, t_symbol *s, long argc, t_atom *argv);
long greatestCommonDivisor(long a, long b);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.gcd", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_list, "list", A_GIMME, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.gcd v2.0-64/32bit; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new() {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valLeft= 0;
        x->valRight= 0;
        floatin(x, 1);
		x->out= intout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "number (int)");
				break;
			case 1:
				sprintf(s, "number (int)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "greatest common divisor (int)");
	}
}
void f0ext_bang(t_f0ext *x) {
	outlet_int(x->out, x->valOut);
}
void f0ext_int(t_f0ext *x, long val) {
	x->valLeft= val;
	x->valOut= greatestCommonDivisor(x->valLeft, x->valRight);
	f0ext_bang(x);
}
void f0ext_float(t_f0ext *x, double val) {
	x->valLeft= (long)val;
	x->valOut= greatestCommonDivisor(x->valLeft, x->valRight);
    f0ext_bang(x);
}
void f0ext_ft1(t_f0ext *x, double val) {
	x->valRight= (long)val;
    x->valOut= greatestCommonDivisor(x->valLeft, x->valRight);
}
void f0ext_list(t_f0ext *x, t_symbol *s, long argc, t_atom *argv) {
	long a= 0, b= 0;
	long i;
    t_atom *ap;
    ap= argv;
	if(atom_gettype(ap)==A_LONG) {
		a= atom_getlong(ap);
	} else if(atom_gettype(ap)==A_FLOAT) {
		a= (long)atom_getfloat(ap);
	}
	ap++;
	for(i= 1; i<argc; i++, ap++) {
		if(atom_gettype(ap)==A_LONG) {
			b= atom_getlong(ap);
		} else if(atom_gettype(ap)==A_FLOAT) {
			b= (long)atom_getfloat(ap);
		}
		a= greatestCommonDivisor(a, b);
	}
	x->valOut= a;
	f0ext_bang(x);
}
long greatestCommonDivisor(long a, long b) {
	if(b==0) {
		return 0;
	}
	if(a%b!=0) {
		return greatestCommonDivisor(b, a % b);
	} else {
		return fabs(b);
	}
}
