#include "ascii_art.h"
#include "screen.h"

void print_system() {
    char c = 0x0B; 

    screen_print_color("                                 /$$                            \n", c);
    screen_print_color("                                | $$                            \n", c);
    screen_print_color("  /$$$$$$$ /$$   /$$  /$$$$$$$ /$$$$$$    /$$$$$$  /$$$$$$/$$$$ \n", c);
    screen_print_color(" /$$_____/| $$  | $$ /$$_____/|_  $$_/   /$$__  $$| $$_  $$_  $$\n", c);
    screen_print_color("|  $$$$$$ | $$  | $$|  $$$$$$   | $$    | $$$$$$$$| $$ \\ $$ \\ $$\n", c);
    screen_print_color(" \\____  $$| $$  | $$ \\____  $$  | $$ /$$| $$_____/| $$ | $$ | $$\n", c);
    screen_print_color(" /$$$$$$$/|  $$$$$$$ /$$$$$$$/  |  $$$$/|  $$$$$$$| $$ | $$ | $$\n", c);
    screen_print_color("|_______/  \\____  $$|_______/    \\___/   \\_______/|__/ |__/ |__/\n", c);
    screen_print_color("           /$$  | $$                                            \n", c);
    screen_print_color("          |  $$$$$$/                                            \n", c);
    screen_print_color("           \\______/                                             \n", c);
}

void print_logo() {
    char c = 0x0B;

    screen_print_color(" /$$$$$$$  /$$                     /$$        /$$$$$$   /$$$$$$ \n", c);
    screen_print_color("| $$__  $$|__/                    | $$       /$$__  $$ /$$__  $$\n", c);
    screen_print_color("| $$  \\ $$ /$$ /$$   /$$  /$$$$$$ | $$      | $$  \\ $$| $$  \\__/\n", c);
    screen_print_color("| $$$$$$$/| $$|  $$ /$$/ /$$__  $$| $$      | $$  | $$|  $$$$$$ \n", c);
    screen_print_color("| $$____/ | $$ \\  $$$$/ | $$$$$$$$| $$      | $$  | $$ \\____  $$\n", c);
    screen_print_color("| $$      | $$  >$$  $$ | $$_____/| $$      | $$  | $$ /$$  \\ $$\n", c);
    screen_print_color("| $$      | $$ /$$/\\  $$|  $$$$$$$| $$      |  $$$$$$/|  $$$$$$/\n", c);
    screen_print_color("|__/      |__/|__/  \\__/ \\_______/|__/       \\______/  \\______/ \n", c);
}
