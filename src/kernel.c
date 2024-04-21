// -----------------------------------kernel.c -------------------------------------
#include "../utils/normal.h"
#include "../utils/stringProcess.h"
#include "../utils/color.h"

char* commands[] = {
    HELP,
    CLEAR,
    SET_COLOR,
    SHOW_INFO
};

int current_index = 0;
int history_index = 0;
int isRenew = 0;

void cli()
{
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
            uart_puts("\n");
            history_index++;
            if (history_index >= MAX_HISTORY)
            {
                history_index = 0;
                isRenew = 1;
            }
            current_index = history_index;

            /* Compare with supported commands and execute */
            if (index > 0) {
                if (strncasecmp(cli_buffer, commands[0], strlen(commands[0])) == 0) {
                    displayHelp(cli_buffer, commands);
                } else if (strcasecmp(cli_buffer, commands[1]) == 0) {
                    clear();
                } else if (strncasecmp(cli_buffer, commands[2], strlen(commands[2])) == 0) {
                    setColor(cli_buffer);
                } else if (strcasecmp(cli_buffer, commands[3]) == 0) {
                    showInfo();
                } else {
                    // uart_puts("Command not found\n");
                    check_baud_rate();
                }
            }

            

            // Return to command line
            index = 0;
            break; // Exit the loop after processing one command
        }
    }     
}



void main() {
    // Set up the UART
    uart_init();

    // Display welcome message
    displayWelcomeMessage();

    //Loop forever
    while (1) {

        // Command-line Interface
        cli();
    }
}