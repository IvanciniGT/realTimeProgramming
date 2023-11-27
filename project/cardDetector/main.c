#include <stdio.h>
#include "card_detector.h"

int main() {
    printf("Card Detector Subsystem\n");
    int response = turn_on();
    return response;
}
