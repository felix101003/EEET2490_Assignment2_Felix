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

void cli()
{
	static char cli_buffer[MAX_CMD_SIZE];
	static int index = 0;

	//read and send back each char
	char c = uart_getc();
	uart_sendc(c);

	//put into a buffer until got new line character
	if (c != '\n'){
		cli_buffer[index] = c; //Store into the buffer
		index++;
	} else if (c == '\n'){
		cli_buffer[index] = '\0';

		uart_puts("\nGot commands: ");
		uart_puts(cli_buffer); uart_puts("\n");

		/* Compare with supported commands and execute
		* ........................................... */
        if (index > 0) {
            if (strncasecmp(cli_buffer, commands[0], strlen(commands[0])-1) == 0) {
                displayHelp(cli_buffer, commands);
            } else if (strncasecmp(cli_buffer, commands[1], strlen(commands[1])-1) == 0) {
                clear();
            } else if (strncasecmp(cli_buffer, commands[2], strlen(commands[2])-1) == 0) {
                setColor(cli_buffer);
            } else if (strncasecmp(cli_buffer, commands[3], strlen(commands[3])-1) == 0) {
                showInfo();
            } else {
                uart_puts("Command not found\n");
            }
        }


		//Return to command line
		index = 0;
	}
}


void main() {
    // Set up the UART
    uart_init();

    // Display welcome message
    displayWelcomeMessage();

    // Display OS name
    displayOS();

    //Loop forever
    while (1) {
        // Command-line Interface
        cli();
    }
}