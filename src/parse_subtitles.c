#ifdef _WIN32
#include <windows.h>
#else
#define _GNU_SOURCE // For CLOCK_MONOTONIC and TIMER_ABSTIME.
#endif

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

#ifdef _WIN32
// The function below is unsed on Windows.
#else
void add_ms_to_timespec(struct timespec *ts, int ms) {
    ts->tv_nsec += ms * 1000000;
    if (ts->tv_nsec >= 1000000000) {
        ts->tv_nsec -= 1000000000;
        ts->tv_sec += 1;
    }
}
#endif

void playback_subtitles(char *in_srt_file, int count, int offset_ms, Subtitle *subtitles) {
    char start[16], end[16];
    sscanf(subtitles[(count - 1)].timestamps, "%15s --> %15s", start, end);

    cls();

    printf("Playing back '%s' (Runtime: %s [%d ms]; Count = %d; Offset ms = %d)\n",
    in_srt_file, end, subtitles[(count - 1)].end_ms, count, offset_ms);

    sleep(3);
    cls();

#ifdef _WIN32
    LARGE_INTEGER freq, tstart, current;
    QueryPerformanceFrequency(&freq);  // Get ticks per second.
    QueryPerformanceFrequency(&tstart); // Start time.
#else
    struct timespec next;
    clock_gettime(CLOCK_MONOTONIC, &next); // Get current monotonic time.
#endif

    int i = 0;
    int timer = 0;
    while (i < count) {

#ifdef _WIN32
        QueryPerformanceCounter(&current);
        LONGLONG elapsed_ms = (((current.QuadPart - tstart.QuadPart) * 1000) / freq.QuadPart);
#endif
        if (timer == subtitles[i].start_ms) {
            printf("%s\n", subtitles[i].timestamps);
            printf("%s\n", subtitles[i].text);
        }
        else if (timer == subtitles[i].end_ms) {
            i++;
            cls();
        }

#ifdef _WIN32
        Sleep(1); // Sleep 1 ms.
        if (elapsed_ms >= timer) timer++;
#else
        add_ms_to_timespec(&next, INTERVAL_MS);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
        timer++;
#endif
    }
}

int get_subtitles_count(char *in_srt_file) {
    FILE *f = fopen(in_srt_file, "r");
    if (!f) {
        perror("Failed to open subtitles (SRT) file for counting.\n");
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

int process_subtitles
(char *in_srt_file, char *out_srt_file, int frame_count, int *frames, int offset_ms, Op op) {
    // Determine number of subtitles (count).
    int count = get_subtitles_count(in_srt_file);
    if (count == -1)
        return -1;

    // Allocate memory on heap for all subtitles.
    Subtitle *subtitles = malloc((sizeof(Subtitle) * count));

    // Parse subtitles.
    FILE *f = fopen(in_srt_file, "r");
    if (!f) {
        perror("Failed to open subtitles (SRT) file for reading.\n");
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
            int ch = line[j];
            if (!isdigit(ch)) {
                tag = TEXT;
                break;
            }
            else {
                tag = SEQUENCE;
            }
        }

        if (strstr(line, ":") != NULL && strstr(line, "-->") != NULL)
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

    // Timeshift subtitles as specified by given offset_ms (0 does not change).
    if (frame_count == 0) {
        // Timeshift every subtitle.
        for (int x = 0; x < count; x++) {
            if (parse_timestamps(&subtitles[x], offset_ms) == -1) {
                fprintf(stderr, "Error: Failed to convert subtitle time into milliseconds.");
                fclose(f);
                free(subtitles);
                return -1;
            }
        }
    }
    else {
        // Timeshift only subtitles for those given in frames array while fr < frame_count.
        for (int fr = 0; fr < frame_count; fr++) {
            if (frames[fr] > count) {
                fprintf(stderr, "Error: Provided frame %d is greater than number of subtitles.",
                frames[fr]);
                fclose(f);
                free(subtitles);
                return -1;
            }

            if (parse_timestamps(&subtitles[(frames[fr])], offset_ms) == -1) {
                fprintf(stderr, "Error: Failed to convert subtitle time into milliseconds.");
                fclose(f);
                free(subtitles);
                return -1;
            }
        }
    }

    fclose(f);

    if (offset_ms != 0)
    {
        // Write out time changes to out file.
        f = fopen(out_srt_file, "w");
        if (!f) {
            perror("Failed to open subtitles (SRT) file for writing.\n");
            return -1;
        }

        for (int y = 0; y < count; y++) {
            fprintf(f, "%d\n", subtitles[y].sequence);
            fprintf(f, "%s\n", subtitles[y].timestamps);
            fprintf(f, "%s\n", subtitles[y].text);
        }

        fclose(f);
    }

    // Any addtional option to run after timeshift, such as playback.
    switch (op) {
        case NO_EXTRA_OP:
            break;

        case PLAYBACK:
            playback_subtitles(in_srt_file, count, offset_ms, subtitles);
            break;
    }

    free(subtitles);

    return 0;
}
