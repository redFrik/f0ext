/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2001-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 010522 (rev.011004)                 --
//      -- updated 040225 - for carbon using cw8.0              --
//      -- updated 051016 - for wmax using cygwin               --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"
#include <cmath>

using namespace c74::min;

class f0_distance : public object<f0_distance> {
public:
    MIN_DESCRIPTION	{ "Calculate delta distance in 1, 2 or 3 dimensions" };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.distance~" };

    inlet<> m_in1	{ this, "(number/list) X" };
    outlet<> m_out1	{ this, "(number) Distance between consecutive numbers (delta)" };

    f0_distance(const atoms& args = {}) {
        if (args.size() > 0) {
            if (args[0] == 2) {
                auto m_in2 = std::make_unique<inlet<>>(this, "(number) Y");
                m_inlets.push_back(std::move(m_in2));
            } else if (args[0] == 3) {
                auto m_in2 = std::make_unique<inlet<>>(this, "(number) Y");
                m_inlets.push_back(std::move(m_in2));
                auto m_in3 = std::make_unique<inlet<>>(this, "(number) Z");
                m_inlets.push_back(std::move(m_in3));
            }
        }
    };

    argument<number> dimensions_arg { this, "dimensions", "Dimensions."};

    attribute<number> x { this, "x", 0.0};

    attribute<number> y { this, "y", 0.0};

    attribute<number> z { this, "z", 0.0};

    message<> bang { this, "bang",
        MIN_FUNCTION {
            theFunction();
            return {};
        }
    };

    message<> list { this, "list",
        MIN_FUNCTION {
            x = args[0];
            if (args.size() > 1) {
                y = args[1];
                if (args.size() > 2) {
                    z = args[2];
                }
            }
            bang();
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.distance v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                x = args[0];
                theFunction();
            } else if (inlet == 1) {
                y = args[0];
            } else if (inlet == 2) {
                z = args[0];
            }
            return {};
        }
    };

private:
    std::vector< std::unique_ptr<inlet<>> >	m_inlets;

    void theFunction() {
        m_out1.send(fabs(std::sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0))));
    }

};

MIN_EXTERNAL(f0_distance);
