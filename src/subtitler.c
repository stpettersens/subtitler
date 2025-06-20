#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int status = process_subtitles_srt_file("Parasite.2019.srt", "copy.srt", (long)0);
    return status;
}
