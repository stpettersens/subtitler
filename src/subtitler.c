#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "subtitle.h"

void display_version() {
    printf("Subtitler v0.1.0\n");
}

void display_usage(char *program) {
    printf("Subtitler\n");
    printf("CLI timeshift and playback utility.\n");
    printf("Copyright 2025 Sam Saint-Pettersen.\n\n");
    printf("Released under the MIT License.\n\n");
    printf("Usage: %s [-h | -v | -f <input_srt_file> -o <output_srt_file>\n", program);
    printf("\t[-x <frame,frame...>] -t <time_offset> -u <time_unit> [-p]]\n\n");
    printf("\t-f: Path to input subtitles (SRT) file.\n");
    printf("\t-o: Path to output subtitles (SRT) file.\n");
    printf("\t-x: Frames (subtitles) to apply timeshift offset to (comma separated or range).\n");
    printf("\t-t: Time offset number (integer which is positive or negative).\n");
    printf("\t-u: Time unit (hrs, mins, secs or ms).\n");
    printf("\t-p: Playback the subtitles in real time (optional).\n");
    printf("\t-h: Display this help information and exit.\n");
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

    int c = -1;
    while ((c = getopt(argc, argv, "f:o:x:t:u:phv")) != -1) {
        switch (c) {
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
        printf("Error: -o requires a file path for output SRT file.\n\n");
        display_usage(argv[0]);
        return -1;
    }

    printf("frames = %s\n", frames);

    if (strcmp(unit, "hrs") == 0)
        offset_ms = (offset_unit * 3600000);

    else if (strcmp(unit, "mins") == 0)
        offset_ms = (offset_unit * 60000);

    else if (strcmp(unit, "secs") == 0)
        offset_ms = (offset_unit * 1000);

    else if (strcmp(unit, "ms") == 0)
        offset_ms = offset_unit;

    else {
        printf("Warning: Unknown unit, defaulting to ms.\n");
        offset_ms = offset_unit;
    }

    printf("offset_ms = %d\n", offset_ms);

    int status = process_subtitles
    (in_srt_file, out_srt_file, frame_count, iframes, offset_ms, op);

    return status;
}
