#ifndef UI_H
#define UI_H

void update_ui(int progress, int total);
char get_input();
void process_input(char input);
void init_screen();
void display_message(const char* message);
void display_memory_status(int verified, int total);

#endif