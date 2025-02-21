/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2002-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 020206                              --
//      -- updated 040225 - for carbon using cw8.0              --
//      -- updated 051016 - for wmax using cygwin               --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class snap : public object<snap> {
public:
    MIN_DESCRIPTION	{ "Smooth input values by snapping." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.smooth, f0.smooth2" };

    inlet<> m_in1	{ this, "(number) Value to be smoothed" };
    inlet<> m_in2	{ this, "(number) Resistance" };
    outlet<> m_out1	{ this, "(number) Output" };

    argument<number> resistance_arg { this, "resistance", "Initial resistance.",
        MIN_ARGUMENT_FUNCTION {
            resistance = arg;
        }
    };

    //TODO maybe this shouldn't be an attribute?
    attribute<number> resistance { this, "resistance", 50.0};

    message<> bang { this, "bang",
        MIN_FUNCTION {
            theFunction();
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.snap v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = args[0];
                theFunction();
            } else if (inlet == 1) {
                resistance = args[0];
            }
            return {};
        }
    };

    message<> set { this, "set",
        MIN_FUNCTION {
            m_prev = args[0];
            return {};
        }
    };

private:
    double m_prev { 0.0 };
    double m_value { 0.0 };

    void theFunction() {
        auto diff = fabs(m_value - m_prev);
        auto step = 0.0;
        if (diff != 0.0) {
            step = resistance / diff;
        }
        if (m_value > m_prev) {
            m_value += step;
            if (m_value > m_prev) {
                m_value = m_prev;
            }
        } else if (m_value < m_prev) {
            m_value -= step;
            if (m_value < m_prev) {
                m_value = m_prev;
            }
        }
        m_out1.send(m_value);
        m_prev = m_value;
    }

};

MIN_EXTERNAL(snap);
