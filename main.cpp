#include <cstdio>
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main() {
    stdio_init_all();
    char in[40];
    printf("Ready when you are!");

    while (true) {
        scanf("%s", in);
        printf("Hello World and %s\n", in);
    }
    return 0;
}
#pragma clang diagnostic pop