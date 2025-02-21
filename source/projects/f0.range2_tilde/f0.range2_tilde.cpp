/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2007-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 070106                              --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_range2_tilde : public object<f0_range2_tilde>, public sample_operator<1, 3> {
public:
    MIN_DESCRIPTION	{ "Finds minimum, middle and maximum values of a signal with fallback. Audio version." };
    MIN_TAGS		{ "audio, f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.range2, f0.range~, f0.auto_scale~" };

    inlet<> m_in1	{ this, "(signal/bang) Input to be checked. Bang resets." };
    inlet<> m_in2	{ this, "(number) Fallback / smooth factor." };
    outlet<> m_out1	{ this, "(signal) Minimum", "signal" };
    outlet<> m_out2	{ this, "(signal) Middle", "signal" };
    outlet<> m_out3	{ this, "(signal) Maximum", "signal" };

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
            cout << "f0.range2~ v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 1) {
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

    samples<3> operator()(sample in) {
        m_max -= m_smooth;
        m_min += m_smooth;
        if (in > m_max) {
            m_max = in;
        }
        if (in < m_min) {
            m_min = in;
        }
        return { m_min, (m_max - m_min) / 2.0 + m_min, m_max };
    }

private:
    short m_flags { 0 }
    auto m_max { -INFINITY };
    auto m_min { INFINITY };
    auto m_setMax { -INFINITY };
    auto m_setMin { INFINITY };
    auto m_smooth { 0.0 }
};

MIN_EXTERNAL(f0_range2_tilde);
