#ifndef SUBTITLE_H
#define SUBTITLE_H

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

#define MAX_LINE_LENGTH 501
#define INTERVAL_MS 1           // 1 millisecond = 1000000 nanoseconds.

typedef struct {
    int sequence;
    char timestamps[30];
    char text[MAX_LINE_LENGTH];
    int start_ms;
    int end_ms;
} Subtitle;

typedef enum {
    SEQUENCE = 0,
    TIMESTAMPS = 1,
    TEXT = 2,
    END = 3
} SubtitleTag;

typedef enum {
    NO_EXTRA_OP = 0,
    PLAYBACK = 1,
} Op;

void cls();
void playback_subtitles(char *in_srt_file, int count, Subtitle *subtitles);
int get_subtitles_count(char *in_srt_file);

int process_subtitles
(char *in_srt_file, char *out_srt_file, int frame_count, int *frames, int offset_ms, Op op);

int parse_timestamp_ms(char *timestamp);
void parse_ms_timestamp(char *timestamp, int total_ms);
int parse_timestamps(Subtitle *subtitle, int offset_ms);

#endif
