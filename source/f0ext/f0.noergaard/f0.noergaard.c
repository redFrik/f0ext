//		----------------------------------------------------------
//		-- fredrik olofsson 010614 (rev.011004)					--
//		-- updated 040225 - for carbon using cw8.0				--
//		-- updated 060507 - minor updates						--
//		-- updated 070103 - for ub using xcode					--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//		-- distributed under GNU GPL license					--
//		----------------------------------------------------------

#include "ext.h"
#include "ext_obex.h"

void *f0ext_class;

typedef struct _f0ext {
	t_object ob;
	long valOut;
	void *out;
} t_f0ext;

void *f0ext_new(void);
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s);
void f0ext_bang(t_f0ext *x);
void f0ext_int(t_f0ext *x, long val);
long f0ext_theFunction(long index);
long count_bits(long n);

int C74_EXPORT main(void) {
    t_class *c= class_new("f0.noergaard", (method)f0ext_new, (method)NULL, (long)sizeof(t_f0ext), 0L, 0);
    class_addmethod(c, (method)f0ext_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)f0ext_bang, "bang", 0);
    class_addmethod(c, (method)f0ext_int, "int", A_LONG, 0);
    class_register(CLASS_BOX, c);
    f0ext_class= c;
    post("f0.noergaard v2.0-64/32bit; distributed under GNU GPL license");
    return 0;
}
void *f0ext_new(void) {
	t_f0ext *x= (t_f0ext *)object_alloc(f0ext_class);
	if(x) {
        x->valOut= 0;
        x->out= intout(x);
	}
    return (x);
}
void f0ext_assist(t_f0ext *x, void *b, long m, long a, char *s) {
	if(m==ASSIST_INLET) {
		sprintf(s, "index >= 0 (int)");
	} else if(m==ASSIST_OUTLET) {
		sprintf(s, "value (int)");
	}
}
void f0ext_bang(t_f0ext *x) {
	outlet_int(x->out, x->valOut);
}
void f0ext_int(t_f0ext *x, long val) {
	if(val>=0) {
		x->valOut= f0ext_theFunction(val);
		f0ext_bang(x);
	} else {
		post("f0.noergaard warning: only possitive indexes");
	}
}
long f0ext_theFunction(long index) {
	long i, len, res= 0;
	len= count_bits(index);
	for(i= len-1; i>=0; i--) {
		if((index>>i)&01) {
			res++;
		} else {
			if(res>=0) {
				res= 0-res;
			} else {
				res= fabs(res);
			}
		}
	}
	return res;
}
long count_bits(long n) {
	long i= 0;
	for(; n!=0; n= n>>1) {
		i++;
	}
	return i;
}
