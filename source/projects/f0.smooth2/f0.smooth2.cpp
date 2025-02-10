//		----------------------------------------------------------
//		-- fredrik olofsson 020115								--
//		-- updated 040225 - for carbon using cw8.0				--
//		-- updated 060507 - fix for output on right input		--
//		-- updated 070103 - for ub using xcode					--
//		-- updated 070106 - clip arg and speed improve fix		--
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
	double valOutLeft;
    double valOutRight;
	void *outLeft;
    void *outRight;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_ft2(t_f0ext *x, double val);
void f0ext_set(t_f0ext *x, double val);
void f0ext_theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.smooth2", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft2, "ft2", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_set, "set", A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.smooth2 v2.01; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valLeft= 0.0;
        x->valOutLeft= 0.0;
        x->valOutRight= 0.0;
        x->valMiddle= 0.15;
        x->valRight= 0.3;
        t_atom *ap;
        ap= argv;
        ap++;
        if(argc>2) {post("warning: f0.smooth2 %d extra argument(s)", argc-2);}
        switch(MIN(argc, 2)) {
            case 2:
                if(atom_gettype(ap)==A_FLOAT) {
                    x->valRight= CLAMP((double)atom_getfloat(ap), 0.0, 1.0);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->valRight= CLAMP((double)atom_getlong(ap), 0.0, 1.0);
                }
            case 1:
                ap--;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->valMiddle= CLAMP((double)atom_getfloat(ap), 0.0, 1.0);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->valMiddle= CLAMP((double)atom_getlong(ap), 0.0, 1.0);
                }
        }
        floatin(x, 2);
        floatin(x, 1);
		x->outRight= floatout(x);
        x->outLeft= floatout(x);
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
            case 2:
				sprintf(s, "smoothing constant gamma (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
				sprintf(s, "smoothed output (float)");
				break;
			case 1:
				sprintf(s, "trend (float)");
				break;
		}
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
	x->valMiddle= CLAMP(val, 0.0, 1.0);
}
void f0ext_ft2(t_f0ext *x, double val) {
	x->valRight= CLAMP(val, 0.0, 1.0);
}
void f0ext_set(t_f0ext *x, double val) {
	x->valOutLeft= val;
}
void f0ext_theFunction(t_f0ext *x) {
	double St0, St1, Bt0, Bt1, a, g, Yt0;
	a= x->valMiddle;
	g= x->valRight;
	Yt0= x->valLeft;
	St1= x->valOutLeft;
	Bt1= x->valOutRight;
	St0= a*Yt0+(1.0-a)*(St1+Bt1);		//DES - Double Exponential Smoothing
	Bt0= g*(St0-St1)+(1.0-g)*Bt1;
	x->valOutLeft= St0;
	x->valOutRight= Bt0;
	outlet_float(x->outRight, x->valOutRight);
	outlet_float(x->outLeft, x->valOutLeft);
}
