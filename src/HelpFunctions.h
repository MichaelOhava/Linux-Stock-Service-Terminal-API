#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <json-c/json.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <zip.h>

#define BUFFER_SIZE 500
#define MAX_PATH 150
#define MAX_STOCKS 300
#define STOCK_NAME_BUFF 30
#define OPENFAILURE 1
#define CLOSEFAILURE 3
#define DONE 0
#define READ_END   0
#define WRITE_END  1
int CheckAndUnzip();
void reverse(char str[], int length);
char* itoa(int num, char* str, int base);
void  swap(void  *v1, void  *v2, size_t  size);
void check_choice_valid(int * choice);
void FinishedSignal(int dummy);
bool Check_Validation(char* input);
void user_menu();
