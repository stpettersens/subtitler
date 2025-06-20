#include "subtitle.h"

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

int get_subtitles_count(char *srt_file) {
    FILE *f = fopen(srt_file, "r");
    if (!f) {
        perror("Failed to open subtitles (SRT) file.\n");
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

int process_subtitles_srt_file(char *in_srt_file, char *out_srt_file, long offset) {
    // Determine number of subtitles (count).
    int count = get_subtitles_count(in_srt_file);
    if (count == -1)
        return -1;

    system(CLEAR_SCREEN); // !TODO
    printf("Number of subtitles (count) = %d\n\n", count);
    sleep(1); // !TODO

    // Allocate memory on heap for all subtitles.
    Subtitle *subtitles = malloc((sizeof(Subtitle) * count));

    FILE *f = fopen(srt_file, "r");
    if (!f) {
        perror("Failed to open subtitles (SRT) file.\n");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    char final_text[MAX_LINE_LENGTH];
    char text[(MAX_LINE_LENGTH / 2)];

    int i = 0;
    SubtitleTag tag = END;
    while (fgets(line, sizeof(line), f)) {
        // Set tag type based on content:
        // SEQUENCE, TIMESTAMPS or TEXT.
        for (int j = 0; line[j] != '\n'; j++) {
            if (!isdigit((char)line[j])) {
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
            memset(text, 0, sizeof(text));
            sscanf(line, "%500[^\n]", text);
            strcat(final_text, text);
            strcat(final_text, "\n");
        }

        // Next subtitle.
        else if (tag == END) {
            strcpy(subtitles[i].text, final_text);
            memset(final_text, 0, sizeof(final_text));
            i++;
        }

        tag = END;
    }

    for (int x = 0; x < count; x++) {
        if (parse_timestamps(&subtitles[x]) == -1) {
            fprintf(stderr, "Error: Failed to convert subtitle into milliseconds.");
            return -1;
        }
    }

    for (int y = 0; y < count; y++) {
        //sleep((unsigned int)subtitles[y].start_ms);
        sleep(3);
        system(CLEAR_SCREEN);
        printf("%s\n", subtitles[y].timestamps);
        printf("%d --> %d (ms)\n", subtitles[y].start_ms, subtitles[y].end_ms);
        printf("%s\n", subtitles[y].text);
    }

    free(subtitles);
    fclose(f);

    return 0;
}
