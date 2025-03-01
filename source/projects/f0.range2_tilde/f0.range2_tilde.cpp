/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2007-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 070106                              --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250301 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_range2_tilde : public object<f0_range2_tilde>, public vector_operator<> {
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

    argument<number> smooth_arg { this, "smooth", "Initial smooth factor.",
        MIN_ARGUMENT_FUNCTION {
            m_smooth = arg;
        }
    };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            if (m_flags == 0) {
                m_min = std::numeric_limits<double>::max();
                m_max = -std::numeric_limits<double>::max();
            } else if (m_flags == 1) {
                m_min = m_setMin;
                m_max = C74_INT64_MIN;
            } else {
                m_min = m_setMin;
                m_max = m_setMax;
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

    void operator()(audio_bundle input, audio_bundle output) {
        auto in = input.samples(0);
        auto out1 = output.samples(0);
        auto out2 = output.samples(1);
        auto out3 = output.samples(2);
        m_max -= m_smooth;
        m_min += m_smooth;

        for (auto i = 0; i < input.frame_count(); ++i) {
            if (in[i] > m_max) {
                m_max = in[i];
            }
            if (in[0] < m_min) {
                m_min = in[0];
            }
            out1[i] = m_min;
            out2[i] = (m_max - m_min) / 2.0 + m_min;
            out3[i] = m_max;
        }
    }

private:
    short m_flags { 0 };
    double m_max { -std::numeric_limits<double>::max() };
    double m_min { std::numeric_limits<double>::max() };
    double m_setMax { -std::numeric_limits<double>::max() };
    double m_setMin { std::numeric_limits<double>::max() };
    double m_smooth { 0.0 };
};

MIN_EXTERNAL(f0_range2_tilde);
