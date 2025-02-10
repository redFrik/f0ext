//		----------------------------------------------------------
//		-- fredrik olofsson 020115								--
//		-- updated 040225 - for carbon using cw8.0				--
//		-- updated 060507 - fix for output on right input		--
//		-- updated 070103 - for ub using xcode					--
//		-- updated 070106 - clip arg fix						--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	double valLeft;
	double valRight;
	double valOut;
	double temp;
	void *out;
} t_f0ext;

void *f0ext_new(double val);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_set(t_f0ext *x, double val);
void f0ext_theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.smooth", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_DEFFLOAT, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_set, "set", A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.smooth v2.0; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(double val) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valLeft= 0.0;
        x->valOut= 0.0;
        x->temp= 0.0;
        if(val>0.0) {
            x->valRight= CLAMP(val, 0.0, 1.0);
        } else {
            x->valRight= 0.15;
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
				sprintf(s, "values to smooth (int/float)");
				break;
			case 1:
				sprintf(s, "smoothing constant alpha (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "smoothed output (float)");
	}
}
void f0ext_bang(t_f0ext *x) {
	f0ext_theFunction(x);
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
	x->valRight= CLAMP(val, 0.0, 1.0);
}
void f0ext_set(t_f0ext *x, double val) {
	x->valOut= val;
}
void f0ext_theFunction(t_f0ext *x) {
	double St0, St1, a, Yt1;
	a= x->valRight;
	St1= x->valOut;
	Yt1= x->temp;
	St0= a*Yt1+(1.0-a)*St1;	//SES - Single Exponential Smoothing, Hunter (1986)
	x->valOut= St0;
	x->temp= x->valLeft;
	outlet_float(x->out, x->valOut);
}
