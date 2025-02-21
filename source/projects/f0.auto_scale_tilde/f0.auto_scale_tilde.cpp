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

class auto_scale_tilde : public object<auto_scale_tilde>, public sample_operator<1, 1> {
public:
    MIN_DESCRIPTION	{ "Finds minimum and maximum values of a signal and uses them as input scaling range. Audio version." };
    MIN_TAGS		{ "audio, f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.auto_scale, scale~, f0.range~" };

    inlet<> m_in1	{ this, "(signal) Input to be scaled" };
    inlet<> m_in2	{ this, "(signal/number) Set low output", low };
    inlet<> m_in3	{ this, "(signal/number) Set high output", high };
    outlet<> m_out1	{ this, "(signal) Scaled output", "signal" };
    outlet<> m_out2	{ this, "(list) Info" };

    argument<number> low_arg { this, "low", "Initial low output.",
        MIN_ARGUMENT_FUNCTION {
            low = arg;
        }
    };

    argument<number> high_arg { this, "high", "Initial high output.",
        MIN_ARGUMENT_FUNCTION {
            high = arg;
        }
    };

    attribute<number> low { this, "low", 0.0};
    
    attribute<number> high { this, "high", 1.0};

    message<> bang { this, "bang",
        MIN_FUNCTION {
            low = 0.0;
            high = 0.0;
            m_flag = false;
            return {};
        }
    };

    message<> factor { this, "factor",
        MIN_FUNCTION {
            atoms daList[3];
            auto rangeIn = fabs(m_max - m_min);
            auto rangeOut = fabs(high - low);
            daList[0] = 0;  //index for routing
            if (rangeIn == 0.0) {
                daList[1] = 0.0;
                daList[2] = rangeOut;
            } else if (rangeOut == 0.0) {
                daList[1] = rangeIn;
                daList[2] = 0.0;
            } else if (rangeIn <= rangeOut) {
                daList[1] = 1.0;
                daList[2] = 1.0 / (rangeIn / rangeOut);
            } else {
                daList[1] = 1.0 / (rangeOut / rangeIn);
                daList[2] = 1.0;
            }
            return { daList };
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.auto_scale~ v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 1) {
                low = args[0];
            } else if (inlet == 2) {
                high = args[0];
            }
            return {};
        }
    };

    message<> range { this, "range",
        MIN_FUNCTION {
            atoms daList[3];
            daList[0] = 1;  //index for routing
            if (m_min <= m_max) {
                daList[1] = m_min);
                daList[2] = m_max);
            } else {
                daList[1] = m_max);
                daList[2] = m_min);
            }
            return { daList };
        }
    };

    message<> set { this, "set",
        MIN_FUNCTION {
            m_min = args[0];
            m_max = args[1];
            return {};
        }
    };

    sample operator()(sample in) {
        auto out;
        if (!m_flag && m_min == m_max) {
            flag = true;
            m_min = in;
            m_max = in;
        }
        if (in < m_min) {
            m_min = in;
        }
        if (in > m_max) {
            m_max = in;
        }
        auto rangeIn = fabs(m_max - m_min);
        auto rangeOut = fabs(high - low)
        if (rangeIn == 0.0) {
            if (low <= high) {
                out = middle
            } else {
                out = high;
            }
        } else if (low <= high) {
            out = fabs((in - m_min) / rangeIn * rangeOut) + low;
        } else {
            out = fabs((in - m_max) / rangeIn * rangeOut) + high;
        }
        return { out };
    }

private:
    bool m_flag { false };
    auto m_max { 0.0 }
    auto m_min { 0.0 }
};

MIN_EXTERNAL(auto_scale_tilde);
