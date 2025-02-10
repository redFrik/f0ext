//		----------------------------------------------------------
//		-- fredrik olofsson 070106								--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

void *f0ext_class;

typedef struct _f0ext {
	t_pxobject ob;
	double valRight;
	double prev;
} t_f0ext;

void *f0ext_new(double val);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count);
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
t_int *f0ext_perform_0(t_int *w);
t_int *f0ext_perform_1(t_int *w);
void f0ext_perform64_0(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_perform64_1(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_float(t_f0ext *x, double val);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.smooth~", (method)f0ext_new, (method)dsp_free, sizeof(t_f0ext), NULL, A_DEFFLOAT, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp, "dsp", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.smooth~ v2.0; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(double val) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->prev= 0.0;
        if(val>0.0) {
            x->valRight= CLAMP(val, 0.0, 1.0);
        } else {
            x->valRight= 0.15;
        }
        dsp_setup((t_pxobject *)x, 2);          //inlets
        outlet_new(x, "signal");  //outlet
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "signal to smooth (signal)");
				break;
			case 1:
				sprintf(s, "smoothing constant alpha (signal/float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "smoothed output (signal)");
	}
}
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count) {
	if(!count[1]) {		//signal float
		dsp_add(f0ext_perform_0, 4, sp[0]->s_vec, sp[2]->s_vec, sp[0]->s_n, x);
	} else {			//signal signal
		dsp_add(f0ext_perform_1, 5, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n, x);
	}
}
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
	if(!count[1]) {		//signal float
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_0, 0, NULL);
	} else {			//signal signal
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_1, 0, NULL);
	}
}
t_int *f0ext_perform_0(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *outL= (t_float *)w[2];
	int n= (int)w[3];
	t_f0ext *x= (t_f0ext *)w[4];
	double a= pow(x->valRight, 4.0);
	double prev= x->prev;
	while(n--) {
		prev= *outL++= (1.0-a)*prev+a*(*inL++);
	}
	x->prev= prev;
	return w+5;
}
t_int *f0ext_perform_1(t_int *w) {
    t_float *inL= (t_float *)w[1];
	t_float *inR= (t_float *)w[2];
	t_float *outL= (t_float *)w[3];
	int n= (int)w[4];
	t_f0ext *x= (t_f0ext *)w[5];
	double a, r;
	double prev= x->prev;
	while(n--) {
		r= *inR++;
		if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		a= pow(r, 4.0);
		prev= *outL++= (1.0-a)*prev+a*(*inL++);
	}
	x->prev= prev;
	return w+6;
}
void f0ext_perform64_0(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *outL= outs[0];
	double a= pow(x->valRight, 4.0);
    double prev= x->prev;
	while(sampleframes--) {
		prev= *outL++= (1.0-a)*prev+a*(*inL++);
	}
	x->prev= prev;
}
void f0ext_perform64_1(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
    t_double *inR= ins[1];
	t_double *outL= outs[0];
	double a, r;
    double prev= x->prev;
	while(sampleframes--) {
        r= *inR++;
        if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		a= pow(r, 4.0);
		prev= *outL++= (1.0-a)*prev+a*(*inL++);
	}
	x->prev= prev;
}
void f0ext_float(t_f0ext *x, double val) {
	x->valRight= CLAMP(val, 0.0, 1.0);
}
