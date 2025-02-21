/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2001-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 010614 (rev.011004)                 --
//      -- updated 040225 - for carbon using cw8.0              --
//      -- updated 060507 - minor updates                       --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class noergaard : public object<noergaard> {
public:
    MIN_DESCRIPTION	{ "Per Nørgård's infinity series." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };

    inlet<> m_in1	{ this, "(int) index (>= 0)" };
    outlet<> m_out1	{ this, "(int) Value" };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            m_out1.send(m_value);
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.noergaard v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "int",
        MIN_FUNCTION {
            if (args[0] >= 0) {
                m_value = theFunction(args[0]);
                bang();
            } else {
                cout << "f0.noergaard warning: only possitive indices" << endl;
            }
            return {};
        }
    };

private:
    long m_value { 0 };

    long theFunction(long index) {
        long i, len, res = 0;
        len = count_bits(index);
        for(i = len - 1; i >= 0; i--) {
            if ((index >>i) & 01) {
                res++;
            } else {
                if (res >= 0) {
                    res= 0 - res;
                } else {
                    res= fabs(res);
                }
            }
        }
        return res;
    }
    long count_bits(long n) {
        long i = 0;
        for(; n != 0; n = n >> 1) {
            i++;
        }
        return i;
    }
};

MIN_EXTERNAL(noergaard);
