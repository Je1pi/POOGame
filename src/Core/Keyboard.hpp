#ifndef JOGO_WAR_KEYBOARD_H
#define JOGO_WAR_KEYBOARD_H

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

class KeyBoard {
public:
    static void setNonBlocking(bool enable) {
        struct termios t{};
        tcgetattr(STDIN_FILENO, &t);  // Obtém as configurações do terminal

        if (enable) {
            t.c_lflag &= ~ICANON;  // Desativa o modo canônico
            t.c_lflag &= ~ECHO;    // Desativa a exibição da tecla pressionada
            tcsetattr(STDIN_FILENO, TCSANOW, &t);

            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);  // Torna o stdin não bloqueante
        } else {
            t.c_lflag |= ICANON;  // Reativa o modo canônico
            t.c_lflag |= ECHO;    // Reativa a exibição da tecla
            tcsetattr(STDIN_FILENO, TCSANOW, &t);

            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);  // Volta ao modo bloqueante
        }
    }

    static bool readInput(char &input) {
        return read(STDIN_FILENO, &input, 1) == 1;
    }
};

#endif //JOGO_WAR_KEYBOARD_H