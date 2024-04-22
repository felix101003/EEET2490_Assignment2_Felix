#include "../utils/color.h"

char *colors[] = {
    "BLACK",
    "RED",
    "GREEN",
    "BLUE",
    "YELLOW",
    "CYAN",
    "MAGENTA",
    "WHITE"};

char *backgroundColors[] = {
    BLACK_BG,
    RED_BG,
    GREEN_BG,
    BLUE_BG,
    YELLOW_BG,
    CYAN_BG,
    MAGENTA_BG,
    WHITE_BG};

char *textColors[] = {
    BLACK_TEXT,
    RED_TEXT,
    GREEN_TEXT,
    BLUE_TEXT,
    YELLOW_TEXT,
    CYAN_TEXT,
    MAGENTA_TEXT,
    WHITE_TEXT};

void setColor(char *cmd)
{
    char *token = strtok(cmd, " ");

    // Skip the first 2 words "set color"
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");

    char *textColor = NULL;
    char *backgroundColor = NULL;

    // Count the number of received color from token
    int text_color = 0, background_color = 0;

    while (token != NULL)
    {
        // Extract the text color
        if (strcasecmp(token, "-t") == 0)
        {
            token = strtok(NULL, " ");
            text_color++;
            textColor = token;
        }

        // Extract the background color
        else if (strcasecmp(token, "-b") == 0)
        {
            token = strtok(NULL, " ");
            background_color++;
            backgroundColor = token;
        }

        // Handle invalid command to set color
        else
        {
            uart_puts("Invalid command to set color\n");
            return;
        }

        // Handle errors if there are multiple colors found in the command
        if (text_color > 1 || background_color > 1)
        {
            uart_puts("Multiple colors found. Please check your command.\n");
            return;
        }

        token = strtok(NULL, " ");
    }

    // Set the color for the text
    if (text_color == 1) {
        setTextColor(textColor);
    }

    // Set the color for the background
    if (background_color == 1) {
        setBackgroundColor(backgroundColor);
    }
}

void setBackgroundColor(char *color)
{
    for (int i = 0; i < MAX_COLOR_SIZE; i++)
    {
        if (strcasecmp(color, colors[i]) == 0)
        {
            uart_puts(backgroundColors[i]);
            return;
        }
    }
    uart_puts("Invalid color\n");
}

void setTextColor(char *color)
{
    for (int i = 0; i < MAX_COLOR_SIZE; i++)
    {
        if (strcasecmp(color, colors[i]) == 0)
        {
            uart_puts(textColors[i]);
            return;
        }
    }
    uart_puts("Invalid color\n");
}