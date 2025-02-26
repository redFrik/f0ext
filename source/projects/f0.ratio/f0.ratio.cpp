/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2004-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 040301                              --
//      -- converted from abstraction with the same name        --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_ratio : public object<f0_ratio> {
public:
    MIN_DESCRIPTION	{ "keep width/height proportion." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "" };

    inlet<> m_in1	{ this, "(number) Width" };
    inlet<> m_in2	{ this, "(number) Height" };
    outlet<> m_out1	{ this, "(number) Output width" };
    outlet<> m_out2	{ this, "(number) Output height" };

    argument<number> width_arg { this, "width", "Initial width proportion.",
        MIN_ARGUMENT_FUNCTION {
            m_proportionWidth = arg;
        }
    };

    argument<number> height_arg { this, "height", "Initial height proportion.",
        MIN_ARGUMENT_FUNCTION {
            m_proportionHeight = arg;
        }
    };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            m_out2.send(m_height);
            m_out1.send(m_width);
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.ratio v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_width = args[0];
                m_height = (m_width / m_proportionWidth) * m_proportionHeight;
            } else if (inlet == 1) {
                m_height = args[0];
                m_width = (m_height / m_proportionHeight) * m_proportionWidth;
            }
            bang();
            return {};
        }
    };

    message<> set { this, "set",
        MIN_FUNCTION {
            if (args.size() < 2) {
                cout << "warning: set needs at least 2 arguments." << endl;
            } else {
                if (args[0] == 0.0) {
                    m_proportionWidth = 1.0;
                } else {
                    m_proportionWidth = args[0];
                }
                if (args[1] == 0.0) {
                    m_proportionHeight = 1.0;
                } else {
                    m_proportionHeight = args[1];
                }
            }
            return {};
        }
    };

private:
    double m_width { 0.0 };
    double m_height { 0.0 };
    double m_proportionWidth { 1.0 };
    double m_proportionHeight { 1.0 };

};

MIN_EXTERNAL(f0_ratio);
