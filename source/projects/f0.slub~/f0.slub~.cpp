//		----------------------------------------------------------
//		-- fredrik olofsson 040331								--
//		-- updated 051016 - for wmax using cygwin				--
//		-- updated 070103 - for ub using xcode					--
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
    double dummy;
	short flag;
} t_f0ext;

void *f0ext_new(double val);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count);
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
t_int *f0ext_perform(t_int *w);
void f0ext_perform64(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void f0ext_bang(t_f0ext *x);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.slub~", (method)f0ext_new, (method)dsp_free, sizeof(t_f0ext), NULL, A_DEFFLOAT, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp, "dsp", A_CANT, 0);
    class_addmethod(c, (method)f0ext_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.slub~ v2.0; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(double val) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        if(val==0.0) {
            x->valRight= 7.0;
        } else {
            x->valRight= val;
        }
        dsp_setup((t_pxobject *)x, 1);          //inlets
        floatin(x, 1);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		switch(a) {
			case 0:
				sprintf(s, "(bang/float)");
				break;
			case 1:
				sprintf(s, "spike amount (float)");
				break;
		}
	}
}
void f0ext_dsp(t_f0ext *x, t_signal **sp, short *count) {
	dsp_add(f0ext_perform, 1, x);
}
void f0ext_dsp64(t_f0ext *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
	object_method(dsp64, gensym("dsp_add64"), x, f0ext_perform64, 0, NULL);
}
t_int *f0ext_perform(t_int *w) {
	t_f0ext *x= (t_f0ext *)w[1];
	if(x->flag==1) {
        x->flag= 0;
        f0ext_theFunction(x);
    }
	return w+2;
}
void f0ext_perform64(t_f0ext *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	if(x->flag==1) {
        x->flag= 0;
        f0ext_theFunction(x);
    }
}
void f0ext_bang(t_f0ext *x) {
	x->flag= 1;
}
void f0ext_int(t_f0ext *x, long val) {
	x->valRight= val;
	x->flag= 1;
}
void f0ext_float(t_f0ext *x, double val) {
	x->valRight= val;
    x->flag= 1;
}
void f0ext_ft1(t_f0ext *x, double val) {
	x->valRight= val;
}
void f0ext_theFunction(t_f0ext *x) {
	long
    i
    ,
    j
	,
    k
    ,
    l
	,
    m
    ;
    m
	=
    (long)round
    (
     pow
     (
      2
      ,
      x->valRight
      )
     )
	;
    for
        (
         i
         =
         0
         ;
         i
         <
         m
         ;
         i++
         )
    {
        for
            (j=
             0
             ;
             j
             <
             m
             ;
             j++
             )
        {		for
			(
             k
             =0
             ;
             k<
             m
             ;
             k++
             )
        {
			for
                
                (
                 l
                 =
                 0;
                 l
                 <
                 m
                 ;
                 l++
                 )
			{
                x->dummy= fmod(
                               l				,
                               3.1415
                               )
                ;
            }
        }}
	}
}
