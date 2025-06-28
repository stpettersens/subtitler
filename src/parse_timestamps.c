#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subtitle.h"

int parse_timestamp_ms(char *timestamp) {
    char *_ms = strtok(timestamp, ",");
    _ms = strtok(NULL, ",");
    int ms = atoi(_ms);

    char *hms = strtok(timestamp, ":");
    int hours = atoi(hms);

    hms = strtok(NULL, ":");
    int mins = atoi(hms);

    hms = strtok(NULL, ":");
    int secs = atoi(hms);

    int ms_hours = (hours * 3600000);
    int ms_mins = (mins * 60000);
    int ms_secs = (secs * 1000);

    int total_ms = (ms + ms_hours + ms_mins + ms_secs);

    return total_ms;
}

void parse_ms_timestamp(char *timestamp, int total_ms) {
    int hours = (total_ms / (1000 * 60 * 60));
    int ms = (total_ms - (hours * 1000 * 60 * 60));
    int mins = (ms / (1000 * 60));
    ms = (ms - (mins * 1000 * 60));
    int secs = (ms / 1000);
    ms = (ms - (secs * 1000));
    sprintf(timestamp, "%02d:%02d:%02d,%03d", hours, mins, secs, ms);
}

int parse_timestamps(Subtitle *subtitle, int offset_ms) {
    char start[13], end[13];
    char sts[13], ets[13];
    if (sscanf(subtitle->timestamps, "%12s --> %12s", start, end) == 2)
    {
        subtitle->start_ms = parse_timestamp_ms(start);
        subtitle->end_ms = parse_timestamp_ms(end);

        if (offset_ms != 0) {
            subtitle->start_ms += offset_ms;
            subtitle->end_ms += offset_ms;

            parse_ms_timestamp(sts, subtitle->start_ms);
            parse_ms_timestamp(ets, subtitle->end_ms);

            sprintf(subtitle->timestamps, "%s --> %s", sts, ets);
        }

        return 0; // Return 0 on success.
    }

    return -1; // Return -1 on parse error.
}
