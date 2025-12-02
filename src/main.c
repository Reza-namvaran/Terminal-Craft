#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

static struct termios org_term, new_term;
static char key_status[256] = {0};

void init_terminal() {
    tcgetattr(STDIN_FILENO, &org_term);
    new_term = org_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
    fflush(stdout);
}

void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &org_term);
    printf("LOG: Terminal has been restored!\n");
}

void input_handler() {
    char ch;
    for (int i = 0; i < 256; i++) {
        key_status[i] = 0;
    }

    while (read(STDIN_FILENO, &ch, 1) > 0) {
        printf("key: %c\n", ch);
        unsigned char uch = (unsigned char) ch;
        key_status[uch] = 1;
    }
}

int is_key_pressed(char key) {
    return key_status[(unsigned char) key];
}

int main() {
    init_terminal();
    for (;;) {
        input_handler();
        if (is_key_pressed('q')) {
            exit(0);
        }
        usleep(20000);
    }
    restore_terminal();
    return 0;
}