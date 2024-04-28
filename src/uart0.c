// -----------------------------------uart0.c -------------------------------------
#include "../header/uart0.h"

/**
 * Set baud rate and characteristics and map to GPIO
 */
void uart_init(int baud_rate, int data_bit_length, int stop_bit, int parity_bit, int handshake)
{
	unsigned int r;

	/* Turn off UART0 */
	UART0_CR = 0x0;
	
	
	/* Set GPIO14 and GPIO15 to be pl011 TX/RX which is ALT0	*/
	r = GPFSEL1;

	/* Setup GPIO pins 14, 15, 16 and 17 */
	if (handshake == 0) {
		r &=  ~((7 << 12) | (7 << 15) | (7 << 18) | (7 << 21)); //clear bits 23-12 (FSEL15, FSEL14, FSEL16, FSEL17)
		r |= (0b100 << 12)|(0b100 << 15);   //Set value 0b100 (select ALT0: TXD0/RXD0)
	} else {
		r &=  ~((7 << 12) | (7 << 15) | (7 << 18) | (7 << 21)); //clear bits 23-12 (FSEL15, FSEL14, FSEL16, FSEL17)
		r |= (0b100 << 12)|(0b100 << 15) | (0b111 << 18) | (0b111 << 21);   //Set value 0b100 (select ALT0: TXD0/RXD0), value 0b111 (select ALT3: CTS0/RTS0)
	}
	GPFSEL1 = r;

	/* enable GPIO 14, 15, 16 and 17 */
#ifdef RPI3	   // RPI3
	GPPUD = 0; // No pull up/down control
	// Toogle clock to flush GPIO setup
	r = 150;
	while (r--)
	{
		asm volatile("nop");
	}								 // waiting 150 cycles
	GPPUDCLK0 = (1 << 14)|(1 << 15)| (1<<16) | (1<<17); //enable clock for GPIO 14, 15
	r = 150;
	while (r--)
	{
		asm volatile("nop");
	}			   // waiting 150 cycles
	GPPUDCLK0 = 0;        // flush GPIO setup

#else // RPI4
	r = GPIO_PUP_PDN_CNTRL_REG0;
	r &= ~((3 << 28) | (3 << 30)); //No resistor is selected for GPIO 14, 15
	GPIO_PUP_PDN_CNTRL_REG0 = r;

	r = GPIO_PUP_PDN_CNTRL_REG1;
	r &= ~((3 << 0) | (3 << 2)); //No resistor is selected for GPIO 16, 17
	GPIO_PUP_PDN_CNTRL_REG1 = r;
#endif

	/* Mask all interrupts. */
	UART0_IMSC = 0;

	/* Clear pending interrupts. */
	UART0_ICR = 0x7FF;

	// Baud rate
	configure_baud_rate(baud_rate);

	/* Set up the Line Control Register */
	configure_bit(data_bit_length, stop_bit, parity_bit);

	/* Enable UART0, receive, and transmit */
	configure_handshaking_control(handshake);
}

/**
 * Send a character
 */
void uart_sendc(char c)
{

	/* Check Flags Register */
	/* And wait until transmitter is not full */
	do
	{
		asm volatile("nop");
	} while (UART0_FR & UART0_FR_TXFF);

	/* Write our data byte out to the data register */
	UART0_DR = c;
}

/**
 * Receive a character
 */
char uart_getc()
{
	char c = 0;

	/* Check Flags Register */
	/* Wait until Receiver is not empty
	 * (at least one byte data in receive fifo)*/
	do
	{
		asm volatile("nop");
	} while (UART0_FR & UART0_FR_RXFE);

	/* read it and return */
	c = (unsigned char)(UART0_DR);

	/* convert carriage return to newline */
	return (c == '\r' ? '\n' : c);
}

/**
 * Display a string
 */
void uart_puts(char *s)
{
	while (*s)
	{
		/* convert newline to carriage return + newline */
		if (*s == '\n')
			uart_sendc('\r');
		uart_sendc(*s++);
	}
}

/**
 * Display a value in hexadecimal format
 */
void uart_hex(unsigned int num)
{
	uart_puts("0x");
	for (int pos = 28; pos >= 0; pos = pos - 4)
	{
		// Get highest 4-bit nibble
		char digit = (num >> pos) & 0xF;
		/* Convert to ASCII code */
		// 0-9 => '0'-'9', 10-15 => 'A'-'F'
		digit += (digit > 9) ? (-10 + 'A') : '0';
		uart_sendc(digit);
	}
}
/**
 * Display a value in decimal format
 */
