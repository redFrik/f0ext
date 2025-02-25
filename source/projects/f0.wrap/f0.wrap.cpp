/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2006-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 060830                              --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_wrap : public object<f0_wrap> {
public:
    MIN_DESCRIPTION	{ "Wrap float or integer numbers." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.fold, %" };

    inlet<> m_in1	{ this, "(number) Number to wrap" };
    inlet<> m_in2	{ this, "(number) Minimum" };
    inlet<> m_in3	{ this, "(number) Maximum" };
    outlet<> m_out1	{ this, "(number) Wrapped number" };

    f0_wrap(const atoms& args = {}) {
        if (args.size() == 1) {
            max = args[0];
            if (args[0].type() == message_type::float_argument) {
                m_isint = false;
            }
        } else if (args.size() == 2) {
            min = args[0];
            max = args[1];
            if ((args[0].type() == message_type::float_argument) || (args[1].type() == message_type::float_argument)) {
                m_isint = false;
            }
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
            cout << "f0.wrap v3.0; distributed under GNU GPL License" << endl;
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
    bool m_isint { true };

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
            double b = fabs(hi - lo);
            if (in < lo) {
                if (m_isint) {
                    in = hi - fabs(fmod(in - lo + 1.0, b + 1.0));
                } else {
                    in = hi - fabs(fmod(in - lo, b));
                }
            } else {
                if (m_isint) {
                    in = lo + fabs(fmod(in - 1.0 - hi, b + 1.0));
                } else {
                    in = lo + fabs(fmod(in - hi, b));
                }
            }
        }
        return in;
    }

};

MIN_EXTERNAL(f0_wrap);
