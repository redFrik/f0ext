/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2004-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 040301                              --
//      -- converted from abstraction with the same name        --
//      -- updated 060507 - added set                           --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_auto_scale : public object<f0_auto_scale> {
public:
    MIN_DESCRIPTION	{ "Finds minimum and maximum values of a stream of values and uses them as input scaling range." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.auto_scale~, scale, f0.range" };

    inlet<> m_in1	{ this, "(number) Value to be scaled" };
    inlet<> m_in2	{ this, "(number) Set low output" };
    inlet<> m_in3	{ this, "(number) Set high output" };
    outlet<> m_out1	{ this, "(number) Scaled output" };
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
            atoms daList(3);
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
            return daList;
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.auto_scale v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = args[0];
                theFunction();
            } else if (inlet == 1) {
                low = args[0];
            } else if (inlet == 2) {
                high = args[0];
            }
            return {};
        }
    };

    message<> range { this, "range",
        MIN_FUNCTION {
            atoms daList(3);
            daList[0] = 1;  //index for routing
            if (m_min <= m_max) {
                daList[1] = m_min;
                daList[2] = m_max;
            } else {
                daList[1] = m_max;
                daList[2] = m_min;
            }
            return daList;
        }
    };

    message<> set { this, "set",
        MIN_FUNCTION {
            m_min = args[0];
            m_max = args[1];
            return {};
        }
    };

private:
    bool m_flag { false };
    double m_max { 0.0 };
    double m_min { 0.0 };
    double m_value { 0.0 };

    void theFunction() {
        if (!m_flag && m_min == m_max) {
            m_flag = true;
            m_min = m_value;
            m_max = m_value;
        }
        if (m_value < m_min) {
            m_min = m_value;
        }
        if (m_value > m_max) {
            m_max = m_value;
        }
        auto rangeIn = fabs(m_max - m_min);
        auto rangeOut = fabs(high - low);
        if (rangeIn == 0.0) {
            if (low <= high) {
                m_value = low;
            } else {
                m_value = high;
            }
        } else if (low <= high) {
            m_value= fabs((m_value - m_min) / rangeIn*rangeOut) + low;
        } else {
            m_value= fabs((m_value - m_max) / rangeIn*rangeOut) + high;
        }
        m_out1.send(m_value);
    }

};

MIN_EXTERNAL(f0_auto_scale);
