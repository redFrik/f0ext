//		----------------------------------------------------------
//		-- fredrik olofsson 091221								--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	double val;
	double step;
	short loop;
	double min;
	double max;
	void *out;
	void *out2;
    void *out3;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_step(t_f0ext *x, double val);
void f0ext_loop(t_f0ext *x, short val);
void f0ext_reset_next(t_f0ext *x, double val);
void f0ext_reset_now(t_f0ext *x, double val);
void f0ext_min(t_f0ext *x, double min);
void f0ext_max(t_f0ext *x, double max);
double clipFunction(t_f0ext *x, double in, double min, double max);
double wrapFunction(t_f0ext *x, double in, double min, double max);
double foldFunction(t_f0ext *x, double in, double min, double max);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.ultimate_counter", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_step, "ft1", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_loop, "in2", A_LONG, 0);
    class_addmethod(c, (method)f0ext_reset_next, "ft3", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_reset_now, "ft4", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_min, "ft5", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_max, "ft6", A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.ultimate_counter v2.01; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->step= 1.0;
        x->loop= 0;
        x->min= LONG_MIN;
        x->max = LONG_MAX;
        x->val= 0.0;
        t_atom *ap;
        ap= argv;
        if(argc>4) {post("warning: f0.ultimate_counter %d extra argument(s)", argc-4);}
        switch(MIN(argc, 4)) {
            case 4:
                ap++;
                ap++;
                ap++;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->max= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->max= (double)atom_getlong(ap);
                }
                ap--;
                ap--;
                ap--;
            case 3:
                ap++;
                ap++;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->min= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->min= (double)atom_getlong(ap);
                }
                ap--;
                ap--;
                x->val= x->min;
            case 2:
                ap++;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->loop= (short)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->loop= (short)atom_getlong(ap);
                }
                ap--;
            case 1:
                if(atom_gettype(ap)==A_FLOAT) {
                    x->step= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->step= (double)atom_getlong(ap);
                }
        }
        floatin(x, 6);
        floatin(x, 5);
        floatin(x, 4);
        floatin(x, 3);
        intin(x, 2);
        floatin(x, 1);
        x->out3= bangout(x);
		x->out2= bangout(x);
        x->out= floatout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "inc/dec (bang), set value (float)");
				break;
			case 1:
				sprintf(s, "set step (float)");
				break;
			case 2:
				sprintf(s, "0= limit, 1= loop, 2= palindrome");
				break;
			case 3:
				sprintf(s, "resets counter to number on next clock");
				break;
			case 4:
				sprintf(s, "resets counter to number immediately");
				break;
			case 5:
				sprintf(s, "floor value (float)");
				break;
			case 6:
				sprintf(s, "ceil value (float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
				sprintf(s, "counter (float)");
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
	switch(x->loop) {
		case 1:
			x->val= wrapFunction(x, x->val, x->min, x->max);
			break;
		case 2:
			x->val= foldFunction(x, x->val, x->min, x->max);
			break;
		default:
			x->val= clipFunction(x, x->val, x->min, x->max);
			break;
	}
	outlet_float(x->out, x->val);
	x->val = x->val+x->step;
}
void f0ext_int(t_f0ext *x, long val) {
	if(x->min<x->max) {
        x->val= CLAMP(val, x->min, x->max);
    } else {
        x->val= CLAMP(val, x->max, x->min);
    }
}
void f0ext_float(t_f0ext *x, double val) {
    if(x->min<x->max) {
        x->val= CLAMP(val, x->min, x->max);
    } else {
        x->val= CLAMP(val, x->max, x->min);
    }
}
void f0ext_step(t_f0ext *x, double val) {
	x->step= val;
}
void f0ext_loop(t_f0ext *x, short val) {
	x->loop= val;
}
void f0ext_reset_next(t_f0ext *x, double val) {
	x->val= val;
}
void f0ext_reset_now(t_f0ext *x, double val) {
	x->val= clipFunction(x, val, x->min, x->max);
	outlet_float(x->out, x->val);
}
void f0ext_min(t_f0ext *x, double min) {
	x->min= min;
}
void f0ext_max(t_f0ext *x, double max) {
	x->max= max;
}
double clipFunction(t_f0ext *x, double in, double min, double max) {
	double a;
	if(min>max) {
		a= min;
		min= max;
		max= a;
	}
	if(in>max) {
		outlet_bang(x->out3);
		a= max;
	} else if(in<min) {
		outlet_bang(x->out2);
		a= min;
	} else {
		a= in;
	}
	return a;
}
double wrapFunction(t_f0ext *x, double in, double min, double max) {
	double a, b;
	if(min>max) {
		a= min;
		min= max;
		max= a;
	}
	if(((in>=min)&&(in<=max))||(min==max)) {
		a= in;
	} else {
		b= fabs(max-min);
		if(in<min) {
			outlet_bang(x->out2);
			a= max-fabs(fmod(in-min, b));
		} else {
			outlet_bang(x->out3);
			a= min+fabs(fmod(in-max, b));
		}
	}
	return a;
}
double foldFunction(t_f0ext *x, double in, double min, double max) {
	double a, b, c;
	if(min>max) {
		a= min;
		min= max;
		max= a;
	}
	if(((in>=min)&&(in<=max))||(min==max)) {
		b= in;
	} else {
		x->step= 0.0-x->step;
		c= fabs(max-min)*2.0;
		if(in<min) {
			outlet_bang(x->out2);
			a= min-fmod(in-min, c);
			if((a>=min)&&(a<=max)) {
				b= a;
			} else {
				b= max+(max-a);
			}
		} else {
			outlet_bang(x->out3);
			a= max-fmod(in-max, c);
			if((a>(min-c/2.0))&&(a<=min)) {
				b= min+(min-a);
			} else {
				b= a;
			}
		}
	}
	return b;
}
