/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2004-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 040330 (rev.060511)                 --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"
#include <cmath>

using namespace c74::min;

class f0_range2 : public object<f0_range2> {
public:
    MIN_DESCRIPTION	{ "Finds minimum, middle and maximum values of a stream of numbers with fallback." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.range~, f0.range2, f0.auto_scale" };

    inlet<> m_in1	{ this, "(number/bang) Value to check. Bang resets." };
    inlet<> m_in2	{ this, "(number) Fallback / smooth factor." };
    outlet<> m_out1	{ this, "(number) Minimum" };
    outlet<> m_out2	{ this, "(number) Middle" };
    outlet<> m_out3	{ this, "(number) Maximum" };

    argument<number> min_arg { this, "min", "Initial anticipated minimum value.",
        MIN_ARGUMENT_FUNCTION {
            m_flags++;
            m_setMin = arg;
        }
    };

    argument<number> max_arg { this, "max", "Initial anticipated maximum value.",
        MIN_ARGUMENT_FUNCTION {
            m_flags++;
            m_setMax = arg;
        }
    };

    argument<number> smooth_arg { this, "smooth", "Initial smooth factor.",
        MIN_ARGUMENT_FUNCTION {
            m_smooth = arg;
        }
    };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            cout << "TODO check if these are set: " << min_arg <<endl;
            cout << "TODO check if these are set: " << max_arg <<endl;
            if (m_flags == 0) {
                m_min = INFINITY;
                m_max = -INFINITY;
            } else if (m_flags == 1) {
                m_min = m_setMin;
                m_max = -INFINITY;
            } else {
                m_min = setMin;
                m_max = setMax;
            }
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.range2 v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                theFunction(args[0]);
            } else {
                m_smooth = args[0];
            }
            return {};
        }
    };

    message<> set { this, "set",
        MIN_FUNCTION {
            if (args[0] < args[1]) {
                m_min = args[0];
                m_max = args[1];
                m_setMin = args[0];
                m_setMax = args[1];
            } else {
                m_min = args[1];
                m_max = args[0];
                m_setMin = args[1];
                m_setMax = args[0];
            }
            m_flags = 2;
            return {};
        }
    };

private:
    short m_flags { 0 }
    auto m_max { -INFINITY };
    auto m_min { INFINITY };
    auto m_setMax { -INFINITY };
    auto m_setMin { INFINITY };
    auto m_smooth { 0.0 }

    void theFunction(auto val) {
        m_max -= m_smooth;
        m_min += m_smooth;
        if (val > m_max) {
            m_max = val;
        }
        if (val < m_min) {
            m_min = val;
        }
        m_out3.send(m_max);
        m_out2.send((m_max - m_min) / 2.0 + m_min);
        m_out1.send(m_min);
    }
};

MIN_EXTERNAL(f0_range2);
