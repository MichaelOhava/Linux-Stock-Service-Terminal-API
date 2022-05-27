#include "HelpFunctions.h"

void master_func(int choice,int fd);
void prog_one(int fd);
void prog_two(int fd);
void prog_three(int fd);
void prog_four(int fd,bool fromsignal);
void json_data_extractor(char* stock_name);
void check_for_stocks(char* name,int fd);
void print_stock_data(char* name,int fd);
bool creat_csv_file(char* name);
void print_stock_data_to_csv(char* name);
void enter_files_to_zip(char* name,zip_t *zipper,struct zip_source * source,int errorp);
