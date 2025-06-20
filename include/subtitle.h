#ifndef SUBTITLE_H
#define SUBTITLE_H

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

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

typedef enum {
    PLAYBACK = 0
} SubtitleOp;

int get_subtitles_count(char *in_srt_file);
int process_subtitles_srt_file(char *in_srt_file, char *out_srt_file, long offset_ms, SubtitleOp op);
long parse_timestamp_ms(char *timestamp);
void parse_ms_timestamp(char *timestamp, long total_ms);
int parse_timestamps(Subtitle *subtitle, long offset_ms);

#endif
