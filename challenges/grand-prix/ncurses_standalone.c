#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

void printBorder();
void initCursor();
void delay(int);
void keyStrokeMove();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();
void printInstructions();

struct position {
    int Y;
    int X;
};

struct position pacManPosition = {10, 20};

int main() {


    initscr();
    raw();

    printBorder();
    printInstructions();
    initCursor();

    keyStrokeMove();

    endwin();

    return 0;
}

void keyStrokeMove(){
    int ch;
    do{
        ch = getch();
        switch (ch) {
            case 'w':
                moveUp();
                break;
            case 's':
                moveDown();
                break;
            case 'a':
                moveLeft();
                break;
            case 'd':
                moveRight();
                break;
            default:
                ch = 'q';
                break;
        }
    } while(ch != 'q');
}

void moveUp(){

}

void moveDown(){}

void moveLeft(){}

void moveRight(){}

void printInstructions(){
    mvprintw(0, 80, "* * I N S T R U C T I O N S * *");
    mvprintw(1, 80, "*   Move Up      =  w         *");
    mvprintw(2, 80, "*   Move Down    =  s         *");
    mvprintw(3, 80, "*   Move Left    =  a         *");
    mvprintw(4, 80, "*   Move Right   =  d         *");
    mvprintw(5, 80, "*   Quit         =  q         *");
    mvprintw(6, 80, "* * * * * * * * * * * * * * * *");
}

void delay(int secs){
    int millis = 1000 * secs;
    clock_t start_time = clock();
    while(clock() < start_time + millis);
}

void initCursor(){
    start_color();
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(2) | A_BOLD);

    mvaddch(pacManPosition.Y, pacManPosition.X, 'O');

    attroff(COLOR_PAIR(2) | A_BOLD);
}

void printBorder(){
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLUE);
    attron(COLOR_PAIR(1) | A_STANDOUT);
    mvprintw(0, 0, "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
    for (int y = 1; y < 20; ++y){
        mvaddch(y, 0,'*');
        mvaddch(y, 78, '*');
    }
    mvprintw(20, 0, "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
    attroff(COLOR_PAIR(1) | A_STANDOUT);
}
