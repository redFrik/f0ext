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

class f0_range_tilde : public object<f0_range_tilde>, public sample_operator<1, 3> {
public:
    MIN_DESCRIPTION	{ "Finds minimum, middle and maximum values of a signal. Audio version." };
    MIN_TAGS		{ "audio, f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.range, f0.range2~, f0.auto_scale~" };

    inlet<> m_in1	{ this, "(signal/bang) Input to be checked. Bang resets." };
    outlet<> m_out1	{ this, "(signal) Minimum", "signal" };
    outlet<> m_out2	{ this, "(signal) Middle", "signal" };
    outlet<> m_out3	{ this, "(signal) Maximum", "signal" };

    argument<number> min_arg { this, "min", "Initial anticipated minimum value.",
        MIN_ARGUMENT_FUNCTION {
            m_flags++;
            m_min = arg;
            m_setMin = arg;
        }
    };

    argument<number> max_arg { this, "max", "Initial anticipated maximum value.",
        MIN_ARGUMENT_FUNCTION {
            m_flags++;
            m_max = arg;
            m_setMax = arg;
        }
    };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            if (m_flags == 0) {
                m_min = std::numeric_limits<sample>::max();
                m_max = -std::numeric_limits<sample>::max();
            } else if (m_flags == 1) {
                m_min = m_setMin;
                m_max = -std::numeric_limits<sample>::max();
            } else {
                m_min = m_setMin;
                m_max = m_setMax;
            }
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.range~ v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> set { this, "set",
        MIN_FUNCTION {
            if (args.size() < 2) {
                cout << "warning: set needs at least 2 arguments." << endl;
            } else {
                double lo = args[0];
                double hi = args[1];
                if (lo < hi) {
                    m_min = lo;
                    m_max = hi;
                    m_setMin = lo;
                    m_setMax = hi;
                } else {
                    m_min = hi;
                    m_max = lo;
                    m_setMin = hi;
                    m_setMax = lo;
                }
                m_flags = 2;
            }
            return {};
        }
    };

    samples<3> operator()(sample in) {
        if (in > m_max) {
            m_max = in;
        }
        if (in < m_min) {
            m_min = in;
        }
        return { m_min, (m_max - m_min) / 2.0 + m_min, m_max };
    }

private:
    short m_flags { 0 };
    double m_max { -std::numeric_limits<sample>::max() };
    double m_min { std::numeric_limits<sample>::max() };
    double m_setMax { -std::numeric_limits<sample>::max() };
    double m_setMin { std::numeric_limits<sample>::max() };
    
};

MIN_EXTERNAL(f0_range_tilde);
