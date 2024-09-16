#ifndef CAPTURE_KEY_HPP
#define CAPTURE_KEY_HPP

#include <unistd.h>
#include <termios.h>

void set_terminal_mode(bool enableRaw) {
    static struct termios oldt, newt;

    if (enableRaw) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

char captureKey() {
    char ch;

    set_terminal_mode(true);
    ch = getchar();
    tcflush(STDIN_FILENO, TCIFLUSH);
    set_terminal_mode(false);

    return ch;
}

#endif