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
	double valLeft;
	double valMiddle;
	double valRight;
	double min;
	double max;
	short flag;
	void *out2;
} t_f0ext;

void *f0ext_new(t_symbol *s, long argc, t_atom *argv);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count);
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
t_int *f0ext_perform_0(t_int *w);
t_int *f0ext_perform_1(t_int *w);
t_int *f0ext_perform_2(t_int *w);
t_int *f0ext_perform_3(t_int *w);
void f0ext_perform64_0(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_perform64_1(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_perform64_2(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_perform64_3(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_bang(t_f0ext *x);
void f0ext_float(t_f0ext *x, double val);
void f0ext_factor(t_f0ext *x);
void f0ext_range(t_f0ext *x);
void f0ext_set(t_f0ext *x, double min, double max);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.auto_scale~", (method)f0ext_new, (method)dsp_free, sizeof(t_f0ext), NULL, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp, "dsp", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_factor, "factor", 0);
    class_addmethod(c, (method)f0ext_range, "range", 0);
    class_addmethod(c, (method)f0ext_set, "set", A_FLOAT, A_FLOAT, 0);
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.auto_scale~ v2.01; distributed under GNU GPL license");
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
        dsp_setup((t_pxobject *)x, 3);          //inlets
        x->out2= listout((t_pxobject *)x);      //outlet
        outlet_new(x, "signal");                //outlet
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "signal to autoscale (signal)");
				break;
			case 1:
				sprintf(s, "low output (signal/float)");
				break;
			case 2:
				sprintf(s, "high output (signal/float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
				sprintf(s, "scaled output (signal)");
				break;
			case 1:
				sprintf(s, "info (list)");
				break;
		}
	}
}
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count) {
	if((!count[1])&&(!count[2])) {		//signal float float
		dsp_add(f0ext_perform_0, 4, sp[0]->s_vec, sp[3]->s_vec, sp[0]->s_n, x);
	} else if(!count[2]) {				//signal signal float
		dsp_add(f0ext_perform_1, 5, sp[0]->s_vec, sp[1]->s_vec, sp[3]->s_vec, sp[0]->s_n, x);
	} else if(!count[1]) {				//signal float signal
		dsp_add(f0ext_perform_2, 5, sp[0]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n, x);
	} else {							//signal signal signal
		dsp_add(f0ext_perform_3, 6, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n, x);
	}
}
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
	if((!count[1])&&(!count[2])) {		//signal float float
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_0, 0, NULL);
	} else if(!count[2]) {				//signal signal float
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_1, 0, NULL);
	} else if(!count[1]) {				//signal float signal
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_2, 0, NULL);
	} else {							//signal signal signal
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_3, 0, NULL);
	}
}
t_int *f0ext_perform_0(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *outL= (t_float *)w[2];
	int n= (int)w[3];
	t_f0ext *x= (t_f0ext *)w[4];
	double in, rangeIn, rangeOut;
	short flag= x->flag;
	double min= x->min;
	double max= x->max;
	double middle= x->valMiddle;
	double right= x->valRight;
	while(n--) {
		in= *inL++;
		if((flag==0)&&(min==max)) {flag= 1; min= in; max= in;}
		if(in<min) min= in;
		if(in>max) max= in;
		rangeIn= fabs(max-min);
		rangeOut= fabs(right-middle);
		if(rangeIn==0.0) {
			if(middle<=right) *outL++= middle;
			else *outL++= right;
		} else if(middle<=right) {
			*outL++= fabs((in-min)/rangeIn*rangeOut)+middle;
		} else {
			*outL++= fabs((in-max)/rangeIn*rangeOut)+right;
		}
	}
	x->flag= flag;
	x->min= min;
	x->max= max;
	x->valMiddle= middle;
	x->valRight= right;
	return w+5;
}
t_int *f0ext_perform_1(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *inM= (t_float *)w[2];
	t_float *outL= (t_float *)w[3];
	int n= (int)w[4];
	t_f0ext *x= (t_f0ext *)w[5];
	double in, middle, rangeIn, rangeOut;
	short flag= x->flag;
	double min= x->min;
	double max= x->max;
	double right= x->valRight;
	while(n--) {
		in= *inL++;
		middle= *inM++;
		if((flag==0)&&(min==max)) {flag= 1; min= in; max= in;}
		if(in<min) min= in;
		if(in>max) max= in;
		rangeIn= fabs(max-min);
		rangeOut= fabs(right-middle);
		if(rangeIn==0.0) {
			if(middle<=right) *outL++= middle;
			else *outL++= right;
		} else if(middle<=right) {
			*outL++= fabs((in-min)/rangeIn*rangeOut)+middle;
		} else {
			*outL++= fabs((in-max)/rangeIn*rangeOut)+right;
		}
	}
	x->flag= flag;
	x->min= min;
	x->max= max;
	x->valRight= right;
	return w+6;
}
t_int *f0ext_perform_2(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *inR= (t_float *)w[2];
	t_float *outL= (t_float *)w[3];
	int n= (int)w[4];
	t_f0ext *x= (t_f0ext *)w[5];
	double in, right, rangeIn, rangeOut;
	short flag= x->flag;
	double min= x->min;
	double max= x->max;
	double middle= x->valMiddle;
	while(n--) {
		in= *inL++;
		right= *inR++;
		if((flag==0)&&(min==max)) {flag= 1; min= in; max= in;}
		if(in<min) min= in;
		if(in>max) max= in;
		rangeIn= fabs(max-min);
		rangeOut= fabs(right-middle);
		if(rangeIn==0.0) {
			if(middle<=right) *outL++= middle;
			else *outL++= right;
		} else if(middle<=right) {
			*outL++= fabs((in-min)/rangeIn*rangeOut)+middle;
		} else {
			*outL++= fabs((in-max)/rangeIn*rangeOut)+right;
		}
	}
	x->flag= flag;
	x->min= min;
	x->max= max;
	x->valMiddle= middle;
	return w+6;
}
t_int *f0ext_perform_3(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *inM= (t_float *)w[2];
	t_float *inR= (t_float *)w[3];
	t_float *outL= (t_float *)w[4];
	int n= (int)w[5];
	t_f0ext *x= (t_f0ext *)w[6];
	double in, middle, right, rangeIn, rangeOut;
	short flag= x->flag;
	double min= x->min;
	double max= x->max;
	while(n--) {
		in= *inL++;
		middle= *inM++;
		right= *inR++;
		if((flag==0)&&(min==max)) {flag= 1; min= in; max= in;}
		if(in<min) min= in;
		if(in>max) max= in;
		rangeIn= fabs(max-min);
		rangeOut= fabs(right-middle);
		if(rangeIn==0.0) {
			if(middle<=right) *outL++= middle;
			else *outL++= right;
		} else if(middle<=right) {
			*outL++= fabs((in-min)/rangeIn*rangeOut)+middle;
		} else {
			*outL++= fabs((in-max)/rangeIn*rangeOut)+right;
		}
	}
	x->flag= flag;
	x->min= min;
	x->max= max;
	return w+7;
}
void f0ext_perform64_0(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *outL= outs[0];
	double in, rangeIn, rangeOut;
	short flag= x->flag;
	double min= x->min;
	double max= x->max;
	double middle= x->valMiddle;
	double right= x->valRight;
	while(sampleframes--) {
		in= *inL++;
		if((flag==0)&&(min==max)) {flag= 1; min= in; max= in;}
		if(in<min) min= in;
		if(in>max) max= in;
		rangeIn= fabs(max-min);
		rangeOut= fabs(right-middle);
		if(rangeIn==0.0) {
			if(middle<=right) *outL++= middle;
			else *outL++= right;
		} else if(middle<=right) {
			*outL++= fabs((in-min)/rangeIn*rangeOut)+middle;
		} else {
			*outL++= fabs((in-max)/rangeIn*rangeOut)+right;
		}
	}
	x->flag= flag;
	x->min= min;
	x->max= max;
	x->valMiddle= middle;
	x->valRight= right;
}
void f0ext_perform64_1(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *inM= ins[1];
	t_double *outL= outs[0];
	double in, middle, rangeIn, rangeOut;
	short flag= x->flag;
	double min= x->min;
	double max= x->max;
	double right= x->valRight;
	while(sampleframes--) {
		in= *inL++;
		middle= *inM++;
		if((flag==0)&&(min==max)) {flag= 1; min= in; max= in;}
		if(in<min) min= in;
		if(in>max) max= in;
		rangeIn= fabs(max-min);
		rangeOut= fabs(right-middle);
		if(rangeIn==0.0) {
			if(middle<=right) *outL++= middle;
			else *outL++= right;
		} else if(middle<=right) {
			*outL++= fabs((in-min)/rangeIn*rangeOut)+middle;
		} else {
			*outL++= fabs((in-max)/rangeIn*rangeOut)+right;
		}
	}
	x->flag= flag;
	x->min= min;
	x->max= max;
	x->valRight= right;
}
void f0ext_perform64_2(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *inR= ins[1];
	t_double *outL= outs[0];
	double in, right, rangeIn, rangeOut;
	short flag= x->flag;
	double min= x->min;
	double max= x->max;
	double middle= x->valMiddle;
	while(sampleframes--) {
		in= *inL++;
		right= *inR++;
		if((flag==0)&&(min==max)) {flag= 1; min= in; max= in;}
		if(in<min) min= in;
		if(in>max) max= in;
		rangeIn= fabs(max-min);
		rangeOut= fabs(right-middle);
		if(rangeIn==0.0) {
			if(middle<=right) *outL++= middle;
			else *outL++= right;
		} else if(middle<=right) {
			*outL++= fabs((in-min)/rangeIn*rangeOut)+middle;
		} else {
			*outL++= fabs((in-max)/rangeIn*rangeOut)+right;
		}
	}
	x->flag= flag;
	x->min= min;
	x->max= max;
	x->valMiddle= middle;
}
void f0ext_perform64_3(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *inM= ins[1];
	t_double *inR= ins[2];
	t_double *outL= outs[0];
	double in, middle, right, rangeIn, rangeOut;
	short flag= x->flag;
	double min= x->min;
	double max= x->max;
	while(sampleframes--) {
		in= *inL++;
		middle= *inM++;
		right= *inR++;
		if((flag==0)&&(min==max)) {flag= 1; min= in; max= in;}
		if(in<min) min= in;
		if(in>max) max= in;
		rangeIn= fabs(max-min);
		rangeOut= fabs(right-middle);
		if(rangeIn==0.0) {
			if(middle<=right) *outL++= middle;
			else *outL++= right;
		} else if(middle<=right) {
			*outL++= fabs((in-min)/rangeIn*rangeOut)+middle;
		} else {
			*outL++= fabs((in-max)/rangeIn*rangeOut)+right;
		}
	}
	x->flag= flag;
	x->min= min;
	x->max= max;
}
void f0ext_bang(t_f0ext *x) {
	x->min= 0;
	x->max= 0;
	x->flag= 0;
}
void f0ext_float(t_f0ext *x, double val) {
    long num= proxy_getinlet((t_object*)x);
    switch(num) {
        case 0:
            x->valLeft= val;
            break;
        case 1:
            x->valMiddle= val;
            break;
        case 2:
            x->valRight= val;
            break;
    }
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
	} else if(rangeOut==0) {
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