void uart_dec(int num)
{
	// A string to store the digit characters
	char str[33] = "";
	// Calculate the number of digits
	int len = 1;
	int temp = num;
	while (temp >= 10)
	{
		len++;
		temp = temp / 10;
	}
	// Store into the string and print out
	for (int i = 0; i < len; i++)
	{
		int digit = num % 10; // get last digit
		num = num / 10;		  // remove last digit from the number
		str[len - (i + 1)] = digit + '0';
	}
	str[len] = '\0';
	uart_puts(str);
}

// Display a byte in hexadecimal format
void uart_hex_byte(unsigned char byte) {
	char hexChars[] = "0123456789ABCDEF";
    uart_sendc(hexChars[(byte >> 4) & 0xF]);
    uart_sendc(hexChars[byte & 0xF]);
}

// Function to configure the baud rate of the UART
void configure_baud_rate(int baud_rate) {
    unsigned int integerPart = UART_CLOCK / (16 * (baud_rate));
    float actual = ((float) UART_CLOCK) / (16 * (baud_rate));
    float fractionalPart = (actual - integerPart);

    // Baud rate
    UART0_IBRD = integerPart;
    UART0_FBRD = (int)(fractionalPart * 64 + 0.5);
}

// Function to configure data bit length, stop bit, and parity bit of the UART
void configure_bit(int data_bit_length, int stop_bit, int parity_bit) {
    // Set up the Line Control Register
    // Enable FIFO and set up bit length, stop bit, and parity bit
    UART0_LCRH = UART0_LCRH_FEN | change_bit_length(data_bit_length) | change_stop_bit(stop_bit) | change_parity_bit(parity_bit);
}

// Function to configure handshake control of the UART
void configure_handshaking_control(int status) {
    if (status == 1) {
        UART0_CR = 0xC301;
    } else {
        UART0_CR = 0x301;
    }
}

// Function to change data bit length
int change_bit_length(int data_bit_length) {
    switch (data_bit_length) {
        case 5:
            return UART0_LCRH_WLEN_5BIT;
        case 6:
            return UART0_LCRH_WLEN_6BIT;
        case 7:
            return UART0_LCRH_WLEN_7BIT;
        case 8:
            return UART0_LCRH_WLEN_8BIT;
        default:
            return UART0_LCRH_WLEN_8BIT;
    }
}

// Function to change stop bit
int change_stop_bit(int stop_bit) {
    switch (stop_bit) {
        case 1:
            return 0;
        case 2:
            return UART0_LCRH_STP2;
        default:
            return 0;
    }
}

// Function to change parity bit
int change_parity_bit(int parity_bit) {
    switch (parity_bit) {
		case 0:
			return 0;
		case 1:
			return UART0_LCRH_PEN;
		case 2:
			return UART0_LCRH_PEN | UART0_LCRH_EPS;
		default:
			return 0;
	}
}

// Function to check UART configuration
void check() {
	// Check baud rate
	uart_puts("UART0_IBRD: ");
	int number1 = UART0_IBRD;
	uart_dec(number1);
	uart_puts("\nUART0_FBRD: ");
	int number2 = UART0_FBRD;
	uart_dec(number2);
	uart_puts("\nBaud rate (approx): ");
	int result = UART_CLOCK / (16 * (number1 + (float) number2 / 64));
	uart_dec(result);
	uart_puts("\n");

	// Check number of data bit
	uart_puts("Nunmber of data bit: ");
	switch (UART0_LCRH & (3 << 5)) {
		case UART0_LCRH_WLEN_5BIT:
			uart_puts("5\n");
			break;
		case UART0_LCRH_WLEN_6BIT:
			uart_puts("6\n");
			break;
		case UART0_LCRH_WLEN_7BIT:
			uart_puts("7\n");
			break;
		case UART0_LCRH_WLEN_8BIT:
			uart_puts("8\n");
			break;
		default:
			uart_puts("8\n");
			break;
	}

	// Check number stop bit
	uart_puts("Number of stop bit: ");
	if (UART0_LCRH & UART0_LCRH_STP2) {
		uart_puts("2\n");
	} else {
		uart_puts("1\n");
	}

	uart_puts("Parity bit: ");
	if (UART0_LCRH & UART0_LCRH_PEN) {
		if (UART0_LCRH & UART0_LCRH_EPS) {
			uart_puts("Even\n");
		} else {
			uart_puts("Odd\n");
		}
	} else {
		uart_puts("None\n");
	}

	uart_puts("Handshake control: ");

	if (UART0_CR == 0xC301 && GPFSEL1 & (0b111 << 18) && GPFSEL1 & (0b111 << 21)) {
		uart_puts("CTS/RTS handshake is enabled\n");
	} else {
		uart_puts("CTS/RTS handshake is disabled\n");
	}
}
