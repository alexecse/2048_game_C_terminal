#include <stdio.h>

void playBeep() {
    // Folosește caracterul de control ASCII pentru a produce un sunet de beep
    printf("\a");
}

int main() {
    playBeep();
}