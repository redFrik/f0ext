/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2009-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 091221                              --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250301 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_ultimate_counter : public object<f0_ultimate_counter> {
public:
    MIN_DESCRIPTION	{ "Counter with fractional direction/rate and loop settings." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.limit_counter, counter" };

    inlet<> m_in1	{ this, "(bang/number) Bang counts, number sets counter value" };
    inlet<> m_in2	{ this, "(number) Step size / direction / rate" };
    inlet<> m_in3	{ this, "(int) Loop settings (0=limit, 1=loop, 2=palindrome)" };
    inlet<> m_in4	{ this, "(number) Reset counter to number on next clock" };
    inlet<> m_in5	{ this, "(number) Reset counter to number immediately" };
    inlet<> m_in6	{ this, "(number) Floor (min)" };
    inlet<> m_in7	{ this, "(number) Ceil (max)" };
    outlet<> m_out1	{ this, "(number) Counter value" };
    outlet<> m_out2	{ this, "(bang) Counter hits floor" };
    outlet<> m_out3	{ this, "(bang) Counter hits ceil" };

    argument<number> step_arg { this, "step", "Initial step size.",
        MIN_ARGUMENT_FUNCTION {
            step = arg;
        }
    };

    argument<int> loop_arg { this, "loop", "Loop settings.",
        MIN_ARGUMENT_FUNCTION {
            loop = arg;
        }
    };

    argument<number> floor_arg { this, "floor", "Minimum.",
        MIN_ARGUMENT_FUNCTION {
            floor = arg;
            m_value = arg;
        }
    };

    argument<number> ceil_arg { this, "ceil", "Maximum.",
        MIN_ARGUMENT_FUNCTION {
            ceil = arg;
        }
    };

    attribute<number> step { this, "step", 1.0,
        description { "Step size / direction / rate" }
    };

    attribute<int, threadsafe::no, limit::clamp> loop { this, "loop", 0,
        range { 0, 2 },
        description { "Loop settings (0=limit, 1=loop, 2=palindrome)" }
    };

    attribute<double> floor { this, "floor", -std::numeric_limits<double>::max(),
        description { "Floor (min)" }
    };

    attribute<double> ceil { this, "ceil", std::numeric_limits<double>::max(),
        description { "Ceil (max)" }
    };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            if (loop < 1) {
                m_value = clipFunction(m_value, floor, ceil);
            } else if (loop < 2) {
                m_value = wrapFunction(m_value, floor, ceil);
            } else {
                m_value = foldFunction(m_value, floor, ceil);
            }
            m_out1.send(m_value);
            m_value += step;
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.ultimate_counter v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = args[0];
            } else if (inlet == 1) {
                step = args[0];
            } else if (inlet == 2) {
                loop = args[0];
            } else if (inlet == 3) {
                m_value = args[0];
            } else if (inlet == 4) {
                m_value = clipFunction(args[0], floor, ceil);
                m_out1.send(m_value);
            } else if (inlet == 5) {
                floor = args[0];
            } else if (inlet == 6) {
                ceil = args[0];
            }
            return {};
        }
    };

private:
    double m_value { 0.0 };

    double clipFunction(double in, double min, double max) {
        double a;
        if (min > max) {
            a = min;
            min = max;
            max = a;
        }
        if (in > max) {
            m_out3.send(k_sym_bang);
            a = max;
        } else if (in < min) {
            m_out2.send(k_sym_bang);
            a = min;
        } else {
            a = in;
        }
        return a;
    }
    double wrapFunction(double in, double min, double max) {
        double a, b;
        if (min > max) {
            a = min;
            min = max;
            max = a;
        }
        if (((in >= min) && (in <= max)) || (min == max)) {
            a = in;
        } else {
            b = fabs(max - min);
            if (in < min) {
                m_out2.send(k_sym_bang);
                a = max - fabs(fmod(in - min, b));
            } else {
                m_out3.send(k_sym_bang);
                a = min + fabs(fmod(in - max, b));
            }
        }
        return a;
    }
    double foldFunction(double in, double min, double max) {
        double a, b, c;
        if (min > max) {
            a = min;
            min = max;
            max = a;
        }
        if (((in >= min) && (in <= max)) || (min == max)) {
            b = in;
        } else {
            this->step = 0.0 - this->step;
            c = fabs(max - min) * 2.0;
            if (in < min) {
                m_out2.send(k_sym_bang);
                a = min - fmod(in - min, c);
                if ((a >= min) && (a <= max)) {
                    b = a;
                } else {
                    b = max + (max - a);
                }
            } else {
                m_out3.send(k_sym_bang);
                a = max - fmod(in - max, c);
                if ((a > (min - c / 2.0)) && (a <= min)) {
                    b = min + (min - a);
                } else {
                    b = a;
                }
            }
        }
        return b;
    }

};

MIN_EXTERNAL(f0_ultimate_counter);
