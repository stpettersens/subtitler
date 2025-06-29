### subtitler
> CLI utility to timeshift subtitle (SRT) files.

```
Subtitler
CLI subtitles (SRT) timeshift and playback utility.
Copyright 2025 Sam Saint-Pettersen.

Released under the MIT License.

Usage: ./bin/subtitler [-h | -v | -f <input_srt_file> -o <output_srt_file>
        [-x <frame,frame...>] -t <time_offset> -u <time_unit> [-p]]

        -f: Path to input subtitles (SRT) file.
        -o: Path to output subtitles (SRT) file.
        -x: Frames (subtitles) to apply timeshift offset to (comma separated or range).
        -t: Time offset number (integer which is positive or negative).
        -u: Time unit (hrs, mins, secs or ms).
        -p: Playback the subtitles in real time (optional).
        -h: Display this help information and exit.
        -v: Display program version and exit.
```
