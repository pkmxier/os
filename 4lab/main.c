#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "help.h"
#include <errno.h>

typedef struct {
    int fd;
    int min;
    int max;
} TFILE;

TFILE * OpenFile(char *path, int min, int max) {
    TFILE *file = (TFILE *)malloc(sizeof(TFILE));
    file->fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    file->min = min;
    file->max = max;

    if (file->fd < 0) {
        free(file);
        file = NULL;
    }

    return file;
}

int IsDigitOrLetter(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}
void PrintStatus(TFILE *file) {
    int symbolsCount = 0;
    int linesCount = 0;
    char c;

    while (read(file->fd, &c, 1) == 1) {
        if (IsDigitOrLetter(c)) {
            ++symbolsCount;
        } else if (c == '\n') {
            ++linesCount;
        }
    }

    if (c != '\n') {
        ++linesCount;
    }

    if (linesCount == 0) {
        ++linesCount;
    }

    printf("Number of lines: %d, number of symbols: %d\n", linesCount, symbolsCount);
}

void FindPattern(TFILE *file, char *mode, char *pattern) {
    char c;
    int i = 0;
    int len = strlen(pattern);
    int size = len > sysconf(_SC_PAGE_SIZE) ? len : sysconf(_SC_PAGESIZE);

    char *tmp = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, SEEK_SET);
    tmp[size] = '\0';

    if (!strstr(tmp, pattern)) {
        printf("Can`t find such word\n");
    } else {
        printf("Yes\n");
    }
}

void PrintLine(TFILE *file, int number) {
    char c;

    while (number != 1 && read(file->fd, &c, 1) == 1) {
        if (c == '\n') {
            --number;
        }
    }

    if (c == '\n') {
        ++number;
    }

    if (number != 1) {
        printf("Wrong line\n");
        return;
    }

    while (read(file->fd, &c, 1) == 1 && c != '\n') {
        printf("%c", c);
    }
    printf("\n");
}

void WriteWithin(TFILE *file, int position, char *text) {
    --position;
    lseek(file->fd, position, SEEK_SET);
    int fd = open("./", __O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("Can`t create temporary file\n");
        return;
    }

    char c;
    while (read(file->fd, &c, 1) == 1) {
        if (write(fd, &c, 1) != 1) {
            printf("Can`t write in temporary file\n");
            return;
        }
    }

    if (ftruncate(file->fd, position) < 0) {
        printf("Can`t erase from that position\n");
        return;
    }

    lseek(file->fd, position, SEEK_SET);
    if (write(file->fd, text, strlen(text)) != strlen(text)) {
        printf("Can`t write in this file\n");
        lseek(file->fd, position, SEEK_SET);
    }

    lseek(fd, 0, SEEK_SET);
    while (read(fd, &c, 1) == 1) {
        if (write(file->fd, &c, 1) != 1) {
            printf("Can`t write in this file\n");
            return;
        }
    }
}

void EraseWithin(TFILE *file, int position, int count) {
    --position;
    lseek(file->fd, position + count, SEEK_SET);
    int fd = open("./", __O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("Can`t create temporary file\n");
        return;
    }

    char c;
    while (read(file->fd, &c, 1) == 1) {
        if (write(fd, &c, 1) != 1) {
            printf("Can`t write in temporary file\n");
            return;
        }
    }

    if (ftruncate(file->fd, position) < 0) {
        printf("Can`t erase from that position\n");
        return;
    }

    lseek(file->fd, position, SEEK_SET);
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &c, 1) == 1) {
        if (write(file->fd, &c, 1) != 1) {
            printf("Can`t write in this file\n");
            return;
        }
    }
}

void SetLimit(TFILE *file, int min, int max) {
    int i = 0;
    int c;
    while (read(file->fd, &c, 1) == 1) {
        ++i;

        if (i + 1 > max) {
            break;
        }
    }

    if (i + 1 > max) {
        if (ftruncate(file->fd, max) < 0) {
            printf("Can`t trunc file\n");
            return;
        }
    } else if (i < min) {
        while (i < min) {
            if (write(file->fd, "0", 1) != 1) {
                return;
            }
            ++i;
        }
    }
}

void PrintFile(TFILE *file) {
    char c;
    while (read(file->fd, &c, 1) == 1) {
        printf("%c", c);
    }
}

