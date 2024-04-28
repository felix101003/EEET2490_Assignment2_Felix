// -----------------------------------color.c -------------------------------------
#include "../utils/color.h"

// Define the set of colors available
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

// Define the set of background colors
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

// Define the set of text colors
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

// Function to extract the color from the command
void setColor(char *cmd)
{
    char *token = strtok(cmd, " ");

    // Skip the first word "setcolor"
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

// Function to set the background color
void setBackgroundColor(char *color)
{
    // Loop through the colors to find the matching color
    for (int i = 0; i < MAX_COLOR_SIZE; i++)
    {
        if (strcasecmp(color, colors[i]) == 0)
        {
            uart_puts(backgroundColors[i]);
            return;
        }
    }

    // Handle invalid color
    uart_puts("Invalid color\n");
}

// Function to set the text color
void setTextColor(char *color)
{
    // Loop through the colors to find the matching color
    for (int i = 0; i < MAX_COLOR_SIZE; i++)
    {
        if (strcasecmp(color, colors[i]) == 0)
        {
            uart_puts(textColors[i]);
            return;
        }
    }
    // Handle invalid color
    uart_puts("Invalid color\n");
}