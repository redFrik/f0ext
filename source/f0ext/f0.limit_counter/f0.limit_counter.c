//		----------------------------------------------------------
//		-- fredrik olofsson 040226								--
//		-- converted from abstraction with the same name		--
//		-- updated 051016 - for wmax using cygwin				--
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
	double valCount;
	double valMin;
	double valMax;
	void *out;
	void *out2;
    void *out3;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_bang2(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_in1(t_f0ext *x, long val);
void f0ext_in2(t_f0ext *x, long val);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.limit_counter", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_bang2, "bang2", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_in1, "in1", A_LONG, 0);
    class_addmethod(c, (method)f0ext_in2, "in2", A_LONG, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.limit_counter v2.01-64/32bit; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valCount= 0.0;
        x->valMin= LONG_MIN;
        x->valMax= LONG_MAX;
        t_atom *ap;
        ap= argv;
        if(argc>2) {post("warning: f0.limit_counter %d extra argument(s)", argc-2);}
        switch(MIN(argc, 2)) {
            case 2:
                if(atom_gettype(ap)==A_FLOAT) {
                    x->valMin= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->valMin= (double)atom_getlong(ap);
                }
                ap++;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->valMax= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->valMax= (double)atom_getlong(ap);
                }
                break;
            case 1:
                x->valMin= 0.0;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->valMax= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->valMax= (double)atom_getlong(ap);
                }
        }
        intin(x, 2);
        intin(x, 1);
        inlet4(x, x, "bang", "bang2");
		x->out3= bangout(x);
        x->out2= bangout(x);
        x->out= intout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "increase (bang), set value (int)");
				break;
			case 1:
				sprintf(s, "decrease (bang)");
				break;
			case 2:
				sprintf(s, "floor value (int)");
				break;
			case 3:
				sprintf(s, "ceil value (int)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
				sprintf(s, "counter (int)");
				break;
			case 1:
				sprintf(s, "counter hits floor (bang)");
				break;
			case 2:
				sprintf(s, "counter hits ceil (bang)");
				break;
		}
	}
}
void f0ext_bang(t_f0ext *x) {
	if(x->valCount<x->valMax) {
		x->valCount= x->valCount+1;
	}
	if(x->valCount==x->valMax) {
		outlet_bang(x->out3);
	}
	outlet_int(x->out, x->valCount);
}
void f0ext_bang2(t_f0ext *x) {
	if(x->valCount>x->valMin) {
		x->valCount= x->valCount-1;
	}
	if(x->valCount==x->valMin) {
		outlet_bang(x->out2);
	}
	outlet_int(x->out, x->valCount);
}
void f0ext_int(t_f0ext *x, long val) {
	if(val<x->valMin) {
		val= x->valMin;
	} else if(val>x->valMax) {
		val= x->valMax;
	}
	x->valCount= val;
}
void f0ext_in1(t_f0ext *x, long val) {
	x->valMin= val;
	if(x->valCount<val) {
		x->valCount= val;
	}
}
void f0ext_in2(t_f0ext *x, long val) {
	x->valMax= val;
	if(x->valCount>val) {
		x->valCount= val;
	}
}
