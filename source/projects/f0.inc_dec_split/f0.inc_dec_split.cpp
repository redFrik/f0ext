/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2004-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 040226                              --
//      -- converted from abstraction with the same name        --
//      -- well, almost the same name: f0.inc/dec_split         --
//      -- updated 051016 - for wmax using cygwin               --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 081104 - bugfix for repeating values - v1.1  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - simplified argument handling in new --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_inc_dec_split : public object<f0_inc_dec_split> {
public:
    MIN_DESCRIPTION	{ "Detect direction of incoming values and split to different outlets." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "change" };

    inlet<> m_in1	{ this, "(bang/number) Value" };
    inlet<> m_in2	{ this, "(number) Start value" };
    outlet<> m_out1	{ this, "(number) Increasing values" };
    outlet<> m_out2	{ this, "(number) Decreasing values" };
    
    argument<number> start_arg { this, "start", "Start value.",
        MIN_ARGUMENT_FUNCTION {
            start = arg;
            m_prev = arg;
        }
    };

    attribute<number> start { this, "start", 0.0};

    message<> bang { this, "bang",
        theFunction();
        return {};
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.inc_dec_split v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = args[0];
                theFunction();
            } else if (inlet == 1) {
                start = args[0];
                m_prev = args[0];
            }
            return {};
        }
    };

    message<> reset { this, "reset",
        MIN_FUNCTION {
            m_prev = start;
            return {};
        }
    };

private:
    bool m_flag { false }
    auto m_prev { 0.0 };
    auto m_value { 0.0 };

    void theFunction() {
        if (m_value > m_prev) {
            m_out1.send(m_value);
            m_flag = false;
            m_prev = m_value;
        } else if (m_value < m_prev) {
            m_out2.send(m_value);
            m_flag = true;
            m_prev = m_value;
        } else {
            if (m_flag) {
                m_out2.send(m_value);
            } else {
                m_out1.send(m_value);
            }
        }
    }
};

MIN_EXTERNAL(f0_inc_dec_split);
