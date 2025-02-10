//		----------------------------------------------------------
//		-- fredrik olofsson 040226								--
//		-- converted from abstraction with the same name		--
//		-- well, almost the same name: f0.inc/dec_split			--
//		-- updated 051016 - for wmax using cygwin				--
//		-- updated 070103 - for ub using xcode					--
//		-- updated 081104 - bugfix for repeating values - v1.1	--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - simplified argument handling in new --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	double valLeft;
	double valLast;
	double valDefault;
	short flag;
	void *out;
	void *out2;
} t_f0ext;

void *f0ext_new(double val);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_reset(t_f0ext *x);
void theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.inc_dec_split", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_DEFFLOAT, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_reset, "reset", 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.inc_dec_split v2.01; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(double val) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valLeft= 0.0;
        x->valLast= val;
        x->valDefault= x->valLast;
        x->flag= 0;
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
				sprintf(s, "values (float)");
				break;
			case 1:
				sprintf(s, "start value (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
				sprintf(s, "increasing values (float)");
				break;
			case 1:
				sprintf(s, "decreasing values (float)");
				break;
		}
	}
}
void f0ext_bang(t_f0ext *x) {
	theFunction(x);
}
void f0ext_int(t_f0ext *x, long val) {
	x->valLeft= val;
	theFunction(x);
}
void f0ext_float(t_f0ext *x, double val) {
	x->valLeft= val;
	theFunction(x);
}
void f0ext_ft1(t_f0ext *x, double val) {
	x->valLast= val;
}
void f0ext_reset(t_f0ext *x) {
	x->valLast= x->valDefault;
}
void theFunction(t_f0ext *x) {
	if(x->valLeft>x->valLast) {
		outlet_float(x->out, x->valLeft);
		x->flag= 0;
		x->valLast= x->valLeft;
	} else if(x->valLeft<x->valLast) {
		outlet_float(x->out2, x->valLeft);
		x->flag= 1;
		x->valLast= x->valLeft;
	} else {
		if(x->flag==0) {
			outlet_float(x->out, x->valLeft);
		} else {
			outlet_float(x->out2, x->valLeft);
		}
	}
}
