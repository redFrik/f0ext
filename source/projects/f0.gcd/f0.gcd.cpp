/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2001-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 010505                              --
//      -- updated 040225 - for carbon using cw8.0              --
//      -- divide by 0 bugfix 060507 (051228)                   --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class gcd : public object<gcd> {
public:
    MIN_DESCRIPTION	{ "Greatest common divisor." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };

    inlet<> m_in1	{ this, "(list/number) Values" };
    inlet<> m_in2	{ this, "(number) Value" };
    outlet<> m_out1	{ this, "(number) Divisor" };
    
    //TODO check if this is needed
    /*message<> bang { this, "bang",
        m_out1.send(m_value);
        return {};
    };*/

    message<> list { this, "list",
        MIN_FUNCTION {
            m_value = args[0];
            for (auto i = 1; i < args.size(); ++i) {
                m_second = args[i];
                m_value = greatestCommonDivisor(m_value, m_second);
            }
            m_out1.send(m_value);
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.gcd v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = greatestCommonDivisor(args[0], m_second);
                m_out1.send(m_value);
                //TODO maybe calculate in bang instead and then call bang here
            } else if (inlet == 1) {
                m_second = args[0];
            }
            return {};
        }
    };

private:
    long m_second { 0 };
    long m_value { 0 };

    long greatestCommonDivisor(long a, long b) {
        if (b == 0) {
            return 0;
        }
        if (a % b != 0) {
            return greatestCommonDivisor(b, a % b);
        } else {
            return fabs(b);
        }
    }
};

MIN_EXTERNAL(gcd);