int main(int argc, char *argv[]) {
    TFILE *cur;
    int isInteractive = 0;
    char args[argc][256];
    for (int i = 0; i < argc; ++i) {
        strcpy(args[i], argv[i]);
    }

    if (argc == 1) {
        isInteractive = 1;
    }

    while (1) {
        if (argc == 2) {
            if (StrToInt(args[1]) == HELP) {
                PrintUsage();
            } else if (StrToInt(args[1]) == CLEAR) {
                system("clear");
            } else if (StrToInt(args[1]) != NEWLINE) {
                PrintWrong();
                if (!isInteractive) {
                    return 1;
                }
            }
        } else if (argc >= 3 && argc <= 6) {
            switch(StrToInt(args[1])) {
                case SWITCH:
                    if (argc == 4 && StrToInt(args[2]) == FILE) {
                        cur = OpenFile(args[3], DEFAULT_MIN, DEFAULT_MAX);
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        isInteractive = 1;
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                case STATUS:
                    if (argc == 4 && StrToInt(args[2]) == FILE) {
                        cur = OpenFile(args[3], DEFAULT_MIN, DEFAULT_MAX);
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        PrintStatus(cur);
                        return 0;
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                case GETLINE:
                    if (argc == 5 && IsDecimal(args[2]) && StrToInt(args[3]) == FILE) {
                        cur = OpenFile(args[4], DEFAULT_MIN, DEFAULT_MAX);
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        PrintLine(cur, atoi(args[2]));
                        return 0;
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                case FIND:
                    if (argc == 6 && StrToInt(args[2]) == WHERETOSEARCH && StrToInt(args[4]) == FILE) {
                        cur = OpenFile(args[5], DEFAULT_MIN, DEFAULT_MAX);
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        FindPattern(cur, args[2], args[3]);
                        return 0;
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                case WRITE:
                    if (argc == 6 && IsDecimal(args[2]) && StrToInt(args[4]) == FILE) {
                        cur = OpenFile(args[5], DEFAULT_MIN, DEFAULT_MAX);
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        WriteWithin(cur, atoi(args[2]), args[3]);
                        return 0;
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                case ERASE:
                    if (argc == 6 && IsDecimal(args[2]) && IsDecimal(args[3]) && StrToInt(args[4]) == FILE) {
                        cur = OpenFile(args[5], DEFAULT_MIN, DEFAULT_MAX);
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        EraseWithin(cur, atoi(args[2]), atoi(args[3]));
                        return 0;
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                case LIMIT:
                    if (argc == 6 && IsDecimal(args[2]) && IsDecimal(args[3]) && StrToInt(args[4]) == FILE) {
                        cur = OpenFile(args[5], atoi(args[2]), atoi(args[3]));
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        SetLimit(cur, atoi(args[2]), atoi(args[3]));
                        isInteractive = 1;
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                case FILE:
                    if (argc == 3) {
                        cur = OpenFile(args[2], DEFAULT_MIN, DEFAULT_MAX);
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        isInteractive = 1;
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                case PRINT:
                    if (argc == 4 && StrToInt(args[2]) == FILE) {
                        cur = OpenFile(args[3], DEFAULT_MIN, DEFAULT_MAX);
                        if (cur == NULL) {
                            printf("Can`t open file\n");
                            return 2;
                        }

                        PrintFile(cur);
                    } else {
                        PrintWrong();
                        if (!isInteractive) {
                            return 1;
                        }
                    }
                    break;
                default:
                    PrintWrong();
                    if (!isInteractive) {
                        return 1;
                    }
                    break;
            }
        } else if (argc != 1) {
            PrintWrong();
        }

        if (!isInteractive) {
            return 0;
        } else {
            char buffer[256];
            int c = getchar();
            if (c == EOF) {
                return 0;
            }
            int i = 0;
            int j = 1;

            while (c != '\n' && i != 256) {
                if (c == ' ') {
                    buffer[i] = '\0';
                    strcpy(args[j], buffer);
                    ++j;
                    i = 0;
                    c = getchar();
                    continue;
                }

                buffer[i++] = c;
                c = getchar();
            }

            if (c == '\n') {
                if (i == 0) {
                    buffer[0] = '\n';
                    ++i;
                }
                buffer[i] = '\0';
                strcpy(args[j], buffer);
            }


            argc = j + 1;
        }
    }
    return 0;
}
