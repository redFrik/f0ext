//		----------------------------------------------------------
//		-- fredrik olofsson 040226								--
//		-- converted from abstraction with the same name		--
//		-- updated 070103 - for ub using xcode					--
//		-- updated 130630 - ported to max sdk 6.1.1				--
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
void theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
	t_class *c= class_new("f0.frames_to_beats", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)f0ext_bang, "bang", 0);
	class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
	class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
	class_addmethod(c, (method)f0ext_ft2, "ft2", A_FLOAT, 0);
    class_register(CLASS_BOX, c);
	f0ext_class= c;
	post("f0.frames_to_beats v2.01-64/32bit; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valLeft= 0.0;
        x->valOut= 0.0;
        x->valMiddle= 120.0;
        x->valRight= 20.0;
        t_atom *ap;
        ap= argv;
        ap++;
        if(argc>2) {post("warning: f0.frames_to_beats %d extra argument(s)", argc-2);}
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
        x->out= floatout(x);
    }
	return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "frames (int/float)");
				break;
			case 1:
				sprintf(s, "tempo bpm (float)");
				break;
			case 2:
				sprintf(s, "fps (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "beats (float)");
	}
}
void f0ext_bang(t_f0ext *x) {
	outlet_float(x->out, x->valOut);
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
	x->valMiddle= val;
}
void f0ext_ft2(t_f0ext *x, double val) {
	x->valRight= val;
}
void theFunction(t_f0ext *x) {
	if((x->valMiddle==0.0)||(x->valRight==0.0)) {
		x->valOut= 0.0;
	} else {
		x->valOut= (x->valLeft/x->valRight)/(60.0/x->valMiddle);
	}
	f0ext_bang(x);
}
