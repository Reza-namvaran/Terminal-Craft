#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#define X_PIXELS 400
#define Y_PIXELS 200
#define X_BLOCKS 20
#define Y_BLOCKS 20
#define Z_BLOCKS 10

static struct termios org_term, new_term;
static char key_status[256] = {0};

typedef struct Vector {
    double x;
    double y;
    double z;
} vect;

typedef struct Camera {
    double yaw;
    double pitch;
} cam;

typedef struct PlayerPos {
    vect pos;
    cam view; 
} player_pos_view;

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

char** init_picture() {
    char** picture = malloc(sizeof(char*) * Y_PIXELS);
    for (int i = 0; i < Y_PIXELS; i++) {
        picture[i] = malloc(sizeof(char) * X_PIXELS);
    }


    return picture;
}

char*** init_block() {
    char*** block = malloc(sizeof(char**) * Z_BLOCKS);
    for (int i = 0; i < Z_BLOCKS; i++) {
        block[i] = malloc(sizeof(char*) * Y_BLOCKS);
        for (int j = 0; j < Y_BLOCKS; j++) {
            block[i][j] = malloc(sizeof(char) * X_BLOCKS);
            for (int k = 0; k < X_BLOCKS; k++) {
                block[i][j][k] = ' ';
            }
        }
    }

    return block;
}

player_pos_view init_pos_view() {
    player_pos_view posView;
    
    posView.pos.x = 5;
    posView.pos.y = 5;
    posView.pos.z = 5;

    posView.view.yaw = 0;
    posView.view.pitch = 0;
    
    return posView;
}

int main() {
    init_terminal();
    char** picture = init_picture();
    char*** block = init_block();
    player_pos_view posView = init_pos_view();
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