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

    outlet<> m_out1	{ this, "(signal) Distance between consecutive numbers (delta)", "signal" };

    f0_distance_tilde(const atoms& args = {}) {
        auto m_in1 = std::make_unique<inlet<>>(this, "(signal) X");
        m_inlets.push_back(std::move(m_in1));
        if (args.size() > 0) {
            if (args[0] == 2) {
                auto m_in2 = std::make_unique<inlet<>>(this, "(signal/number) Y");
                m_inlets.push_back(std::move(m_in2));
            } else if (args[0] == 3) {
                auto m_in2 = std::make_unique<inlet<>>(this, "(signal/number) Y");
                m_inlets.push_back(std::move(m_in2));
                auto m_in3 = std::make_unique<inlet<>>(this, "(signal/number) Z");
                m_inlets.push_back(std::move(m_in3));
            }
        }
    };

    argument<int> dimensions_arg { this, "dimensions", "Dimensions (1 - 3)." };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.distance~ v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    void operator()(audio_bundle input, audio_bundle output) {
        auto in = input.samples(0);
        auto out = output.samples(0);
        if (m_inlets.size() == 1) {
            for (auto i = 0; i < input.frame_count(); ++i) {
                out[i] = fabs(std::sqrt(pow(in[i] - m_x, 2.0)));
                m_x = in[i];
            }
        } else if (m_inlets.size() == 2) {
            auto in2 = input.samples(1);
            for (auto i = 0; i < input.frame_count(); ++i) {
                out[i] = fabs(std::sqrt(pow(in[i] - m_x, 2.0) + pow(in2[i] - m_y, 2.0)));
                m_x = in[i];
                m_y = in2[i];
            }
        } else if (m_inlets.size() == 3) {
            auto in2 = input.samples(1);
            auto in3 = input.samples(2);
            for (auto i = 0; i < input.frame_count(); ++i) {
                out[i] = fabs(std::sqrt(pow(in[i] - m_x, 2.0) + pow(in2[i] - m_y, 2.0) + pow(in3[i] - m_z, 2.0)));
                m_x = in[i];
                m_y = in2[i];
                m_z = in3[i];
            }
        }
    }

private:
    std::vector<std::unique_ptr<inlet<>>> m_inlets;
    double m_x { 0.0 };
    double m_y { 0.0 };
    double m_z { 0.0 };
    
};

MIN_EXTERNAL(f0_distance_tilde);
