#include "../utils/color.h"

char *colors[] = {
    "BLACK",
    "RED",
    "GREEN",
    "BLUE",
    "YELLOW",
    "CYAN",
    "MAGENTA",
    "WHITE"
};

char *backgroundColors[] = {
    BLACK_BG,
    RED_BG,
    GREEN_BG,
    BLUE_BG,
    YELLOW_BG,
    CYAN_BG,
    MAGENTA_BG,
    WHITE_BG
};

 char *textColors[] = {
    BLACK_TEXT,
    RED_TEXT,
    GREEN_TEXT,
    BLUE_TEXT,
    YELLOW_TEXT,
    CYAN_TEXT,
    MAGENTA_TEXT,
    WHITE_TEXT
};

void setColor(char* cmd) {
    char *token = strtok(cmd, " ");
    char *textColor = NULL;
    char *backgroundColor = NULL;

    while (token != NULL)
    {
        if (strcasecmp(token, "-t") == 0)
        {
            token = strtok(NULL, " ");
            textColor = token;
        }
        else if (strcasecmp(token, "-b") == 0)
        {
            token = strtok(NULL, " ");
            backgroundColor = token;
        }
        token = strtok(NULL, " ");
    }
    setBackgroundColor(backgroundColor);
    setTextColor(textColor);
}

void setBackgroundColor(char* color) {
    for (int i = 0; i < MAX_COLOR_SIZE; i++)
    {
        if (strcasecmp(color, colors[i]) == 0)
        {
            uart_puts(backgroundColors[i]);
            return;
        }
    }
}

void setTextColor(char* color) {
    for (int i = 0; i < MAX_COLOR_SIZE; i++)
    {
        if (strcasecmp(color, colors[i]) == 0)
        {
            uart_puts(textColors[i]);
            return;
        }
    }
}