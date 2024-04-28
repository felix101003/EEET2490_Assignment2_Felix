// -----------------------------------kernel.c -------------------------------------
#include "../utils/normal.h"
#include "../utils/stringProcess.h"
#include "../utils/color.h"


// Define the set of commands available
char* commands[] = {
    HELP,
    CLEAR,
    SET_COLOR,
    SHOW_INFO,
    BAUD_RATE,
    LENGTH,
    STOP,
    PARITY,
    HANDSHAKE,
    CHECK
};

// Set up the UART by default: baud rate = 115200, data bit length = 8, stop bit = 1, parity bit = none, turn off CTS/RTS handshake
int baud_rate = 115200;
int data_bit_length = 8;
int stop_bit = 1;
int parity_bit = 0;
int handshake = 0;

// Index for command and commandHistory
int current_index = 0;
int history_index = 0;

// Flag to overwrite the history when it reaches the limit capacity
int isRenew = 0;

// Flag to update UART configuration
int isUARTUpdate = 0;

void cli()
{   
    // Define the maximum size of the command
    static char cli_buffer[MAX_CMD_SIZE];
    static char history[MAX_HISTORY][MAX_CMD_SIZE];
    static int index = 0;
    static int command_index = 0;
    char c;
    int isUpLim = 0, isDownLim = 1;

    // Reset the buffer
    reset(cli_buffer);

    // The initial variable will prevent the command history from looping
    int initial = current_index;

    // Display OS name
    displayOS();

    // If the UART configuration is updated, wait for 1000000 cycles before reinitializing UART. This will help to maintain the OS name before reinitializing UART
    if (isUARTUpdate == 1) {
        unsigned int waitTime = 1000000;
        while (waitTime--) {
            asm volatile("nop");
        }
        uart_init(baud_rate, data_bit_length, stop_bit, parity_bit, handshake);
        isUARTUpdate = 0;
    }
    
    // Loop to receive characters from keyboard
    while (1) {
        c = uart_getc();
        
        // Find all commands that match the cli_buffer and add to match array
        char* match[MAX_COMMAND_SIZE];
        int match_index = 0;
        for (int i = 0; i < MAX_COMMAND_SIZE; i++) {
            if (strncasecmp(cli_buffer, commands[i], strlen(cli_buffer)) == 0) {
                match[match_index] = commands[i];
                match_index++;
            }
        } 
        
        // If tab is pressed, display the next matching command (only if there is a match)
        if (c == '\t') {
            if (index > 0) {
                if (match_index > 0) {
                    if (command_index == 0) {
                        deleteChar(cli_buffer);
                    } else {
                        deleteChar(match[(command_index-1) % (match_index)]);
                    }

                    index = strlen(match[command_index % match_index]);
                    uart_puts(match[command_index % match_index]); 
                    command_index++;
                }
            }
        // UP history
        } else if (c == '_') {
            if (isUpLim == 0) {
                if (command_index > 0) {
                    strcpy(cli_buffer, match[(command_index-1) % match_index]);
                    index = strlen(cli_buffer);
                    command_index = 0;
                }

                if (current_index == history_index)
                {
                    // If currently at the most recent command, save current input first
                    strcpy(history[history_index % MAX_HISTORY], cli_buffer);
                }
                deleteChar(cli_buffer);
                reset(cli_buffer);
                current_index = (current_index - 1 + MAX_HISTORY) % MAX_HISTORY;
                strcpy(cli_buffer, history[current_index]);
                index = strlen(history[current_index]);
                uart_puts(history[current_index]);
                isDownLim = 0;
            }
            
            // Prevent looping or end of the history
            if ((current_index == ((initial + 1) % MAX_HISTORY) && isRenew == 1) || (isRenew == 0 && current_index == 0)) {
                isUpLim = 1;
            }
        // DOWN history
        } else if (c == '+') {
            if (isDownLim == 0) {
                if (command_index > 0) {
                    strcpy(cli_buffer, match[(command_index-1) % match_index]);
                    index = strlen(cli_buffer);
                    command_index = 0;
                }
                deleteChar(cli_buffer);
                reset(cli_buffer);
                current_index = (current_index + 1) % MAX_HISTORY;
                strcpy(cli_buffer, history[current_index]);
                index = strlen(cli_buffer);
                uart_puts(cli_buffer);
                isUpLim = 0;
            }
            
            // Prevent looping 
            if ((current_index == initial && isRenew == 1) || (isRenew == 0 && current_index == history_index)) {
                isDownLim = 1;
            }
        
        // Handle backspace and delete key
        } else if (c == 8 || c == 127) {
            if (index > 0) {
                if (command_index > 0) {
                    strcpy(cli_buffer, match[(command_index-1) % match_index]);
                    index = strlen(cli_buffer);
                    command_index = 0;
                }
                index = index-1;
                cli_buffer[index] = '\0';
                deleteOneChar();
                
            }   
        // Receive characters from keyboard          
        } else if (c != '\n') {
            if (command_index > 0) {
                strcpy(cli_buffer, match[(command_index-1) % match_index]);
                index = strlen(cli_buffer);
                command_index = 0;
            }
            uart_sendc(c);
            cli_buffer[index] = c;
            strcpy(history[history_index % MAX_HISTORY], cli_buffer);
            current_index = history_index;
            isUpLim = 0, isDownLim = 1;
            index++;
        // If enter is pressed, execute the command
        } else {
            if (command_index > 0) {
                strcpy(cli_buffer, match[(command_index-1) % match_index]);
                index = strlen(cli_buffer);
                command_index = 0;
            }
            
            cli_buffer[index] = '\0';
            uart_puts("\nGot command: ");
            uart_puts(cli_buffer);
            strcpy(history[history_index % MAX_HISTORY], cli_buffer);
            uart_puts("\n");
            history_index++;
            if (history_index >= MAX_HISTORY)
            {
                history_index = 0;
                isRenew = 1;
            }
            current_index = history_index;

            /* Compare with supported commands and execute */
            if (index > 0 && index < MAX_CMD_SIZE) {
                // "help" command
                if (strncasecmp(cli_buffer, commands[0], strlen(commands[0])) == 0) {
                    displayHelp(cli_buffer, commands);

                // "clear" command
                } else if (strcasecmp(cli_buffer, commands[1]) == 0) {
                    clear();

                // "set color" command
                } else if (strncasecmp(cli_buffer, commands[2], strlen(commands[2])) == 0) {
                    setColor(cli_buffer);
                
                // "showinfo" command
                } else if (strcasecmp(cli_buffer, commands[3]) == 0) {
                    showInfo();
                
                // "baudrate" command
                } else if (strncasecmp(cli_buffer, commands[4], strlen(commands[4])) == 0) {
                    update_baud_rate(cli_buffer, &baud_rate);
                    isUARTUpdate = 1;
                
                // "length" command
                } else if (strncasecmp(cli_buffer, commands[5], strlen(commands[5])) == 0) {
                    update_data_length(cli_buffer, &data_bit_length);
                    isUARTUpdate = 1;

                // "stop" command
                } else if (strncasecmp(cli_buffer, commands[6], strlen(commands[6])) == 0) {
                    update_stop_bit(cli_buffer, &stop_bit);
                    isUARTUpdate = 1;
                
                // "parity" command
                } else if (strncasecmp(cli_buffer, commands[7], strlen(commands[7])) == 0) {
                    update_parity_bit(cli_buffer, &parity_bit);
                    isUARTUpdate = 1;
                
                // "handshake" command
                } else if (strncasecmp(cli_buffer, commands[8], strlen(commands[8])) == 0) {
                    update_handshake_control(cli_buffer, &handshake);
                    isUARTUpdate = 1;
                
                // "check" command
                } else if (strcasecmp(cli_buffer, commands[9]) == 0) {
                    check();
                } else {
                    uart_puts("Command not found\n");
                }
            // Print error if the command's length exceeds 100
            } else if (index >= MAX_CMD_SIZE) {
                uart_puts("Command is too long\n");
            // Print error if no command is entered
            } else {
                uart_puts("No command entered\n");
            }

            // Return to command line
            index = 0;
            break; // Exit the loop after processing one command
        }
    }     
}



void main() {
    // Initialize UART with default configuration
    uart_init(baud_rate, data_bit_length, stop_bit, parity_bit, handshake);

    // Display welcome message
    displayWelcomeMessage();

    //Loop forever
    while (1) {

        // Command-line Interface
        cli();
    }
}