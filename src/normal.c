#include "../utils/normal.h"

char* commandsBriefInfo[] = {
    "For more information on a specific command, type \"help [command_name]\"\n",
    "Clear the screen\n",
    "Set text color, and/or background color of the console\n",
    "Display board revision and board MAC address\n"
};

char* commandsFullInfo[] = {
    "help: This command offers brief descriptions of available commands. To obtain detailed information about a specific command, type \"help [command_name]\"\n",
    "clear: This command clears the screen and scrolls down to the current position of the cursor. \n",
    "set color: This command sets the text color and/or background color of the console. Use '-t' flag followed by the color to change text color, use '-b' flag followed by the color to change background color. Supported colors are: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE. \n",
    "showinfo: This command displays the revision and MAC address of the board you are currenly working with. \n"
};

// Display welcome message
void displayWelcomeMessage() {
    uart_puts(WELCOME_MESSAGE);
}

// Display the OS name
void displayOS() {
    uart_puts(INTIAL_NAME);
}


// Display help
void displayHelp(char* clibuffer, char* commands[]) {
    char* token = strtok(clibuffer, " ");
    if (strcasecmp(token, HELP) == 0) {
        token = strtok(NULL, " "); // Move to the next token
        if (token == NULL) {
            displayBriefHelp(commands);
        } else {
            char helpCommand[MAX_CMD_SIZE]; // Buffer to store the command after "help"
            strcpy(helpCommand, token); // Copy the command to the buffer
            
            // Concatenate subsequent tokens into the help command
            token = strtok(NULL, " ");
            if (token != NULL) {
                strcat(helpCommand, " ");
                strcat(helpCommand, token);
            }

            // Now check if the constructed help command matches any specific command
            for (int i = 0; i < MAX_COMMAND_SIZE; i++) {
                if (strcasecmp(helpCommand, commands[i]) == 0) {
                    displayDetailHelp(commandsFullInfo[i]);
                    break;
                }
            }
        }
    }
}

// Display brief help for all commands
void displayBriefHelp(char* commands[]) {
    uart_puts(commandsBriefInfo[0]);

    // Find the length of the longest command
    int maxCommandLength = 0;
    for (int i = 1; commands[i] != NULL && i < MAX_COMMAND_SIZE; i++) {
        int commandLength = strlen(commands[i]);
        if (commandLength > maxCommandLength) {
            maxCommandLength = commandLength;
        }
    }

    // Output commands and their brief info with consistent tabulation
    for (int i = 1; commands[i] != NULL && i < MAX_COMMAND_SIZE; i++) {
        uart_puts(commands[i]);
        // Calculate the number of tabs needed to align with the longest command
        int numTabs = (maxCommandLength - strlen(commands[i])) / 4 + 2; // assuming each tab is 4 characters wide
        for (int j = 0; j < numTabs; j++) {
            uart_puts("\t");
        }
        uart_puts(commandsBriefInfo[i]);
    }
}

// Display detailed help for a specific command
void displayDetailHelp(char* detail) {
    uart_puts(detail);        
}

// Clear the terminal
void clear() {
    uart_puts("\033[2J\033[H");
}

void showInfo() {
    mBox[0] = 11 * 4; // Buffer size in bytes
    mBox[1] = MBOX_REQUEST;

    mBox[2] = 0x00010002; // Tag identifier: GET_BOARD_REVISION
    mBox[3] = 4; // Value buffer size in bytes
    mBox[4] = 0; // Request code
    mBox[5] = 0; // Clear output buffer (response data is mBox[5])

    mBox[6] = 0x00010003; // Tag identifier: GET_BOARD_MAC
    mBox[7] = 6; // Value buffer size in bytes
    mBox[8] = 0; // Request code
    mBox[9] = 0; // Clear output buffer (response data is mBox[9])

    mBox[10] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBox), MBOX_CH_PROP)) {
        // Display board revision
        uart_puts("Board revision = ");
        uart_hex(mBox[5]);

        // Display board MAC address
        uart_puts("\nBoard MAC address = ");
        unsigned char* macBytes = (unsigned char*) &mBox[9];
        for (int i = 0; i <= 5; i++)
        {
            uart_hex_byte(macBytes[i]);
            if (i != 5)
                uart_sendc(':');
        }
        uart_puts("\n");
    }
}
void deleteChar(char* buffer) {
    int length = strlen(buffer);
    for (int i = 0; i < length; i++) {
        uart_puts("\b \b"); // Move cursor back, overwrite with space, move cursor back again
    }
    buffer[length] = '\0'; // Null-terminate the string to effectively delete the last character
}
