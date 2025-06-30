#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "subtitle.h"

void display_version() {
    printf("Subtitler v0.1.0\n");
}

void display_usage(char *program) {
    printf("Subtitler\n");
    printf("CLI subtitles (SRT) timeshift and playback utility.\n");
    printf("Copyright 2025 Sam Saint-Pettersen.\n\n");
    printf("Released under the MIT License.\n\n");
    printf("Usage: %s [-h | -v | [-q] -f <input_srt_file> [-o <output_srt_file>]\n", program);
    printf("\t[-x <frame,frame...>] [-t <time_offset>] [-u <time_unit>] [-p]]\n\n");
    printf("\t-q: Quiet mode; only print errors not warnings or most info (optional: has no effect on -p).\n");
    printf("\t-f: Path to input subtitles (SRT) file.\n");
    printf("\t-o: Path to output subtitles (SRT) file (optional: if not specifed, uses input as output).\n");
    printf("\t-x: Frames (subtitles) to apply timeshift offset to (optional: comma separated or range).\n");
    printf("\t-t: Time offset number (optional: integer which is positive or negative).\n");
    printf("\t-u: Time unit (optional: defaults to 'ms' if unspecifed; can be 'hrs', 'mins', 'secs' or 'ms').\n");
    printf("\t-p: Playback the subtitles in real time (optional).\n");
    printf("\n\t-h: Display this help information and exit.\n");
    printf("\t-v: Display program version and exit.\n");
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Error: Please provide necessary options and arguments.\n\n");
        display_usage(argv[0]);
        return -1;
    }

    char *in_srt_file = NULL;
    char *out_srt_file = NULL;
    int frame_count = 0;
    int offset_unit = 0;
    int offset_ms = 0;

    char unit[MAX_UNIT];
    char frames[MAX_FRAMES];
    int iframes[MAX_FRAMES];

    Op op = NO_EXTRA_OP;

    bool verbose = true;

    int c = -1;
    while ((c = getopt(argc, argv, "qf:o:x:t:u:phv")) != -1) {
        switch (c) {
            case 'q':
                verbose = false;
                break;

            case 'f':
                in_srt_file = optarg;
                break;

            case 'o':
                out_srt_file = optarg;
                break;

            case 'x':
                strcpy(frames, optarg);
                break;

            case 't':
                offset_unit = atoi(optarg);
                break;

            case 'u':
                strcpy(unit, optarg);
                break;

            case 'p':
                op = PLAYBACK;
                break;

            case 'h':
                display_usage(argv[0]);
                return 0;

            case 'v':
                display_version();
                return 0;
        }
    }

    if (in_srt_file == NULL) {
        printf("Error: -f requires a file path for input SRT file.\n\n");
        display_usage(argv[0]);
        return -1;
    }

    if (out_srt_file == NULL) {
        out_srt_file = in_srt_file;
        if (verbose && offset_unit != 0) {
            printf("Warning: Modifying input file in-place as a timeshift offset was set.\n");
        }
        if (offset_unit != 0) {
            printf("Ctrl+C within 5 seconds to cancel.\n");
            sleep(5);
            cls();
        }
    }

    // !TODO set frames as specified by command line.
    // This will include frame_counter++ in the operation.

    if (strcmp(unit, "hrs") == 0)
        offset_ms = (offset_unit * 3600000);

    else if (strcmp(unit, "mins") == 0)
        offset_ms = (offset_unit * 60000);

    else if (strcmp(unit, "secs") == 0)
        offset_ms = (offset_unit * 1000);

    else if (strcmp(unit, "ms") == 0)
        offset_ms = offset_unit;

    else
        offset_ms = offset_unit;

    // Print what the offset operation will do to the output when verbose.
    if (verbose && offset_ms != 0) {
        if (frame_count != 0)
            printf("Timeshifting selected subtitles by %d ms.", offset_ms);
        else
            printf("Timeshifting all subtitles by %d ms.", offset_ms);
    }

    int status = process_subtitles
    (in_srt_file, out_srt_file, frame_count, iframes, offset_ms, op);

    return status;
}
