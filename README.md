# externals for maxmsp

Externals / plugins to be used with [MaxMSPJitter](https://cycling74.com)

* **f0.auto_scale** - Find minimum and maximum values from a stream of numbers and use them as input scaling range.
* **f0.auto_scale~** - MSP
* **f0.construct** - A constructive constructor. Connect to a standard 2D graphics object like Max's [lcd], Jitter's [jit.lcd], SoftVNS' [v.draw] or nato.0+55's [242.qd]
* **f0.distance** - Calculate delta distance in 1, 2 or 3 dimensions.
* **f0.distance~** - MSP
* **f0.fold** - Fold float and integer numbers to a specified range.
* **f0.gcd** - Greatest common divisor.
* **f0.inc_dec_split** - Detect direction of incoming values and split to different outlets.
* **f0.limit_counter** - A different counter with floor and ceiling.
* **f0.noergaard** - Per Nørgård's infinity series.
* **f0.range** - Find minimum, middle and maximum values from a stream of values.
* **f0.range~** - MSP
* **f0.range2** - Similar to f0.range but with an added smooth factor.
* **f0.range2~** - MSP
* **f0.slub** - Generate CPU spikes - disrupt timing version.
* **f0.slub~** - Generate CPU spikes - audio click version.
* **f0.smooth** - Single exponential smoothing. Good for filtering data from sensors.
* **f0.smooth~** - MSP
* **f0.smooth2** - Double exponential smoothing.
* **f0.smooth2~** - MSP
* **f0.snap** - Smooth by snapping.
* **f0.tune** - Frequency quantiser.
* **f0.tune~** - MSP
* **f0.ultimate_counter** - Counter with floating-point direction/rate and loop settings.

Latest version (Max8, Max9, x86_64 and arm64) available for download from... [releases/latest](http://github.com/redFrik/f0ext/releases/latest).

For older releases (Max4 to Max7) see... <https://fredrikolofsson.com/code/max/>

Older windows 32-bit binaries are available here... <https://www.fredrikolofsson.com/software/f0ext091222win.zip>

JavaScript versions of some of the externals are available here... <https://github.com/redFrik/f0js>

Pd equivalents are here... <https://fredrikolofsson.com/code/pd/>

Distributed under GNU GPL license. See the included file.


## version history

-----250301
* modernised everything using min-devkit (sdk8)
* helpfiles cleanup
* new builds for macOS (both intel and arm) and win64
* deleted f0.wrap (it was made back when stock [%] did not accept floating-point numbers)
* moved f0.beats_to_frames, f0.frames_to_beats and f0.ratio to the f0.abs package
* f0.smooth2~ bugfix for third inlet float (argument or signal not affected)

-----130810
* uploaded to github
* rebuilt with maxsdk-6.1.3
* bugfix f0.tune - tonesPerOctave was broken
* minor formatting in source code files

-----130702
* updates to objects with multiple arguments (f0.auto_scale, f0.auto_scale~, f0.beats_to_frames, f0.fold, f0.frames_to_beats, f0.inc_dec_split, f0.limit_counter, f0.range, f0.range~, f0.range2, f0.range2~, f0.ratio, f0.smooth2, f0.smooth2~, f0.ultimate_counter)
* fixed set message bug in f0.smooth2
* fixed float inlets bug in f0.auto_scale~

-----130630
* rebuilt all the externals using MaxSDK-6.1.1.  should now work with max6.1 on 64 and 32bit computers as well as max5.
* changed the helpfiles to new max6 format
* removed f0.round and f0.round~.  use the standard round and round~ instead.

-----091222
* added f0.ultimate_counter.

-----081104
* fix for f0.inc_dec_split.  now repeated values after decrease are treated correctly

-----070116
* minor update to all externals - trimmed the initial post to the max-window

-----070107
* new msp versions of some objects: f0.auto_scale~, f0.distance~, f0.range~, f0.range2~, f0.round~, f0.smooth~, f0.smooth2~, f0.tune~
* bugfix for f0.smooth and f0.smooth2 - parameters now clipped 0-1

-----070103
* compiled for universal binary using xcode
* removed the need for resources (the .rsrc files) for f0.distance, f0.fold, f0.inc_dec_split, f0.limit_counter, f0.slub, f0.slub~, f0.snap.  these were the last ones

-----060830
* new external f0.wrap

-----060515
(* ported all externals to pd (osx, linux, win) too)
* recompiled all windows externals with the optimisation flag -O2
* put back the helpfile for f0.% (os9 only) which got lost
* recompiled f0.beats_to_frames, f0.frames_to_beats, f0.ratio, f0.tunes and made them check for /0.  also removed the need for resource files for these.
* fixed stepX and stepY <1 bugs in f0.construct and removed the need for resource file

-----060511
* bug in f0.range and f0.range2.  reset (bang) didn't work 100%.  also removed the need for resource files for these.

-----060510
* fixed bug in f0.round that would mess it up if quantise value was set to 0.  fixed some typos in f0.round.help and removed the need for resource file for this external

-----060507
* split up distribution in 3 packages: osx(xmax), win(wmax) and os9(max)
* f0.smooth and f0.smooth2 was changed to not output when changing right inlet.  note - this may break patches under rare circumstances!
* f0.auto_scale: added set message to set input range
* f0.gcd: updated carbon+classic code to do the same check for divide by 0 as win
* renamed f0.nørgård to f0.noergaard also in the osx and classic distros
* removed the need for resources (the .rsrc files) for f0.smooth, f0.smooth2, f0.auto_scale, f0gcd, f0.noergaard
* recompiled all wmax externals and stripped them.  much smaller filesizes

-----051228
* fixed divide by 0 bug in f0.gcd for windows
* removed f0.%.help from the windows distribution

-----051016
* compiled for windows using cygwin+gcc
* renamed f0.nørgård to f0.noergaard so it also will work under windows
* helpfiles - minor cleanup
* removed f0.% for osx.  from now on os9 only

-----040402
* new msp external f0.slub~
* minor changes to f0.range and f0.range2 helpfiles

-----040331
* new external f0.slub
* tiny bugfix f0.range2

-----040330
* new externals f0.range and f0.range2

-----040301
* new external f0.auto_scale - converted from abstraction (f0.abs package)
* new external f0.ratio - converted from abstraction called f0.ratio4-3 (f0.abs package)
* minor changes to f0.%, f0.beats_to_frames, f0.distance, f0.fold, f0.frames_to_beats, f0.gcd, f0.nørgård, f0.round, f0.smooth, f0.smooth2, f0.snap, f0.tune.  fixed so that they now all load in the 'Math' category in New Object List.

-----040226
* new external f0.round - converted from abstraction (f0.abs package).  works as before but with an added quantize value inlet that makes f0.fquantize and f0.quantize obsolete.
* converted f0.beats_to_frames from abstraction
* converted f0.frames_to_beats from abstraction
* converted f0.inc_dec_split from abstraction
* converted f0.limit_counter from abstraction

-----040225
* finally all externals updated for xmax and compiled with cw8.0
* helpfiles - fixed spelling/typos
* f0.construct helpfile - remake and added examples for jitter and softvns

-----010502
* first externals (f0.construct and f0.fold)