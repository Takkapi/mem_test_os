#include "ui.h"
#include "shutdown.h"

void update_ui() {

}

char get_input() {
    return '\0';
}

void process_input(char input) {
    if(input == 'q') shutdown();
}

void init_screen() {
    // TODO
}

void display_progress_bar(int progress, int total) {
    // TODO
}

void display_memory_status(int verified, int total) {

}