/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2001-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 010522 (rev.011004)                 --
//      -- updated 040224 - for carbon using cw8.0              --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- bugfix 130810 - tonesPerOctave was broken            --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class tune : public object<tune> {
public:
    MIN_DESCRIPTION	{ "Frequency quantiser." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.tune~, mtof, round" };

    inlet<> m_in1	{ this, "(number) Frequency (Hz)" };
    inlet<> m_in2	{ this, "(number) Base frequency (Hz)" };
    outlet<> m_out1	{ this, "(number) Quantised frequency (Hz)" };

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
            cout << "f0.tune v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = args[0];
                theFunction();
            } else if (inlet == 1) {
                base = args[0];
            }
            return {};
        }
    };

private:
    double m_value { 0.0 };

    void theFunction() {
        auto c = 0.0;
	    if ((base != 0.0) && (tonesPerOctave != 0.0)) {
		    auto v = pow(2.0, 1.0 / tonesPerOctave);
		    auto a = (log(fabs(m_value)) - log(fabs(base))) / log(v);
		    auto b = round(69.0 + a) - 69.0;
		    c= fabs(base) * pow(v, b);
	    }
	    m_value= c;
	    m_out1.send(m_value);
    }

};

MIN_EXTERNAL(tune);
