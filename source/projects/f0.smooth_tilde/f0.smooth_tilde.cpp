/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2007-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//		----------------------------------------------------------
//		-- fredrik olofsson 070106								--
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250210 - ported to min-devkit (sdk8)         --
//		----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class smooth_tilde : public object<smooth_tilde>, public sample_operator<1, 1> {
public:
    MIN_DESCRIPTION { "Single exponential smoothing (SES). Audio version." };
	MIN_TAGS        { "audio, f0ext" };
	MIN_AUTHOR      { "Fredrik Olofsson" };
	MIN_RELATED     { "f0.smooth, f0.smooth2~" };

	inlet<> m_in1   { this, "(signal) Input to be smoothed" };
	inlet<> m_in2   { this, "(number) Smoothing constant (alpha)" };
	outlet<> m_out1 { this, "(signal) Smoothed output", "signal" };

	argument<number> alpha_arg { this, "alpha", "Initial smoothing constant (alpha).",
        MIN_ARGUMENT_FUNCTION {
            alpha = arg;
        }
    };

	attribute<number> alpha { this, "alpha", 0.15,
		description { "Smoothing constant (alpha)." },
        setter { MIN_FUNCTION {
            return { pow(max(min(args[0], 1.0), 0.0), 4.0) };
        }}
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.smooth~ v2.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

	sample operator()(sample input) {
        
		return input*alpha;
        
		// r= *inR++;
		// if(r<0.0) r= 0.0; else if(r>1.0) r= 1.0;
		// a= pow(r, 4.0);
		// prev= *outL++= (1.0-a)*prev+a*(*inL++);
		//x->prev= prev;
    }
};

MIN_EXTERNAL(smooth_tilde);
