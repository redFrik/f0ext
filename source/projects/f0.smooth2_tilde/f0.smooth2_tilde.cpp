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

class f0_smooth2_tilde : public object<f0_smooth2_tilde>, public sample_operator<3, 2> {
public:
    MIN_DESCRIPTION	{ "Double exponential smoothing (DES). Audio version." };
    MIN_TAGS		{ "audio, f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.smooth2, f0.smooth~" };

    inlet<> m_in1	{ this, "(signal) Input to be smoothed" };
    inlet<> m_in2	{ this, "(signal/number) Smoothing constant (alpha)", alpha };
    inlet<> m_in3	{ this, "(signal/number) Smoothing constant (beta)", beta };
    outlet<> m_out1	{ this, "(signal) Smoothed output", "signal" };
    outlet<> m_out2	{ this, "(signal) Trend", "signal" };

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
        range { 0.0, 1.0 },
        description { "Smoothing constant (alpha)." }
    };

    attribute<number, threadsafe::no, limit::clamp> beta { this, "beta", 0.3,
        range { 0.0, 1.0 },
        description { "Smoothing constant (beta)." }
    };

    message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.smooth2~ v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 1) {
                alpha = args[0];
            } else if (inlet == 2) {
                beta = args[0];
            }
            return {};
        }
    };

    samples<2> operator()(sample in1, sample in2, sample in3) {
        sample a;
        sample b;
        if (m_in2.has_signal_connection()) {
            a = in2;
        } else {
            a = this->alpha;
        }
        if (m_in3.has_signal_connection()) {
            b = in3;
        } else {
            b = this->beta;
        }
        a = pow(a, 4.0);
        b = pow(b, 4.0);

        m_out = a * m_value + (1.0 - a) * (m_out + m_trend);    //DES - Double Exponential Smoothing
        m_trend = b * (m_out - m_prev) + (1.0 - b) * m_trend;
        m_prev = m_out;
        m_value = in1;
        
        return { { m_out, m_trend } };
    }

private:
    sample m_out { 0.0 };
    sample m_prev { 0.0 };
    sample m_trend { 0.0 };
    sample m_value { 0.0 };

};

MIN_EXTERNAL(f0_smooth2_tilde);
