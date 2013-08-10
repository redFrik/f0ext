//		----------------------------------------------------------
//		-- fredrik olofsson 040331								--
//		-- updated 051016 - for wmax using cygwin				--
//		-- updated 070103 - for ub using xcode					--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	double valRight;
    double dummy;
} t_f0ext;

void *f0ext_new(double val);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
void f0ext_float(t_f0ext *x, double val);
void f0ext_ft1(t_f0ext *x, double val);
void f0ext_theFunction(t_f0ext *x);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.slub", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, A_DEFFLOAT, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_addmethod(c, (method)f0ext_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)f0ext_ft1, "ft1", A_FLOAT, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.slub v2.0-64/32bit; distributed under GNU GPL license");
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
void f0ext_bang(t_f0ext *x) {
	f0ext_theFunction(x);
}
void f0ext_int(t_f0ext *x, long val) {
	x->valRight= val;
	f0ext_theFunction(x);
}
void f0ext_float(t_f0ext *x, double val) {
	x->valRight= val;
	f0ext_theFunction(x);
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
