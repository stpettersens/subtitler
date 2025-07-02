### subtitler
> CLI utility to timeshift subtitle (SRT) files.

```
Subtitler
CLI subtitles (SRT) timeshift and playback utility.
Copyright 2025 Sam Saint-Pettersen.

Released under the MIT License.

Usage: C:\Users\Sam\Projects\GitHub\subtitler\bin\subtitler.exe [-h | -v | [-q] -f <input_srt_file> [-o <output_srt_file>]
        [-x <frame,frame...>] [-t <time_offset>] [-u <time_unit>] [-p]]

        -q: Quiet mode; only print errors not warnings or most info (optional: has no effect on -p).
        -f: Path to input subtitles (SRT) file.
        -o: Path to output subtitles (SRT) file (optional: if not specifed, uses input as output).
        -x: Frames (subtitles) to apply timeshift offset to (optional: comma separated or inclusive range (e.g. 1-5).
        -t: Time offset number (optional: integer which is positive or negative).
        -u: Time unit (optional: defaults to 'ms' if unspecifed; can be 'hrs', 'mins', 'secs' or 'ms').
        -p: Playback the subtitles in real time (optional).

        -h: Display this help information and exit.
        -v: Display program version and exit
```
