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

class f0_auto_scale_tilde : public object<f0_auto_scale_tilde>, public sample_operator<3, 1> {
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

    attribute<number> low { this, "low", 0.0 };
    
    attribute<number> high { this, "high", 1.0 };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            m_max = 0.0;
            m_min = 0.0;
            m_flag = false;
            return {};
        }
    };

    message<> factor { this, "factor",
        MIN_FUNCTION {
            auto rangeIn = fabs(m_max - m_min);
            auto rangeOut = fabs(high - low);
            if ((rangeIn == 0.0) || (rangeOut == 0.0)) {
                m_out2.send(0, rangeIn, rangeOut);
            } else if (rangeIn <= rangeOut) {
                m_out2.send(0, 1.0, 1.0 / (rangeIn / rangeOut));
            } else {
                m_out2.send(0, 1.0 / (rangeOut / rangeIn), 1.0);
            }
            return {};
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
            if (m_min <= m_max) {
                m_out2.send(1, m_min, m_max);
            } else {
                m_out2.send(1, m_max, m_min);
            }
            return {};
        }
    };

    message<> set { this, "set",
        MIN_FUNCTION {
            m_min = args[0];
            m_max = args[1];
            return {};
        }
    };

    sample operator()(sample in, sample in2, sample in3) {
        sample out;
        if (m_in2.has_signal_connection()) {
            m_min = in2;
        }
        if (m_in3.has_signal_connection()) {
            m_max = in3;
        }
        if ((m_flag == false) && (m_min == m_max)) {
            m_flag = true;
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
        auto rangeOut = fabs(this->high - this->low);
        if (rangeIn == 0.0) {
            if (this->low <= this->high) {
                out = this->low;
            } else {
                out = this->high;
            }
        } else if (this->low <= this->high) {
            out = fabs((in - m_min) / rangeIn * rangeOut) + this->low;
        } else {
            out = fabs((in - m_max) / rangeIn * rangeOut) + this->high;
        }
        return out;
    }

private:
    bool m_flag { false };
    double m_max { 0.0 };
    double m_min { 0.0 };
};

MIN_EXTERNAL(f0_auto_scale_tilde);
