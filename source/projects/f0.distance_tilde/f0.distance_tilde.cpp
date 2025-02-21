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

class f0_distance_tilde : public object<f0_distance_tilde>, public vector_operator<> {
public:
    MIN_DESCRIPTION	{ "Calculate delta distance in 1, 2 or 3 dimensions. Audio version." };
    MIN_TAGS		{ "audio, f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.distance" };

    inlet<> m_in1	{ this, "(signal) X" };
    outlet<> m_out1	{ this, "(signal) Distance between consecutive numbers (delta)" };

    distance_tilde(const atoms& args = {}) {
        if (args.size() > 0) {
            if (args[0] == 2) {
                inlet<> m_in2	{ this, "(signal) Y" };
            } else if (args[0] == 3) {
                    inlet<> m_in2	{ this, "(signal/number) Y" };
                    inlet<> m_in3	{ this, "(signal/number) Z" };
                }
            }
        }
    }

    argument<number> dimensions_arg { this, "dimensions", "Dimensions."};

    message<> bang { this, "bang",
        MIN_FUNCTION {
            theFunction();
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.distance~ v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 1) {
                y = args[0];
            } else if (inlet == 2) {
                z = args[0];
            }
            return {};
        }
    };

    sample operator()(audio_bundle input, audio_bundle output) {
        auto in = input.samples(0);
        auto out = output.samples(0);
        if (in.channelcount() == 1) {
            for (auto i = 0; i < input.framecount(); ++i) {
                m_x = in[i] - m_x;
                out[i] = fabs(std::sqrt(pow(m_x, 2.0)));
            }
        } else if (in.channelcount() == 2) {
            for (auto i = 0; i < input.framecount(); ++i) {
                m_x = in[i] - m_x;
                m_y = in[i + 1] - m_y;
                out[i] = fabs(std::sqrt(pow(m_x, 2.0) + pow(m_y, 2.0)));
            }
        } else if (in.channelscount() == 3) {
            for (auto i = 0; i < input.framecount(); ++i) {
                m_x = in[i] - m_x;
                m_y = in[i + 1] - m_y;
                m_z = in[i + 2] - m_z;
                out[i] = fabs(std::sqrt(pow(m_x, 2.0) + pow(m_y, 2.0) + pow(m_z, 2.0)));
            }
        }
    }

private:
    auto m_x { 0.0 }
    auto m_y { 0.0 }
    auto m_z { 0.0 }
};

MIN_EXTERNAL(f0_distance_tilde);
