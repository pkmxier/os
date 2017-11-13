#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELP 1
#define EXIT 2
#define SWITCH 3
#define STATUS 4
#define GETLINE 5
#define FIND 6
#define WHERETOSEARCH 7
#define BEGIN 700
#define END 800
#define ETC 900
#define WRITE 10
#define ERASE 11
#define LIMIT 12
#define FILE 13
#define NOTCOMMAND -1

int IsDecimal(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
        ++i;
    }
    return 1;
}

int StrToInt(char *string) {
    if (strcmp(string, "/help") == 0) {
        return HELP;
    } else if (strcmp(string, "/exit") == 0) {
        return EXIT;
    } else if (strcmp(string, "/switch") == 0) {
        return SWITCH;
    } else if (strcmp(string, "/status") == 0) {
        return STATUS;
    } else if (strcmp(string, "/getline") == 0) {
        return GETLINE;
    } else if (strcmp(string, "/find") == 0) {
        return FIND;
    } else if (strcmp(string, "/begin") == 0) {
        return WHERETOSEARCH;
    } else if (strcmp(string, "/end") == 0) {
        return WHERETOSEARCH;
    } else if (strcmp(string, "/etc") == 0) {
        return WHERETOSEARCH;
    } else if (strcmp(string, "/write") == 0) {
        return WRITE;
    } else if (strcmp(string, "/erase") == 0) {
        return ERASE;
    } else if (strcmp(string, "/limit") == 0) {
        return LIMIT;
    } else if (strcmp(string, "/file") == 0) {
        return FILE;
    } else {
        return NOTCOMMAND;
    }
}

void PrintUsage() {
    printf("Usage: ./a.out /\"command name\" /file \"path\"\n"
           "commands:\n"
           "    help - print usage\n"
           "    exit - exit from program\n"
           "    switch /file:\"filename\" - change current file to the given\n"
           "    status - print lines' and symbols' count\n"
           "    getline \"number of line\" - print line\n"
           "    find begin/end/etc \"pattern\" - find word, which the pattern begins/ends/contains\n"
           "    write \"position\" \"text\" - write text at the position\n"
           "    erase \"position\" \"number of symbols\" - delete some symbols\n"
           "    limit \"min number of symbols\" \"max number of symbols\" - set up min and max size of file\n");
}

void PrintWrong() {
    printf("Wrong command\n"
           "type \"help\" for usage\n");
}

const int DEFAULT_MIN = 0;
const int DEFAULT_MAX = __INT_MAX__;

typedef struct {
    int fd;
    int min;
    int max;
} TFILE;

TFILE * OpenFile(char *path, int min, int max) {
    TFILE *file = (TFILE *)malloc(sizeof(TFILE));
//    open(&file->fd, ...);
//    if (file->fd < 0) {

//    }
    file->min = min;
    file->max = max;
    return file;
}

int main(int argc, char *argv[]) {
    TFILE *cur;

    if (argc == 1) {
        printf("Interactive mode:\n");
    } else if (argc == 2) {
        if (StrToInt(argv[1]) == HELP) {
            PrintUsage();
        } else {
            printf("Wrong command\n"
                   "type \"help\" for usage\n");
        }
    } else if (argc >= 3 && argc <= 6) {
        switch(StrToInt(argv[1])) {
            case SWITCH:
                if (argc == 4 && StrToInt(argv[2]) == FILE) {
                    cur = OpenFile(argv[3], DEFAULT_MIN, DEFAULT_MAX);
                    if (cur == NULL) {
                        printf("Can`t open file\n");
                        return 2;
                    }
                } else {
                    PrintWrong();
                    return 1;
                }
                break;
            case STATUS:
                if (argc == 4 && StrToInt(argv[2]) == FILE) {
                    cur = OpenFile(argv[3], DEFAULT_MIN, DEFAULT_MAX);
                    if (cur == NULL) {
                        printf("Can`t open file\n");
                        return 2;
                    }
                } else {
                    PrintWrong();
                    return 1;
                }
                break;
            case GETLINE:
                if (argc == 5 && IsDecimal(argv[2]) && StrToInt(argv[3]) == FILE) {
                    cur = OpenFile(argv[4], DEFAULT_MIN, DEFAULT_MAX);
                    if (cur == NULL) {
                        printf("Can`t open file\n");
                        return 2;
                    }
                } else {
                    PrintWrong();
                    return 1;
                }
                break;
            case FIND:
                if (argc == 6 && StrToInt(argv[2]) == WHERETOSEARCH && StrToInt(argv[4]) == FILE) {
                    cur = OpenFile(argv[5], DEFAULT_MIN, DEFAULT_MAX);
                    if (cur == NULL) {
                        printf("Can`t open file\n");
                        return 2;
                    }
                } else {
                    PrintWrong();
                    return 1;
                }
                break;
            case WRITE:
                if (argc == 6 && IsDecimal(argv[2]) && StrToInt(argv[4]) == FILE) {
                    cur = OpenFile(argv[5], DEFAULT_MIN, DEFAULT_MAX);
                    if (cur == NULL) {
                        printf("Can`t open file\n");
                        return 2;
                    }
                } else {
                    PrintWrong();
                    return 1;
                }
                break;
            case ERASE:
                if (argc == 6 && IsDecimal(argv[2]) && IsDecimal(argv[3]) && StrToInt(argv[4]) == FILE) {
                    cur = OpenFile(argv[5], DEFAULT_MIN, DEFAULT_MAX);
                    if (cur == NULL) {
                        printf("Can`t open file\n");
                        return 2;
                    }
                } else {
                    PrintWrong();
                    return 1;
                }
                break;
            case LIMIT:
                if (argc == 6 && IsDecimal(argv[2]) && IsDecimal(argv[3]) && StrToInt(argv[4]) == FILE) {
                    cur = OpenFile(argv[5], atoi(argv[2]), atoi(argv[3]));
                    if (cur == NULL) {
                        printf("Can`t open file\n");
                        return 2;
                    }
                } else {
                    PrintWrong();
                    return 1;
                }
                break;
            case FILE:
                if (argc == 3) {
                    cur = OpenFile(argv[2], DEFAULT_MIN, DEFAULT_MAX);
                    if (cur == NULL) {
                        printf("Can`t open file\n");
                        return 2;
                    }
                } else {
                    PrintWrong();
                    return 1;
                }
                break;
            default:
                PrintWrong();
                return 1;
                break;
        }
    }
    return 0;
}
