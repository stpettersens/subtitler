#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "subtitle.h"

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

int process_subtitles_srt_file(char *in_srt_file, char *out_srt_file, long offset_ms, SubtitleOp op) {
    printf("Will output to '%s'.\n", out_srt_file);

    // Determine number of subtitles (count).
    int count = get_subtitles_count(in_srt_file);
    if (count == -1)
        return -1;

    system(CLEAR_SCREEN); // !TODO
    printf("Number of subtitles (count) = %d\n\n", count);
    sleep(5);

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

    if (op == PLAYBACK) {
        long s = 0;
        int y = 0;
        system(CLEAR_SCREEN);
        while (y < count) {
            if (s == (subtitles[y].start_ms / 1000)) {
                printf("%s\n", subtitles[y].timestamps);
                printf("%ld --> %ld (ms)\n", subtitles[y].start_ms, subtitles[y].end_ms);
                printf("%s\n", subtitles[y].text);
            }
            else if (s == (subtitles[y].end_ms / 1000)) {
                system(CLEAR_SCREEN);
                y++;
            }
            sleep(1);
            s++;
        }
    }

    free(subtitles);
    fclose(f);

    return 0;
}
