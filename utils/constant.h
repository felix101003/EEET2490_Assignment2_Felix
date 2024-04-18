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

#define MAX_CMD_SIZE 100
#define MAX_COLOR_SIZE 8
#define MAX_COMMAND_SIZE 4
#define INTIAL_NAME "FelixOS> "

#define BLACK_TEXT "\033[30m"
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define BLUE_TEXT "\033[34m"
#define MAGENTA_TEXT "\033[35m"
#define CYAN_TEXT "\033[36m"
#define WHITE_TEXT "\033[37m"
#define RESET_TEXT "\033[0m"

#define BLACK_BG "\033[40m"
#define RED_BG "\033[41m"
#define GREEN_BG "\033[42m"
#define YELLOW_BG "\033[43m"
#define BLUE_BG "\033[44m"
#define MAGENTA_BG "\033[45m"
#define CYAN_BG "\033[46m"
#define WHITE_BG "\033[47m"
#define RESET_BG "\033[0m"\

#define SET_COLOR "set color"
#define HELP "help"
#define CLEAR "clear"
#define SHOW_INFO "showinfo"


#define NULL ((void *)0)

#endif // CONSTANT_H
