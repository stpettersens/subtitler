long parse_timestamp_ms(char *timestamp) {
    char *_ms = strtok(timestamp, ",");
    _ms = strtok(NULL, ",");
    long int ms = atol(_ms);

    char *hms = strtok(timestamp, ":");
    long hours = atol(hms);

    hms = strtok(NULL, ":");
    long mins = atol(hms);

    hms = strtok(NULL, ":");
    long secs = atol(hms);

    long ms_hours = (hours * 3600000);
    long ms_mins = (mins * 60000);
    long ms_secs = (secs * 1000);

    long total_ms = (ms + ms_hours + ms_mins + ms_secs);
    return total_ms;
}

void parse_ms_timestamp(char *timestamp, long total_ms) {
    long hours = (long)floor(total_ms / (1000 * 60 * 60));
    long ms = (total_ms - (hours * 1000 * 60 * 60));
    long mins = (long)floor(ms / (1000 * 60));
    ms = (ms - (mins * 1000 * 60));
    long secs = (long)floor(ms / 1000);
    ms = (ms - (secs * 1000));
    sprintf(timestamp, "%2d:%2d:%d2,%3d", hours, mins, secs, ms);
}

int parse_timestamps(Subtitle *subtitle, long offset) {
    char start[16], end[16];
    char sts[12], ets[12];
    if (sscanf(subtitle->timestamps, "%15s --> %15s", start, end) == 2)
    {
        subtitle->start_ms = parse_timestamp_ms(start);
        subtitle->end_ms = parse_timestamp_ms(end);
        if (offset != 0) {
            subtitle->start_ms += offset;
            subtitle->end += offset;
            parse_ms_timestamp(sts, subtitle->start_ms);
            parse_ms_timestamp(ets, subtitle->end_ms);
            memset(subtitle->timestamps, 0, sizeof(subtitle->timestamps));
            sprintf(subtitle->timestamps, "%s --> %s", sts, ets);
            return 1; // Return 1 success and offset modified.
        }
        return 0; // Return 0 success and no offset applied.
    }
    return -1; // Return -1 on parse error.
}
