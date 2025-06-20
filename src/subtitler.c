#include <stdio.h>
#include <getopt.h>
#include "subtitle.h"

//int main(int argc, char *argv[]) {
int main() {
    int status = process_subtitles_srt_file("Parasite.2019.srt", "copy.srt", (long)0);
    return status;
}
