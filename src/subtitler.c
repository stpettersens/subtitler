#include <stdio.h>
#include <getopt.h>
#include "subtitle.h"

//int main(int argc, char *argv[]) {
int main() {
    Op op = PLAYBACK;

    int __offset_ms = (11 * 1000);

    int status = process_subtitles("Parasite.in.srt", "Parasite.srt", 0, NULL, __offset_ms, op);
    return status;
}
