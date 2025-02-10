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
	short dim;
	double x;
	double y;
	double z;
} t_f0ext;

void *f0ext_new(long val);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count);
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
t_int *f0ext_perform_0(t_int *w);
t_int *f0ext_perform_1(t_int *w);
t_int *f0ext_perform_2(t_int *w);
void f0ext_perform64_0(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_perform64_1(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_perform64_2(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.distance~", (method)f0ext_new, (method)dsp_free, sizeof(t_f0ext), NULL, A_DEFLONG, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp, "dsp", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp64, "dsp64", A_CANT, 0);
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.distance~ v2.0; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(long val) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->x= 0.0;
        x->y= 0.0;
        x->z= 0.0;
        x->dim= CLAMP(val, 1, 3);
        if((val<0)||(val>3)) {
            post("warning: f0.distance~ only 1, 2 or 3 dimensions");
        }
        dsp_setup((t_pxobject *)x, x->dim);   //inlets
        outlet_new(x, "signal");  //outlet
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "x (signal)");
				break;
			case 1:
				sprintf(s, "y (signal)");
				break;
			case 2:
				sprintf(s, "z (signal)");
				break;
		}
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "distance between delta (signal)");
	}
}
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count) {
	if(x->dim==1) {				//1dim signal
		dsp_add(f0ext_perform_0, 4, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n, x);
	} else if(x->dim==2) {		//2dim signal signal
		dsp_add(f0ext_perform_1, 5, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n, x);
	} else {					//3dim signal signal signal
		dsp_add(f0ext_perform_2, 6, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n, x);
	}
}
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
    if(x->dim==1) {				//1dim signal
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_0, 0, NULL);
	} else if(x->dim==2) {		//2dim signal signal
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_1, 0, NULL);
	} else {					//3dim signal signal signal
		object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64_2, 0, NULL);
	}
}
t_int *f0ext_perform_0(t_int *w) {		//1dim
	t_float *inL= (t_float *)w[1];
	t_float *outL= (t_float *)w[2];
	int n= (int)w[3];
	t_f0ext *x= (t_f0ext *)w[4];
	double xx= x->x;
	double xxx;
	while(n--) {
		xxx= *inL++;
		*outL++= fabs(xxx-xx);
		xx= xxx;
	}
	x->x= xx;
	return w+5;
}
t_int *f0ext_perform_1(t_int *w) {		//2dim
	t_float *inL= (t_float *)w[1];
	t_float *inM= (t_float *)w[2];
	t_float *outL= (t_float *)w[3];
	int n= (int)w[4];
	t_f0ext *x= (t_f0ext *)w[5];
	double xx= x->x;
	double yy= x->y;
	double xxx, yyy;
	while(n--) {
		xxx= *inL++;
		yyy= *inM++;
		*outL++= fabs(sqrt(pow(xxx-xx, 2)+pow(yyy-yy, 2)));
		xx= xxx;
		yy= yyy;
	}
	x->x= xx;
	x->y= yy;
	return w+6;
}
t_int *f0ext_perform_2(t_int *w) {		//3dim
	t_float *inL= (t_float *)w[1];
	t_float *inM= (t_float *)w[2];
	t_float *inR= (t_float *)w[3];
	t_float *outL= (t_float *)w[4];
	int n= (int)w[5];
	t_f0ext *x= (t_f0ext *)w[6];
	double xx= x->x;
	double yy= x->y;
	double zz= x->z;
	double xxx, yyy, zzz;
	while(n--) {
		xxx= *inL++;
		yyy= *inM++;
		zzz= *inR++;
		*outL++= fabs(sqrt(pow(xxx-xx, 2)+pow(yyy-yy, 2)+pow(zzz-zz, 2)));
		xx= xxx;
		yy= yyy;
		zz= zzz;
	}
	x->x= xx;
	x->y= yy;
	x->z= zz;
	return w+7;
}
void f0ext_perform64_0(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *outL= outs[0];
	double xx= x->x;
	double xxx;
	while(sampleframes--) {
		xxx= *inL++;
        *outL++= fabs(xxx-xx);
		xx= xxx;
	}
	x->x= xx;
}
void f0ext_perform64_1(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *inM= ins[1];
	t_double *outL= outs[0];
	double xx= x->x;
    double yy= x->y;
    double xxx, yyy;
	while(sampleframes--) {
		xxx= *inL++;
		yyy= *inM++;
        *outL++= fabs(sqrt(pow(xxx-xx, 2)+pow(yyy-yy, 2)));
        xx= xxx;
        yy= yyy;
	}
    x->x= xx;
    x->y= yy;
}
void f0ext_perform64_2(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	t_double *inL= ins[0];
	t_double *inM= ins[1];
    t_double *inR= ins[2];
	t_double *outL= outs[0];
	double xx= x->x;
    double yy= x->y;
    double zz= x->z;
    double xxx, yyy, zzz;
	while(sampleframes--) {
		xxx= *inL++;
		yyy= *inM++;
        zzz= *inR++;
        *outL++= fabs(sqrt(pow(xxx-xx, 2)+pow(yyy-yy, 2)+pow(zzz-zz, 2)));
        xx= xxx;
        yy= yyy;
        zz= zzz;
	}
    x->x= xx;
    x->y= yy;
    x->z= zz;
}
