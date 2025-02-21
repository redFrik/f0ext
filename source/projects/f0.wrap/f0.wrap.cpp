/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2006-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 060830                              --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class wrap : public object<wrap> {
public:
    MIN_DESCRIPTION	{ "Frequency quantiser." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.fold, %" };

    inlet<> m_in1	{ this, "(number) Number to wrap" };
    inlet<> m_in2	{ this, "(number) Minimum" };
    inlet<> m_in3	{ this, "(number) Maximum" };
    outlet<> m_out1	{ this, "(number) Wrapped number" };

    argument<number> min_arg { this, "min", "Minimum.",
        MIN_ARGUMENT_FUNCTION {
            min = arg;
        }
    };

    argument<number> max_arg { this, "max", "Maximum.",
        MIN_ARGUMENT_FUNCTION {
            max = arg;
        }
    };

    //TODO maybe this shouldn't be an attribute?
    attribute<number> min { this, "min", 0.0};

    attribute<number> max { this, "max", 100.0};

    message<> bang { this, "bang",
        MIN_FUNCTION {
            theFunction();
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.wrap v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = args[0];
                theFunction();
            } else if (inlet == 1) {
                min = args[0];
            } else if (inlet == 2) {
                max = args[0];
            }
            return {};
        }
    };

private:
    double m_value { 0.0 };
    short intswitch { 0 };  //TODO

    void theFunction() {
        auto a;
        if (min > max) {
            a = min;
            min = max;
            max = a;
        }
        if ((m_value >= min && in <= max) || (min == max)) {
            a = m_value;
        } else {
            auto b = fabs(max - min);
            if (m_value < min) {
                if (intswitch == 1) {
                    a = max - fabs(fmod(m_value - min + 1.0, b + 1.0));
                } else {
                    a = max - fabs(fmod(m_value - min, b));
                }
            } else {
                if (intswitch == 1) {
                    a = min + fabs(fmod(m_value - 1.0 - max, b + 1.0));
                } else {
                    a = min + fabs(fmod(m_value - max, b));
                }
            }
        }
	    m_out1.send(a);
    }

};

MIN_EXTERNAL(wrap);
