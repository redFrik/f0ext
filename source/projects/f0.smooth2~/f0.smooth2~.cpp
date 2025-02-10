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
    double valMiddle;
	double valRight;
	double prev;
    double prevTrend;
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
void f0ext_float(t_f0ext *x, double val);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.smooth2~", (method)f0ext_new, (method)dsp_free, sizeof(t_f0ext), NULL, A_GIMME, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp, "dsp", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.smooth2~ v2.01; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(t_symbol *s, long argc, t_atom *argv) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->prev= 0.0;
        x->prevTrend= 0.0;
        x->valMiddle= 0.15;
        x->valRight= 0.3;
        t_atom *ap;
        ap= argv;
        ap++;
        if(argc>2) {post("warning: f0.smooth2~ %d extra argument(s)", argc-2);}
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
        dsp_setup((t_pxobject *)x, 3);          //inlets
        outlet_new(x, "signal");                //outlet
        outlet_new(x, "signal");                //outlet
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
            case 2:
				sprintf(s, "smoothing constant gamma (signal/float)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		switch(a) {
			case 0:
                sprintf(s, "smoothed output (signal)");
                break;
			case 1:
                sprintf(s, "trend (signal)");
                break;
		}
	}
}
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count) {
	if((!count[1])&&(!count[2])) {		//signal float float
		dsp_add(f0ext_perform_0, 5, sp[0]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[0]->s_n, x);
	} else if(!count[2]) {				//signal signal float
		dsp_add(f0ext_perform_1, 6, sp[0]->s_vec, sp[1]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[0]->s_n, x);
	} else if(!count[1]) {				//signal float signal
		dsp_add(f0ext_perform_2, 6, sp[0]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[0]->s_n, x);
	} else {
		dsp_add(f0ext_perform_3, 7, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[0]->s_n, x);
	}
}
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
    if((!count[1])&&(!count[2])) {		//signal float float
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_0, 0, NULL);
	} else if(!count[2]) {				//signal signal float
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_1, 0, NULL);
	} else if(!count[1]) {				//signal float signal
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_2, 0, NULL);
	} else {
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_3, 0, NULL);
	}
}
t_int *f0ext_perform_0(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *outL= (t_float *)w[2];
	t_float *outR= (t_float *)w[3];
	int n= (int)w[4];
	t_f0ext *x= (t_f0ext *)w[5];
	double a= pow(x->valMiddle, 4.0);
	double g= pow(x->valRight, 4.0);
	double prev= x->prev;
	double prevTrend= x->prevTrend;
	double temp;
	while(n--) {
		temp= prev;
		prev= *outL++= a*(*inL++)+(1.0-a)*(temp+prevTrend);
		prevTrend= *outR++= g*(prev-temp)+(1.0-g)*prevTrend;
	}
	x->prev= prev;
	x->prevTrend= prevTrend;
	return w+6;
}
t_int *f0ext_perform_1(t_int *w) {
    t_float *inL= (t_float *)w[1];
	t_float *inM= (t_float *)w[2];
	t_float *outL= (t_float *)w[3];
	t_float *outR= (t_float *)w[4];
	int n= (int)w[5];
	t_f0ext *x= (t_f0ext *)w[6];
	double a, r;
	double g= pow(x->valRight, 4.0);
	double prev= x->prev;
	double prevTrend= x->prevTrend;
	double temp;
	while(n--) {
		r= *inM++;
		if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		a= pow(r, 4.0);
		temp= prev;
		prev= *outL++= a*(*inL++)+(1.0-a)*(temp+prevTrend);
		prevTrend= *outR++= g*(prev-temp)+(1.0-g)*prevTrend;
	}
	x->prev= prev;
	x->prevTrend= prevTrend;
	return w+7;
}
t_int *f0ext_perform_2(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *inR= (t_float *)w[2];
	t_float *outL= (t_float *)w[3];
	t_float *outR= (t_float *)w[4];
	int n= (int)w[5];
	t_f0ext *x= (t_f0ext *)w[6];
	double g, r;
	double a= pow(x->valMiddle, 4.0);
	double prev= x->prev;
	double prevTrend= x->prevTrend;
	double temp;
	while(n--) {
		r= *inR++;
		if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		g= pow(r, 4.0);
		temp= prev;
		prev= *outL++= a*(*inL++)+(1.0-a)*(temp+prevTrend);
		prevTrend= *outR++= g*(prev-temp)+(1.0-g)*prevTrend;
	}
	x->prev= prev;
	x->prevTrend= prevTrend;
	return w+7;
}
t_int *f0ext_perform_3(t_int *w) {
	t_float *inL= (t_float *)w[1];
	t_float *inM= (t_float *)w[2];
	t_float *inR= (t_float *)w[3];
	t_float *outL= (t_float *)w[4];
	t_float *outR= (t_float *)w[5];
	int n= (int)w[6];
	t_f0ext *x= (t_f0ext *)w[7];
	double a, g, r;
	double prev= x->prev;
	double prevTrend= x->prevTrend;
	double temp;
	while(n--) {
		r= *inM++;
		if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		a= pow(r, 4.0);
		r= *inR++;
		if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		g= pow(r, 4.0);
		temp= prev;
		prev= *outL++= a*(*inL++)+(1.0-a)*(temp+prevTrend);
		prevTrend= *outR++= g*(prev-temp)+(1.0-g)*prevTrend;
	}
	x->prev= prev;
	x->prevTrend= prevTrend;
	return w+8;
}
void f0ext_perform64_0(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *outL= outs[0];
    t_double *outR= outs[1];
	double a= pow(x->valMiddle, 4.0);
    double g= pow(x->valRight, 4.0);
    double prev= x->prev;
    double prevTrend= x->prevTrend;
    double temp;
	while(sampleframes--) {
		temp= prev;
		prev= *outL++= a*(*inL++)+(1.0-a)*(temp+prevTrend);
		prevTrend= *outR++= g*(prev-temp)+(1.0-g)*prevTrend;
	}
	x->prev= prev;
    x->prevTrend= prevTrend;
}
void f0ext_perform64_1(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
    t_double *inM= ins[1];
	t_double *outL= outs[0];
    t_double *outR= outs[1];
	double a, r;
    double g= pow(x->valRight, 4.0);
    double prev= x->prev;
    double prevTrend= x->prevTrend;
    double temp;
	while(sampleframes--) {
        r= *inM++;
        if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		a= pow(r, 4.0);
        temp= prev;
		prev= *outL++= a*(*inL++)+(1.0-a)*(temp+prevTrend);
		prevTrend= *outR++= g*(prev-temp)+(1.0-g)*prevTrend;
	}
	x->prev= prev;
    x->prevTrend= prevTrend;
}
void f0ext_perform64_2(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
    t_double *inR= ins[1];
	t_double *outL= outs[0];
    t_double *outR= outs[1];
	double g, r;
    double a= pow(x->valMiddle, 4.0);
	double prev= x->prev;
	double prevTrend= x->prevTrend;
	double temp;
	while(sampleframes--) {
		r= *inR++;
		if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		g= pow(r, 4.0);
		temp= prev;
		prev= *outL++= a*(*inL++)+(1.0-a)*(temp+prevTrend);
		prevTrend= *outR++= g*(prev-temp)+(1.0-g)*prevTrend;
	}
	x->prev= prev;
	x->prevTrend= prevTrend;
}
void f0ext_perform64_3(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
    t_double *inM= ins[1];
    t_double *inR= ins[2];
	t_double *outL= outs[0];
    t_double *outR= outs[1];
	double a, g, r;
	double prev= x->prev;
	double prevTrend= x->prevTrend;
	double temp;
	while(sampleframes--) {
		r= *inM++;
		if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		a= pow(r, 4.0);
		r= *inR++;
		if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		g= pow(r, 4.0);
		temp= prev;
		prev= *outL++= a*(*inL++)+(1.0-a)*(temp+prevTrend);
		prevTrend= *outR++= g*(prev-temp)+(1.0-g)*prevTrend;
	}
	x->prev= prev;
	x->prevTrend= prevTrend;
}
void f0ext_float(t_f0ext *x, double val) {
	x->valRight= CLAMP(val, 0.0, 1.0);
}
