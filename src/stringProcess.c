#include "../utils/stringProcess.h"

int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

// Function to compare two strings ignoring the case
int strcasecmp(const char *str1, const char *str2) {
    while (*str1 && *str2 && tolower(*str1) == tolower(*str2)) {
        str1++;
        str2++;
    }
    return tolower(*str1) - tolower(*str2);
}

// Function to compare two strings ignoring the case up to n characters
int strncasecmp(const char *str1, const char *str2, int n) {
    while (n > 0 && *str1 && *str2 && tolower(*str1) == tolower(*str2)) {
        str1++;
        str2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return tolower(*str1) - tolower(*str2);
}

// Function to get the length of a string
int strlen(const char *str) {
    int len = 0;
    while (*str) {
        len++;
        str++;
    }
    return len;
}

// Function to tokenize a string
char *strtok(char *str, const char *delim)
{
    static char *next_token = NULL;
    char *token_start;

    // If the input is NULL, continue tokenizing the previous string
    if (str == NULL)
    {
        str = next_token;
    }

    // If the string is NULL or an empty string, return NULL
    if (str == NULL || *str == '\0')
    {
        next_token = NULL;
        return NULL;
    }

    // Tokenize and skip any leading delimiters
    token_start = str + strspn(str, delim);
    if (*token_start == '\0')
    {
        next_token = NULL;
        return NULL;
    }

    // Find the end of the token
    next_token = token_start + strcspn(token_start, delim);
    if (*next_token == '\0')
    {
        next_token = NULL;
    }
    else
    {
        // Replace the following delimiter with a null terminator
        *next_token = '\0';
        next_token++;
    }

    return token_start;
}

// Function to get the length of the prefix of a string that contains only characters from a set
int strspn(const char *str, const char *set)
{
    int len = 0;
    while (*str && strchr(set, *str))
    {
        str++;
        len++;
    }
    return len;
}

// Function to get the length of the prefix of a string that contains only characters not from a set
int strcspn(const char *str, const char *set)
{
    int len = 0;
    while (*str && !strchr(set, *str))
    {
        str++;
        len++;
    }
    return len;
}

// Function to find the first occurrence of a character in a string
char *strchr(const char *str, int c)
{
    while (*str)
    {
        if (*str == (char)c)
        {
            return (char *)str;
        }
        str++;
    }
    if (*str == (char)c)
    {
        return (char *)str;
    }
    return NULL;
}


// Function to copy a string
char* strcpy(char *dest, const char *src) {
    char *start = dest;
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return start;
}

// Functino to concatenate two strings
char* strcat(char *dest, const char *src) {
    char *start = dest;
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return start;
}

// Function to reset a string
void reset(char *str) {
    for (int i = 0; i < MAX_CMD_SIZE; i++) {
        str[i] = '\0';
    }
}


// Fucntion to convert a string to an integer
int atoi(char *str) {
    int num = 0;
    while (*str) {
        // If not a number
        if (*str < '0' || *str > '9') {
            return -1;
        }
        num = num * 10 + (*str - '0');
        str++;
    }
    return num;
}
