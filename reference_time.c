#include <stdio.h>
#include <time.h>

int main() {
    // Get the current Unix timestamp
    time_t start = time(NULL);
    printf("Start timestamp: %ld\n", start);

    // Simulate some delay (e.g. with sleep)
    printf("Sleeping for 5 seconds...\n");
    sleep(5);  // Use Sleep(5000) on Windows with <windows.h>

    // Get current timestamp again
    time_t end = time(NULL);
    printf("End timestamp: %ld\n", end);

    // Calculate elapsed time
    double elapsed = difftime(end, start);
    printf("Seconds passed: %.0f\n", elapsed);

    return 0;
}
