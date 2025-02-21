/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2004-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 040226                              --
//      -- converted from abstraction with the same name        --
//      -- updated 051016 - for wmax using cygwin               --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class limit_counter : public object<limit_counter> {
public:
    MIN_DESCRIPTION	{ "Counter with floor and ceil limits." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.ultimate_counter, counter" };

    inlet<> m_in1	{ this, "(bang/number) Bang counts upwards, number sets counter value" };
    inlet<> m_in2	{ this, "(bang) Bang counts downwards" };
    inlet<> m_in3	{ this, "(number) Floor (min)" };
    inlet<> m_in4	{ this, "(number) Ceil (max)" };
    outlet<> m_out1	{ this, "(number) Counter value" };
    outlet<> m_out2	{ this, "(bang) Counter hits floor" };
    outlet<> m_out3	{ this, "(bang) Counter hits ceil" };

    argument<number> floor_arg { this, "floor", "Minimum.",
        MIN_ARGUMENT_FUNCTION {
            floor = arg;
        }
    };

    argument<number> ceil_arg { this, "ceil", "Maximum.",
        MIN_ARGUMENT_FUNCTION {
            ceil = arg;
        }
    };

    attribute<long> floor { this, "floor", C74_LONG_INT_MIN};

    attribute<long> ceil { this, "ceil", C74_LONG_INT_MAX};

    message<> bang { this, "bang",
        if (inlet == 0) {
            if (m_value < ceil) {
                m_value++;
            }
            if (m_value == ceil) {
                m_out3.send(k_sym_bang);
            }
        } else if (inlet == 1) {
            if (m_value > floor) {
                m_value--;
            }
            if (m_value == floor) {
                m_out2.send(k_sym_bang);
            }
        }
        m_out1.send(m_value);
        return {};
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.limit_counter v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = args[0];
            } else if (inlet == 2) {
                floor = args[0];
            } else if (inlet == 3) {
                ceil = args[0];    
            }
            return {};
        }
    };

private:
    long m_value { 0 };

};

MIN_EXTERNAL(limit_counter);
