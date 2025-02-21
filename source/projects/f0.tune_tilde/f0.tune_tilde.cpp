/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2007-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 070106                              --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_tune_tilde : public object<f0_tune_tilde>, public sample_operator<2, 1> {
public:
    MIN_DESCRIPTION	{ "Frequency quantiser. Audio version." };
    MIN_TAGS		{ "audio, f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.tune, mtof~, round~" };

    inlet<> m_in1	{ this, "(signal) Frequency (Hz)" };
    inlet<> m_in2	{ this, "(signal/number) Base frequency (Hz)", base };
    outlet<> m_out1	{ this, "(signal) Quantised frequency (Hz)", "signal" };

    argument<number> base_arg { this, "base", "Initial base frequency.",
        MIN_ARGUMENT_FUNCTION {
            base = arg;
        }
    };

    //TODO maybe this shouldn't be an attribute?
    attribute<number> base { this, "base", 440.0};

    attribute<number> tonesPerOctave { this, "tonesPerOctave", 12.0};

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.tune~ v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 1) {
                tune = args[0];
            }
            return {};
        }
    };

    sample operator()(sample in1, sample in2) {
        sample c = 0.0;
        if ((base != 0.0) && (tonesPerOctave != 0.0)) {
            auto v = pow(2.0, 1.0 / tonesPerOctave);
            auto a = (log(fabs(in1)) - log(fabs(base))) / log(v);
		    auto b = round(69.0 + a) - 69.0;
		    c = fabs(base) * pow(v, b);
        }
        return { c };
    }

};

MIN_EXTERNAL(f0_tune_tilde);
