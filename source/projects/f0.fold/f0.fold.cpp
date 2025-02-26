/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2001-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 010502                              --
//      -- updated 040225 - for carbon using cw8.0              --
//      -- updated 051016 - for wmax using cygwin               --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_fold : public object<f0_fold> {
public:
    MIN_DESCRIPTION	{ "Fold float and integer numbers." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.wrap, %" };

    inlet<> m_in1	{ this, "(number) Number to fold" };
    inlet<> m_in2	{ this, "(number) Minimum" };
    inlet<> m_in3	{ this, "(number) Maximum" };
    outlet<> m_out1	{ this, "(number) Folded number" };

    f0_fold(const atoms& args = {}) {
        if (args.size() == 1) {
            max = args[0];
        } else if (args.size() == 2) {
            min = args[0];
            max = args[1];
        }
    };

    argument<number> min_arg { this, "min", "Minimum." };

    argument<number> max_arg { this, "max", "Maximum." };

    attribute<number> min { this, "min", 0.0 };

    attribute<number> max { this, "max", 100.0 };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            m_out1.send(m_value);
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.fold v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = theFunction(args[0]);
                bang();
            } else if (inlet == 1) {
                min = args[0];
            } else if (inlet == 2) {
                max = args[0];
            }
            return {};
        }
    };

private:
    double m_value { 0.0 };

    double theFunction(double in) {
        double lo, hi;
        if (this->min > this->max) {
            lo = this->max;
            hi = this->min;
        } else {
            lo = this->min;
            hi = this->max;
        }
        if (((in < lo) || (in > hi)) && (lo != hi)) {
            double a;
            double c = fabs(hi - lo) * 2.0;
            if (in < lo) {
                a = lo - fmod(in - lo, c);
                if ((a >= lo) && (a <= hi)) {
                    in = a;
                } else {
                    in = hi + (hi - a);
                }
            } else {
                a = hi - fmod(in - hi, c);
                if ((a > (lo - c / 2.0)) && (a <= lo)) {
                    in = lo + (lo - a);
                } else {
                    in = a;
                }
            }
        }
        return in;
    }

};

MIN_EXTERNAL(f0_fold);
