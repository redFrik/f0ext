/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2004-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 040226                              --
//      -- converted from abstraction with the same name        --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 130702 - rewrote argument handling in new    --
//      -- updated 250121 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_beats_to_frames : public object<f0_beats_to_frames> {
public:
    MIN_DESCRIPTION	{ "Calculates how many [video]frames will fit in x beats." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "f0.frames_to_beats" };

    inlet<> m_in1	{ this, "(number) Number of beats" };
    inlet<> m_in2	{ this, "(number) Tempo (BPM)" };
    inlet<> m_in3	{ this, "(number) Frames per second (FPS)" };
    outlet<> m_out1	{ this, "(number) Number of frames" };

    argument<number> tempo_arg { this, "tempo", "Initial tempo (BPM).",
        MIN_ARGUMENT_FUNCTION {
            tempo = arg;
        }
    };

    argument<number> fps_arg { this, "fps", "Initial frames per second (FPS).",
        MIN_ARGUMENT_FUNCTION {
            fps = arg;
        }
    };

    attribute<number> tempo { this, "tempo", 120.0,
        description { "Tempo (BPM)" }
    };

    attribute<number> fps { this, "fps", 20.0,
        description { "Frames per second (FPS)" }
    };

    message<> bang { this, "bang",
        MIN_FUNCTION {
            m_out1.send(m_value);
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.beats_to_frames v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            if (inlet == 0) {
                m_value = theFunction(args[0]);
                bang();
            } else if (inlet == 1) {
                tempo = args[0];
            } else if (inlet == 2) {
                fps = args[0];
            }
            return {};
        }
    };

private:
    double m_value { 0.0 };

    double theFunction(double in) {
        if (tempo != 0.0) {
            return (60.0 / tempo) * (in * fps);
        }
        return 0.0;
    }

};

MIN_EXTERNAL(f0_beats_to_frames);
