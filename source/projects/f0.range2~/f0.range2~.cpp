//		----------------------------------------------------------
//		-- fredrik olofsson 070106								--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

void *f0ext_class;

typedef struct _f0ext {
	t_pxobject ob;
    double valRight;
	double min;
	double max;
    double setMin;
	double setMax;
	short setFlag;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count);
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
t_int *f0ext_perform(t_int *w);
void f0ext_perform64(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_bang(t_f0ext *x);
void f0ext_float(t_f0ext *x, double val);
void f0ext_set(t_f0ext *x, double val1, double val2);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.range2~", (method)f0ext_new, (method)dsp_free, sizeof(t_f0ext), NULL, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp, "dsp", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_set, "set", A_FLOAT, A_FLOAT, 0);
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.range2~ v2.01; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valRight= 0.0;
        x->setMin= LONG_MAX;
        x->setMax= LONG_MIN;
        x->setFlag= MIN(argc, 3);
        t_atom *ap;
        ap= argv;
        if(argc>3) {post("warning: f0.range2~ %d extra argument(s)", argc-3);}
        switch(MIN(argc, 3)) {
            case 3:
                ap++;
                ap++;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->valRight= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->valRight= (double)atom_getlong(ap);
                }
                ap--;
                ap--;
            case 2:
                ap++;
                if(atom_gettype(ap)==A_FLOAT) {
                    x->setMax= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->setMax= (double)atom_getlong(ap);
                }
                ap--;
            case 1:
                if(atom_gettype(ap)==A_FLOAT) {
                    x->setMin= (double)atom_getfloat(ap);
                } else if(atom_gettype(ap)==A_LONG) {
                    x->setMin= (double)atom_getlong(ap);
                }
        }
        x->min= x->setMin;
        x->max= x->setMax;
        dsp_setup((t_pxobject *)x, 2);          //inlets
        outlet_new(x, "signal");                //outlet
        outlet_new(x, "signal");                //outlet
        outlet_new(x, "signal");                //outlet
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
                sprintf(s, "signal to check (signal)");
                break;
			case 1:
                sprintf(s, "smooth or fallback (float)");
                break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
                sprintf(s, "minimum (signal)");
                break;
			case 1:
                sprintf(s, "middle (signal)");
                break;
			case 2:
                sprintf(s, "maximum (signal)");
                break;
		}
	}
}
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count) {
    dsp_add(f0ext_perform, 6, sp[0]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[0]->s_n, x);
}
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
	object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64, 0, NULL);
}
t_int *f0ext_perform(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *outL= (t_float *)w[2];
	t_float *outM= (t_float *)w[3];
	t_float *outR= (t_float *)w[4];
	int n= (int)w[5];
	t_f0ext *x= (t_f0ext *)w[6];
	double right= x->valRight;
	double min= x->min+right;
	double max= x->max-right;
	double in;
	while(n--) {
		in= *inL++;
		if(in<min) min= in;
		if(in>max) max= in;
		*outL++= min;
		*outM++= (max-min)/2.0+min;
		*outR++= max;
	}
	x->min= min;
	x->max= max;
	return w+7;
}
void f0ext_perform64(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *outL= outs[0];
    t_double *outM= outs[1];
    t_double *outR= outs[2];
	double right= x->valRight;
    double min= x->min+right;
	double max= x->max-right;
	double in;
	while(sampleframes--) {
		in= *inL++;
		if(in<min) min= in;
        if(in>max) max= in;
        *outL++= min;
		*outM++= (max-min)/2.0+min;
		*outR++= max;
    }
	x->min= min;
	x->max= max;
}
void f0ext_bang(t_f0ext *x) {
	if(x->setFlag==0) {
		x->min= LONG_MAX;
		x->max= LONG_MIN;
	} else if(x->setFlag==1) {
        x->min= x->setMin;
		x->max= LONG_MIN;
    } else {
		x->min= x->setMin;
		x->max= x->setMax;
	}
}
void f0ext_float(t_f0ext *x, double val) {
	x->valRight= val;
}
void f0ext_set(t_f0ext *x, double val1, double val2) {
	if(val1<val2) {
		x->min= val1;
		x->max= val2;
		x->setMin= val1;
		x->setMax= val2;
	} else {
		x->min= val2;
		x->max= val1;
		x->setMin= val2;
		x->setMax= val1;
	}
	x->setFlag= 2;
}
