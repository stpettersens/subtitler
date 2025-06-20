#ifndef SUBTITLE_H
#define SUBTITLE_H

#define MAX_LINE_LENGTH 501

typedef struct {
    int sequence;
    char timestamps[30];
    char text[MAX_LINE_LENGTH];
    long start_ms;
    long end_ms;
} Subtitle;

typedef enum {
    SEQUENCE = 0,
    TIMESTAMPS = 1,
    TEXT = 2,
    END = 3
} SubtitleTag;

int process_subtitles_srt_file(char *in_srt_file, char *out_srt_file, long offset)

#endif
