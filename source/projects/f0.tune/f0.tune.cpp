//		----------------------------------------------------------
//		-- fredrik olofsson 010522 (rev.011004)					--
//		-- updated 040224 - for carbon using cw8.0				--
//		-- updated 070103 - for ub using xcode					--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- bugfix 130810 - tonesPerOctave was broken            --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	double valLeft;
	double valRight;
	double tonesPerOctave;
	double valOut;
	void *out;
} t_f0ext;

void *f0ext_new(double val);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_tonesPerOctave(t_f0ext *x, double val);
void f0ext_theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.tune", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_DEFFLOAT, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_tonesPerOctave, "tonesPerOctave", A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.tune v2.1; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(double val) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valLeft= 0.0;
        x->tonesPerOctave= 12.0;
        if(val>0.0) {
            x->valRight= val;
        } else {
            x->valRight= 440.0;
        }
        floatin(x, 1);
		x->out= floatout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "frequency (int/float)");
				break;
			case 1:
				sprintf(s, "base frequency (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "quantised frequency (float)");
	}
}
void f0ext_bang(t_f0ext *x) {
	outlet_float(x->out, x->valOut);
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
	x->valRight= val;
}
void f0ext_tonesPerOctave(t_f0ext *x, double val) {
	x->tonesPerOctave= val;
}
void f0ext_theFunction(t_f0ext *x) {
	double a, b, c= 0.0, v;
	if((x->valRight!=0.0)&&(x->tonesPerOctave!=0.0)) {
		v= pow(2.0, (1.0/x->tonesPerOctave));
		a= (log(fabs(x->valLeft))-log(fabs(x->valRight)))/log(v);
		b= round(69.0+a)-69.0;
		c= fabs(x->valRight)*pow(v, b);
	}
	x->valOut= c;
	f0ext_bang(x);
}
