// ----------------------------------- normal.h -------------------------------------
#ifndef NORMAL_H // Include guard to prevent multiple inclusions
#define NORMAL_H

#include "constant.h"
#include "../header/uart1.h"
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

#endif // NORMAL_H