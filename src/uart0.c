#include "../header/uart0.h"

/**
 * Set baud rate and characteristics and map to GPIO
 */
void uart_init()
{
	unsigned int r;

	/* Turn off UART0 */
	UART0_CR = 0x0;
	/* Setup GPIO pins 36 and 37 */

	/* Set GPIO36 and GPIO37 to be pl011 TX/RX which is ALT2	*/
	r = GPFSEL3;
	r &= ~((7 << 18) | (7 << 21));		/// clear bits 23-18 (FSEL36, FSEL37)
	r |= (0b110 << 18) | (0b110 << 21); // Set value 0b110 (select ALT2: TXD0/RXD0)
	GPFSEL3 = r;

	/* enable GPIO 36, 37 */
#ifdef RPI3	   // RPI3
	GPPUD = 0; // No pull up/down control
	// Toogle clock to flush GPIO setup
	r = 150;
	while (r--)
	{
		asm volatile("nop");
	}								 // waiting 150 cycles
	GPPUDCLK1 = (1 << 4) | (1 << 5); // enable clock for GPIO 36, 37
	r = 150;
	while (r--)
	{
		asm volatile("nop");
	}			   // waiting 150 cycles
	GPPUDCLK1 = 0; // flush GPIO setup

#else // RPI4
	r = GPIO_PUP_PDN_CNTRL_REG2;
	r &= ~((3 << 8) | (3 << 10)); // No resistor is selected for GPIO 36, 37
	GPIO_PUP_PDN_CNTRL_REG2 = r;
#endif

	/* Mask all interrupts. */
	UART0_IMSC = 0;

	/* Clear pending interrupts. */
	UART0_ICR = 0x7FF;

	/* Set integer & fractional part of Baud rate
	Divider = UART_CLOCK/(16 * Baud)
	Default UART_CLOCK = 48MHz (old firmware it was 3MHz);
	Integer part register UART0_IBRD  = integer part of Divider
	Fraction part register UART0_FBRD = (Fractional part * 64) + 0.5 */

	// Baud rate
	configure_baud_rate(115200);

	/* Set up the Line Control Register */
	/* Enable FIFO */
	/* Set length to 8 bit */
	/* Defaults for other bit are No parity, 1 stop bit */
	UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_8BIT;

	/* Enable UART0, receive, and transmit */
	UART0_CR = 0x301; // enable Tx, Rx, FIFO
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
    uart_sendc(hexChars[(byte >> 4) & 0x0F]);
    uart_sendc(hexChars[byte & 0x0F]);
}


// Function to configure the baud rate of the UART
void configure_baud_rate(int baud_rate) {
	unsigned int integerPart = UART_CLOCK / (16 * baud_rate);
    float actual = ((float) UART_CLOCK)/ (16 * baud_rate);

	float fractionalPart = (actual - integerPart);

	// Baud rate
	UART0_IBRD = integerPart;
	UART0_FBRD = (int)(fractionalPart * 64 + 0.5);
}

void check_baud_rate() {
	uart_puts("Baud rate: ");
	uart_dec(UART0_IBRD);
	uart_puts(" ");
	uart_dec(UART0_FBRD);
	uart_puts("\n");
}