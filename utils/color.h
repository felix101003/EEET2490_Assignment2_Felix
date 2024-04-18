#ifndef COLOR_H
#define COLOR_H

#include "constant.h"
#include "../header/uart1.h"
#include "stringProcess.h"

void setColor(char* cmd);
void setBackgroundColor(char* color);
void setTextColor(char* color);

#endif