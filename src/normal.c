#include "../utils/normal.h"

char* commandsBriefInfo[] = {
    "For more information on a specific command, type \"help [command_name]\"\n",
    "Clear the screen\n",
    "Set text color, and/or background color of the console\n",
    "Display board revision and board MAC address\n",
    "Configure the baud rate of the UART\n",
    "Configure the length of data bits\n",
    "Configure the number of stop bits\n",
    "Configure the parity bit\n",
    "Configure handshake control\n",
    "Check the current configuration of the UART\n"
};

char* commandsFullInfo[] = {
    "help: This command offers brief descriptions of available commands. To obtain detailed information about a specific command, type \"help [command_name]\"\n",
    "clear: This command clears the screen and scrolls down to the current position of the cursor. \n",
    "set color: This command sets the text color and/or background color of the console. Use '-t' flag followed by the color to change text color, use '-b' flag followed by the color to change background color. Supported colors are: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE. \n",
    "showinfo: This command displays the revision and MAC address of the board you are currenly working with. \n",
    "baudrate: This command configures the baud rate of the UART. Type \"baudrate [baud_rate]\", baud_rate must be a non-zero positive integer.\n",
    "length: This command sets the length of data bits. Type \"length [number of data bits]\", number of data bits must be 5, 6, 7 or 8.\n",
    "stop: This command sets the number of stop bits. Type \"stop [number of stop bits]\", number of stop bits must be 1 or 2.\n",
    "parity: This command sets the parity bit. Type \"parity [parity bit]\", parity bit must be 'none', 'even' or 'odd'.\n",
    "handshake: This command configures the handshake control of the UART. Type \"handshake [status]\", status must be 0 or 1.\n",
    "check: This command checks the current configuration of the UART including UART's current baud rate, number of data bit, number of stop bit, parity bit and handshake control\n"
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
    token = strtok(NULL, " "); // Skip the first token "help"

    // If there is no command after "help", display brief help for all commands
    if (token == NULL) {
        displayBriefHelp(commands);
    }
    else {
        char helpCommand[MAX_CMD_SIZE]; // Buffer to store the command after "help"
        helpCommand[0] = '\0'; // Initialize the buffer
        
        while (token != NULL) {
            strcat(helpCommand, token);
            token = strtok(NULL, " ");

            if (token != NULL) {
                strcat(helpCommand, " ");
            }   
        }

        // Now check if the constructed help command matches any specific command
        for (int i = 0; i < MAX_COMMAND_SIZE; i++) {
            if (strcasecmp(helpCommand, commands[i]) == 0) {
                displayDetailHelp(commandsFullInfo[i]);
                return;
            }
        }

        uart_puts("Invalid help command\n");
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
        if (strlen(commands[i]) % 6 == 0) {
            numTabs++;
        }
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
    unsigned int *revision = (unsigned int *) &(mBox[5]);

    mBox[6] = 0x00010003; // Tag identifier: GET_BOARD_MAC
    mBox[7] = 6; // Value buffer size in bytes
    mBox[8] = 0; // Request code
    mBox[9] = 0; // Clear output buffer (response data is mBox[9])

    mBox[10] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBox), MBOX_CH_PROP)) {
        // Display board revision
        uart_puts("Board revision = ");
        uart_hex(*revision);

        // Display board model
        if (*revision == RPI3B_BCM2837_1G_SONYUK) {
            uart_puts(" (Board model: rpi-3B BCM2837 1GiB Sony UK)");
        } else if (*revision == RPI2B_BCM2836_1G_SONYUK) {
            uart_puts(" (Board model: rpi-2B BCM2836 1GiB Sony UK)");
        } else if (*revision == RPI4B_BCM2711_2G_SONYUK) {
            uart_puts(" (Board model: rpi-4B BCM2711 2GiB Sony UK)");
        } else if (*revision == RPI1B_PLUS_BCM2835) {
            uart_puts(" (Board model: rpi-1B+ BCM2835)");
        } else if (*revision == RPI0_BCM2835_512M_SONYUK) {
            uart_puts(" (Board model: rpi-Zero BCM2835 512MB Sony UK)");
        } else if (*revision == RPI3B_PLUS_BCM2837_1G_SONYUK) {
            uart_puts(" (Board model: rpi-3B+ BCM2837 1GiB Sony UK)");
        } else {
            uart_puts(" (Board model: Other model)");
        }

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
}

void deleteOneChar() {
    uart_puts("\b \b"); // Move cursor back, overwrite with space, move cursor back again
}

void update_baud_rate(char* buffer, int* baudrate) {
    char* token = strtok(buffer, " ");
    token = strtok(NULL, " "); // Skip the first token "baudrate"
    int count = 0;
    int baud_rate = -1;

    while (token != NULL) {
        if (count == 1) {
            uart_puts("Multiple baud rate. Please check your command\n");
            return;
        }

        baud_rate = atoi(token);
        count++;
        if (baud_rate <= 0) {
            uart_puts("Invalid baud rate\n");
            return;
        }

        token = strtok(NULL, " ");
    }
    if (baud_rate != -1) {
        *baudrate = baud_rate; // Dereference and assign the new value to baudrate
    }
}

void update_data_length(char* buffer, int* length) {

    char* token = strtok(buffer, " ");
    token = strtok(NULL, " "); // Skip the first token "length"
    int count = 0;
    int data_bit_length = -1;

    while (token != NULL) {
        if (count == 1) {
            uart_puts("Multiple data length. Please check your command\n");
            return;
        }

        data_bit_length = atoi(token);
        count++;
        if (data_bit_length != 5 && data_bit_length != 6 && data_bit_length != 7 && data_bit_length != 8) {
            uart_puts("Invalid data length\n");
            return;
        }

        token = strtok(NULL, " ");
    }
    if (data_bit_length != -1) {
        *length = data_bit_length; // Dereference and assign the new value to length
    }
}


void update_stop_bit(char* buffer, int* stop) {
    char* token = strtok(buffer, " ");
    token = strtok(NULL, " "); // Skip the first token "stop"
    int count = 0;
    int stop_bit = -1;

    while (token != NULL) {
        if (count == 1) {
            uart_puts("Multiple stop bit. Please check your command\n");
            return;
        }

        stop_bit = atoi(token);
        count++;
        if (stop_bit != 1 && stop_bit != 2) {
            uart_puts("Invalid stop bit\n");
            return;
        }

        token = strtok(NULL, " ");
    }
    if (stop_bit != -1) {
        *stop = stop_bit; // Dereference and assign the new value to stop
    }
}

void update_parity_bit(char* buffer, char** parity) {
    char* token = strtok(buffer, " ");
    token = strtok(NULL, " "); // Skip the first token "parity"
    int count = 0;
    char* parity_bit = NULL;

    while (token != NULL) {
        if (count == 1) {
            uart_puts("Multiple parity bit. Please check your command\n");
            return;
        }

        parity_bit = token;
        count++;
        if (strcasecmp(parity_bit, "none") != 0 && strcasecmp(parity_bit, "even") != 0 && strcasecmp(parity_bit, "odd") != 0) {
            uart_puts("Invalid parity bit\n");
            return;
        }

        token = strtok(NULL, " ");
    }
    if (parity_bit != NULL) {
        *parity = parity_bit; // Dereference and assign the new value to parity
    }
}