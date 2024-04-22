#ifndef CONSTANT_H
#define CONSTANT_H

#define WELCOME_MESSAGE \
"######## ######## ######## ########  #######  ##         #######    #####   \n"\
"##       ##       ##          ##    ##     ## ##    ##  ##     ##  ##   ##  \n"\
"##       ##       ##          ##           ## ##    ##  ##     ## ##     ## \n"\
"######   ######   ######      ##     #######  ##    ##   ######## ##     ## \n"\
"##       ##       ##          ##    ##        #########        ## ##     ## \n"\
"##       ##       ##          ##    ##              ##  ##     ##  ##   ##  \n"\
"######## ######## ########    ##    #########       ##   #######    #####   \n"\
"\n"\
"\n"\
"\n"\
"\n"\
"######## ######## ##       #### ##     ##     #######   ######              \n"\
"##       ##       ##        ##   ##   ##     ##     ## ##    ##             \n"\
"##       ##       ##        ##    ## ##      ##     ## ##                   \n"\
"######   ######   ##        ##     ###       ##     ##  ######              \n"\
"##       ##       ##        ##    ## ##      ##     ##       ##             \n"\
"##       ##       ##        ##   ##   ##     ##     ## ##    ##             \n"\
"##       ######## ######## #### ##     ##     #######   ######              \n"\
"\n"\
"Developed by Nguyen Dinh Viet- s3927291\n"

// Maximum length
#define MAX_CMD_SIZE 100
#define MAX_COLOR_SIZE 8
#define MAX_COMMAND_SIZE 10
#define MAX_HISTORY 1000

#define INTIAL_NAME "FelixOS> "

// Text color
#define BLACK_TEXT "\033[30m"
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define BLUE_TEXT "\033[34m"
#define MAGENTA_TEXT "\033[35m"
#define CYAN_TEXT "\033[36m"
#define WHITE_TEXT "\033[37m"
#define RESET_TEXT "\033[0m"

// Background color
#define BLACK_BG "\033[40m"
#define RED_BG "\033[41m"
#define GREEN_BG "\033[42m"
#define YELLOW_BG "\033[43m"
#define BLUE_BG "\033[44m"
#define MAGENTA_BG "\033[45m"
#define CYAN_BG "\033[46m"
#define WHITE_BG "\033[47m"
#define RESET_BG "\033[0m"\

// Command list
#define SET_COLOR "set color"
#define HELP "help"
#define CLEAR "clear"
#define SHOW_INFO "showinfo"

// Command list to configure UART and check configuration
#define BAUD_RATE "baudrate"
#define STOP "stop"
#define PARITY "parity"
#define LENGTH "length"
#define HANDSHAKE "handshake"
#define CHECK "check"


// Model list
#define RPI0_BCM2835_512M_SONYUK 0x00900092
#define RPI1B_PLUS_BCM2835 0x00000010
#define RPI2B_BCM2836_1G_SONYUK 0x00a01041
#define RPI3B_BCM2837_1G_SONYUK 0x00a02082
#define RPI3B_PLUS_BCM2837_1G_SONYUK 0x00a020d3
#define RPI4B_BCM2711_2G_SONYUK 0x00b03111

// Board configuration
#define UART_CLOCK 48000000

#define NULL ((void *)0)

#endif // CONSTANT_H
