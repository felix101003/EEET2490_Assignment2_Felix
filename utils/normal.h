// ----------------------------------- normal.h -------------------------------------
#ifndef NORMAL_H // Include guard to prevent multiple inclusions
#define NORMAL_H

#include "constant.h"
#include "../header/uart0.h"
#include "../header/mbox.h"
#include "../utils/stringProcess.h"

// Function prototypes
void displayWelcomeMessage();
void displayOS();
void displayHelp(char* clibuffer, char* commands[]);
void displayBriefHelp(char* commands[]);
void displayDetailHelp(char* detail);
void clear();
void showInfo();
void deleteChar(char* str);
void deleteOneChar();
void update_baud_rate(char* buffer, int* baudrate);
void update_data_length(char* buffer, int* length);
void update_stop_bit(char* buffer, int* stop);
void update_parity_bit(char* buffer, char** parity);

#endif // NORMAL_H