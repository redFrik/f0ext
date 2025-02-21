/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2004-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 040331                              --
//      -- updated 051016 - for wmax using cygwin               --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_slub : public object<f0_slub> {
public:
    MIN_DESCRIPTION	{ "Generate CPU spikes." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.slub~" };

    inlet<> m_in1	{ this, "(bang/number) Spike" };
    inlet<> m_in2	{ this, "(number) Amount" };

    argument<number> amount_arg { this, "amount", "Initial spike amount.",
        MIN_ARGUMENT_FUNCTION {
            amount = arg;
        }
    };

    attribute<number> amount { this, "amount", 7.0 };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            theFunction();
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.slub v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            amount = args[0];
            if (inlet == 0) {
                theFunction();
            }
            return {};
        }
    };

private:
    double m_dummy;

    void theFunction() {
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
          amount
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
                    m_dummy= fmod(
                                   l				,
                                   3.1415
                                   )
                    ;
                }
            }}
    	}
    };

};

MIN_EXTERNAL(f0_slub);
