#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;
char * choices[] = {
    "Choices 1",
    "Choices 2",
    "Choices 3",
    "Choices 4",
    "Exit",
};
int n_choices = sizeof(choices) / sizeof (char*);
void print_menu (WINDOW *menu_win, int hightlight);
void report_choice (int mouse_x, int mouse_y, int * p_choice);

#include <ncurses.h>

int main (int agrc, char *argv)
{
    WINDOW * menu_win;
    int choice = 0;
    int c;
    MEVENT event;

    initscr ();
    clear ();
    noecho ();
    cbreak ();
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    attron (A_REVERSE);
    mvprintw (23, 1, "Click on Exit to quit(Works best in a virtual console)");
    refresh ();
    attroff (A_REVERSE);
    /* 首先显示菜单 */
    menu_win = newwin (HEIGHT, WIDTH, starty, startx);
    print_menu (menu_win, 1);
    /* 监听所有的鼠标事件 */
    mousemask (ALL_MOUSE_EVENTS, NULL);
    while (1)
    {
        c = wgetch (menu_win);
        switch (c)
        {
        case KEY_MOUSE:

            if(getmouse(&event) == OK)
            {
                /* 用户按下鼠标左键 */
                if (event.bstate & BUTTON1_PRESSED)
                {
                    report_choice (event.x + 1, event.y + 1, &choice);
                    if (choice == -1)
                        goto end;
                    mvprintw (22, 1, "Choice made is : %d String  Chosen is \"%10s\"", choice, choices[choice - 1]);
                    refresh ();
                }

            }
            print_menu (menu_win, choice);
            break;
        }
    }
end:
    endwin ();
    return 0;

}


void print_menu (WINDOW *menu_win, int hightlight)
{
    int x, y, i;
    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i)
    {
        if (hightlight == i + 1)
        {
            wattron (menu_win, A_REVERSE);
            mvwprintw (menu_win, y, x, "%s", choices[i]);
            wattroff (menu_win, A_REVERSE);
        }
        else
            mvwprintw (menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh (menu_win);
}

void report_choice (int mouse_x, int mouse_y, int * p_choice)
{
    int i, j, choice;
    i = startx + 2;
    j = starty + 3;

    for (choice = 0; choice < n_choices; ++choice)
    {
        if(mouse_y == j + choice && mouse_x >= i &&
           mouse_x <= i + strlen (choices[choice]))
        {
            if (choice == n_choices -1)
                *p_choice = -1;
            else
                *p_choice = choice + 1;
            break;
        }

    }
}
