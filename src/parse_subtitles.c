#include <math.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "subtitle.h"

void cls() {
    system(CLEAR_SCREEN);
}

void playback_subtitles(char *in_srt_file, int count, Subtitle *subtitles) {
    char start[16], end[16];
    sscanf(subtitles[(count - 1)].timestamps, "%15s --> %15s", start, end);

    cls();

    printf("Playing back '%s' (Runtime: %s [%ld ms])\n",
    in_srt_file, end, subtitles[(count - 1)].end_ms);

    sleep(3);
    cls();

    long f = 0;
    long timer = 999;
    while (f < count) {
        if (timer == subtitles[f].start_ms) {
            printf("%s\n", subtitles[f].timestamps);
            printf("%s\n", subtitles[f].text);
        }
        else if (timer == subtitles[f].end_ms) {
            f++;
            cls();
        }
        usleep(1000);
        timer += 1;
    }
}

int get_subtitles_count(char *in_srt_file) {
    FILE *f = fopen(in_srt_file, "r");
    if (!f) {
        perror("Failed to open subtitles (SRT) file.");
        return -1;
    }
    int count, last = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d", &count) == 1) {
            // Ensure we get subtitle count and not parts of timestamps or text
            // by checking against last value.
            if (count < last) count = last;
            last = count;
        }
    }
    fclose(f);
    return count;
}

int process_subtitles(char *in_srt_file, char *out_srt_file, long offset_ms, SubtitleOp op) {
    // Determine number of subtitles (count).
    int count = get_subtitles_count(in_srt_file);
    if (count == -1)
        return -1;

    // Allocate memory on heap for all subtitles.
    Subtitle *subtitles = malloc((sizeof(Subtitle) * count));

    FILE *f = fopen(in_srt_file, "r");
    if (!f) {
        perror("Failed to open subtitles (SRT) file.\n");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    char final_text[MAX_LINE_LENGTH];
    char text[((MAX_LINE_LENGTH / 2) + 1)];

    // Ensure final text buffer starts clear.
    memset(final_text, 0, sizeof(final_text));

    int i = 0;
    SubtitleTag tag = END;
    while (fgets(line, sizeof(line), f)) {
        // Set tag type based on content:
        // SEQUENCE, TIMESTAMPS or TEXT.
        for (int j = 0; line[j] != '\n'; j++) {
            if (!isdigit(line[j])) {
                tag = TEXT;
                break;
            }
            else {
                tag = SEQUENCE;
            }
        }

        if (strstr(line, ":") && strstr(line, "-->"))
            tag = TIMESTAMPS;

        // Add sequence number.
        if (tag == SEQUENCE) {
            sscanf(line, "%d", &subtitles[i].sequence);
        }

        // Add timestamps.
        else if (tag == TIMESTAMPS) {
            sscanf(line, "%29[^\n]", subtitles[i].timestamps);
        }

        // Add text.
        else if (tag == TEXT) {
            // Clear current text buffer first.
            memset(text, 0, sizeof(text));
            sscanf(line, "%250[^\n]", text);
            strcat(final_text, text);
            strcat(final_text, "\n");
        }

        // Next subtitle.
        else if (tag == END) {
            strcpy(subtitles[i].text, final_text);
            // Clear final text buffer for next subtitle's text.
            memset(final_text, 0, sizeof(final_text));
            i++;
        }

        tag = END;
    }

    for (int x = 0; x < count; x++) {
        if (parse_timestamps(&subtitles[x], offset_ms) == -1) {
            fprintf(stderr, "Error: Failed to convert subtitle into milliseconds.");
            return -1;
        }
    }

    switch (op) {
        case NO_EXTRA_OP:
            break;

        case PLAYBACK:
            playback_subtitles(in_srt_file, count, subtitles);
            break;
    }

    free(subtitles);
    fclose(f);

    return 0;
}
