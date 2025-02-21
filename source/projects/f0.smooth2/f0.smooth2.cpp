/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2002-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 020115                              --
//      -- updated 040225 - for carbon using cw8.0              --
//      -- updated 060507 - fix for output on right input       --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 070106 - clip arg and speed improve fix      --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250221 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class smooth2 : public object<smooth2> {
public:
    MIN_DESCRIPTION	{ "Double exponential smoothing (DES)." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.smooth2~, f0.smooth" };

    inlet<> m_in1	{ this, "(number) Value to be smoothed" };
    inlet<> m_in2	{ this, "(number) Smoothing constant (alpha)" };
    inlet<> m_in3	{ this, "(number) Smoothing constant (beta)" };
    outlet<> m_out1	{ this, "(number) Smoothed output" };
    outlet<> m_out2	{ this, "(number) Trend" };

    argument<number> alpha_arg { this, "alpha", "Initial smoothing constant (alpha).",
        MIN_ARGUMENT_FUNCTION {
            alpha = arg;
        }
    };

    argument<number> beta_arg { this, "beta", "Initial smoothing constant (beta).",
        MIN_ARGUMENT_FUNCTION {
            beta = arg;
        }
    };

    attribute<number, threadsafe::no, limit::clamp> alpha { this, "alpha", 0.15,
        description { "Smoothing constant (alpha)." },
        range { 0.0, 1.0 }
    };

    attribute<number, threadsafe::no, limit::clamp> beta { this, "beta", 0.3,
        description { "Smoothing constant (beta)." },
        range { 0.0, 1.0 }
    };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            theFunction();
            return {};
        }
    };

    message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.smooth2 v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = args[0];
                theFunction();
            } else if (inlet == 1) {
                alpha = args[0];
            } else if (inlet == 2) {
                beta = args[0];
            }
            return {};
        }
    };

    message<> set { this, "set",
        MIN_FUNCTION {
            m_prev_value = args[0];
            return {};
        }
    };

private:
    double m_prev_value { 0.0 };
    double m_value { 0.0 };
    double m_prev_trend { 0.0 };
    double m_trend { 0.0 };

    void theFunction() {
        m_value = (1.0 - alpha) * (m_prev_value + m_prev_trend) + alpha * m_value;
        m_trend = (1.0 - beta) * m_prev_trend + beta * (m_value - m_prev_value);
        m_out2.send(m_trend);
        m_out1.send(m_value);
        m_prev_value = m_value;
        m_prev_trend = m_trend;
    }

};

MIN_EXTERNAL(smooth2);
