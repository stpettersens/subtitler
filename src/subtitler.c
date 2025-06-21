#include <stdio.h>
#include <getopt.h>
#include "subtitle.h"

//int main(int argc, char *argv[]) {
int main() {
    SubtitleOp op = PLAYBACK;
    int status = process_subtitles("Parasite.2019.srt", "copy.srt", (long)0, op);
    return status;
}
