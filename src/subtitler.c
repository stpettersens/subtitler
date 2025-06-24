#include <stdio.h>
#include <getopt.h>
#include "subtitle.h"

//int main(int argc, char *argv[]) {
int main() {
    Op op = PLAYBACK;
    int status = process_subtitles("Parasite.2019.srt", "copy.srt", 0, NULL, (long)0, op);
    return status;
}
