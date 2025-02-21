/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2007-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 070106                              --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_smooth_tilde : public object<f0_smooth_tilde>, public sample_operator<2, 1> {
public:
    MIN_DESCRIPTION	{ "Single exponential smoothing (SES). Audio version." };
    MIN_TAGS		{ "audio, f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.smooth, f0.smooth2~" };

    inlet<> m_in1	{ this, "(signal) Input to be smoothed" };
    inlet<> m_in2	{ this, "(signal/number) Smoothing constant (alpha)", alpha };
    outlet<> m_out1	{ this, "(signal) Smoothed output", "signal" };

    argument<number> alpha_arg { this, "alpha", "Initial smoothing constant (alpha).",
        MIN_ARGUMENT_FUNCTION {
            alpha = arg;
        }
    };

    attribute<number, threadsafe::no, limit::clamp> alpha { this, "alpha", 0.15,
        description { "Smoothing constant (alpha)." },
        range { 0.0, 1.0 }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.smooth~ v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 1) {
                alpha = args[0];
            }
            return {};
        }
    };

    sample operator()(sample in1, sample in2) {
        auto a = pow(alpha, 4.0);
        m_prev = (1.0 - a) * m_prev + a * in1;
        return m_prev;
    }

private:
    sample m_prev { 0.0 };
};

MIN_EXTERNAL(f0_smooth_tilde);
