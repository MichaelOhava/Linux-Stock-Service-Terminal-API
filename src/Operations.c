#include "Operations.h"

//This function navigate the son proccess to the relevant operation acording to the user input
void master_func(int choice,int fd)
{ //shell func that will be called by the 'WORKER' , and will choose relevant func
  switch (choice)
  {
      case 1:
      //printf("reached master function, amount of stocks: %d \n", choice);
        prog_one(fd);
        break;
      case 2:
        prog_two(fd);
        break;
      case 3:
        prog_three(fd);
        break;
      case 4:
        prog_four(fd,FALSE);
        break;
        case 5:
        prog_four(fd,FALSE);
        write(fd,"\nThank You And GoodBay!\n", strlen("\nThank You And GoodBay!\n"));
        break;
  }
  
}

//This function download the asked stock and order it in csv format 
void prog_one(int fd)
{//this is based on the assumption only one stock is given in arguments!
   //int saveOUT = dup(STDOUT_FILENO);
   write(STDOUT_FILENO,"enter stock name: ",sizeof("enter stock name: "));
   //printf("enter stock name: ");
   char stock_name[BUFFER_SIZE];
   scanf("%s", stock_name);
   write(STDOUT_FILENO , "fetching data .. please wait ...\n", sizeof(char[34]));
   char path[MAX_PATH];
   strcpy(path, "\0");
   strcat(path,"./get_stocks_data.sh ");
   strcat(path, stock_name);
   strcat(path," 2>/dev/null");
   system(path);

    //Script takes time to download the stocks therefore need to wait some time before letting this portion run
    //printf("PARENT: finished waiting for script! NEED THE SCRIPT TO FINISH BY NOW RETREIVING DATA \n");
    json_data_extractor(stock_name);
    write(fd, "fetched ", sizeof(char[9]));
    write(fd, stock_name, strlen(stock_name));
    write(fd, "\n", sizeof("\n"));
    //printf("%s : ", stock);
}

//This function displays available stocks
void prog_two(int fd)
{//this program will read the files from working dir to understand which stock data exists, and list them
   struct dirent *entry;
  DIR *dp;

//gettint current directory 
  dp = opendir(".");
  if (dp == NULL) 
    perror("opendir");
  
  while((entry = readdir(dp)))
  check_for_stocks(entry->d_name,fd);
  
  closedir(dp);
}

//This function show csv data of requested stock
void prog_three(int fd)
{//this program will print the data of a selected stock from available stock files in working dir
 //valid check: if file exists
 int x;
  struct dirent *entry;
  DIR *dp;
  char name[35]="",stock_ending[6]=".data";
  bool found=false;
  //gettint current directory 
  dp = opendir(".");
  if (dp == NULL) 
    perror("opendir");
    write(STDOUT_FILENO,"Type stock name: ",sizeof("Type stock name: "));
    scanf("%s",name);
    strcat(name,stock_ending);
    x=open(name,O_RDONLY);
   if(x!=-1)
    while((entry = readdir(dp)))
    {
        if(strcmp(name,entry->d_name)==0)
            {
                print_stock_data(name,fd);//printing the stock data 
                found=true;
                break;
            }
    }
  else //if(found==false)
  {
    char msg[50] = {0};
    strcat(msg , "Stock ");
    strcat(msg, name);
    strcat(msg, " data does not exist! \n");
    write(fd, msg , sizeof(msg));
  }
}

//This function saves stocks.data files to zip file
void prog_four(int fd,bool fromsignal)
{//this program will be called upon using a signal,which will cause the function to activate
 //after the function operated close the entire program if called by signal!
 //this program zips all of the stock data files in working dir
   struct dirent *entry;
  DIR *dp;
  int errorp;
  zip_error_t ziperror;
 struct zip_source * source ;
char name[35]="",buff[BUFFER_SIZE];
//gettint current directory 
  dp = opendir(".");
 if (dp == NULL) 
    perror("opendir");

//enter files to zip
    getcwd(buff,BUFFER_SIZE);//getting current directory path for the zip_source_file function
    struct  zip *zipper = zip_open("stocks_db.zip",ZIP_CREATE|ZIP_TRUNCATE,&errorp);//creat zip archiv
    if(zipper==NULL){//checks if the zip archive opend successfully
    {
      if(fromsignal==false){
      zip_error_init_with_code(&ziperror, errorp);
      write(fd, "Failed to open  zip archive 'stocks_db'", strlen("Failed to open  zip archive 'stocks_db'"));
      write(fd, zip_error_strerror(&ziperror), strlen(zip_error_strerror(&ziperror)));}
      else printf("Failed to open  zip archive 'stocks_db' :%s",zip_error_strerror(&ziperror));

    }
      
    }
    while((entry = readdir(dp)))//walking on all files in current directory
    {
      if (strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name ,"..")!=0) {//ignore ./.. files
        source=zip_source_file(zipper,entry->d_name, 0, 0);
          enter_files_to_zip(entry->d_name,zipper,source,errorp);
      }

    }
    if(fromsignal==false){
    write(fd
    , "Data saved in ", strlen("Data saved in "));
    write(fd,buff, strlen(buff));
    write(fd,"\n", 1);}
    else printf("Data saved in %s\n",buff);
  closedir(dp);
  if(zip_close(zipper)!=0)//checking if closing operation succeed
  {
    if(fromsignal==false)
     write(fd, "problem with closing the zip archive", strlen("problem with closing the zip archive"));
     else printf("problem with closing the zip archive");

  }
     


}

//This funcrion is the main executive of prog_one
void json_data_extractor(char* stock_name)
{
    char file_stock_name[STOCK_NAME_BUFF];
    strcpy(file_stock_name, stock_name);
    strcat(file_stock_name, ".data");
    int file = open(file_stock_name , O_WRONLY | O_CREAT , 0755); //opens the file for data to be written
    if (file == -1)
    {
      printf("error opening a file in json_data_extractor()");
      exit(12);
    }
    int saveOUT = dup(STDOUT_FILENO);
    dup2(file , STDOUT_FILENO); //causes printfs to print into the file.
   
    char stock_json[STOCK_NAME_BUFF], stock_eps[STOCK_NAME_BUFF];
    strcpy(stock_eps,stock_name); strcat(stock_eps, ".esp");
    strcpy(stock_json , stock_name); strcat(stock_json, ".stock"); 

    //the extractor:
    char *Curr_year = (char*)malloc(sizeof(char)*4);
    assert(Curr_year);
	  char Eps_curr_year[4]="";
	  struct json_object *eps_json,*stock_data_json,*monthely_obj,*yearly_eps_json ,*object_stock,*object_eps;
	  struct json_object *open,*high,*closej,*low,*volume,*time,*eps,*year_eps;
    struct json_object_iterator it_b , it_e;
    size_t n_eps,i=0;
    bool helper=false;
    //array 
    
    //stack data 
    stock_data_json = json_object_from_file(stock_json);
    json_object_object_get_ex(stock_data_json, "Monthly Adjusted Time Series", &monthely_obj);
    it_b =json_object_iter_begin(monthely_obj);
    it_e =json_object_iter_end(monthely_obj);
     memcpy(Curr_year,json_object_iter_peek_name(&it_b),4);
    Curr_year[4]='\0';
    //eps data 
    eps_json = json_object_from_file(stock_eps);
    json_object_object_get_ex(eps_json, "annualEarnings", &yearly_eps_json);
    n_eps=json_object_array_length(yearly_eps_json);
    object_eps = json_object_array_get_idx(yearly_eps_json, 0);
    json_object_object_get_ex(object_eps, "fiscalDateEnding", &year_eps);
    memcpy(Eps_curr_year,json_object_get_string(year_eps),4);
    
    //sscanf(json_object_get_string(year_eps),"%4s",Eps_curr_year);
    

    
    //loop
   while(!json_object_iter_equal(&it_b, &it_e)){
     memcpy(Curr_year,json_object_iter_peek_name(&it_b),4);
    printf("Time: %s, ", json_object_iter_peek_name(&it_b));
//extracting from stock data 
    object_stock = json_object_iter_peek_value(&it_b); 
    json_object_object_get_ex(object_eps, "reportedEPS", &eps);
	json_object_object_get_ex(object_stock, "1. open", &open);
    json_object_object_get_ex(object_stock, "2. high", &high);
    json_object_object_get_ex(object_stock, "3. low", &low);
    json_object_object_get_ex(object_stock, "4. close", &closej);
    json_object_object_get_ex(object_stock, "6. volume", &volume);

   //printing
	  printf("Open: %s, ", json_object_get_string(open));
    printf("High: %s, ", json_object_get_string(high));
    printf("Low: %s, ", json_object_get_string(low));
    printf("Close: %s, ", json_object_get_string(closej));
    printf("Volume: %s, ", json_object_get_string(volume));
    
     if(atoi(Curr_year)>atoi(Eps_curr_year))
        printf("Eps: 0 \n");
     else if(atoi(Curr_year)<=atoi(Eps_curr_year)){
        if(atoi(Curr_year)-atoi(Eps_curr_year)!=0 && i< n_eps){

          i++;
          object_eps = json_object_array_get_idx(yearly_eps_json, i);
          json_object_object_get_ex(object_eps, "fiscalDateEnding", &year_eps);
          memcpy(Eps_curr_year,json_object_get_string(year_eps),4);
          json_object_object_get_ex(object_eps, "reportedEPS", &eps); 
          }

         printf("Eps: %s\n",json_object_get_string(eps));
       }
      json_object_iter_next(&it_b);
    }
   
    free(Curr_year);
    dup2(saveOUT , STDOUT_FILENO);
    close(file);
}

//This function is the main executive of prog_two
void check_for_stocks(char* name,int fd)
{
char *token,f_type[6]="data",seps[]=".",*tokencpy;
token = strtok(name, seps);
if(token!=NULL){
tokencpy=(char*)malloc(sizeof(char)*(strlen(token))+ 1);
    assert(tokencpy);
    strcpy(tokencpy,token);
    token = strtok(NULL, seps);
    if(token!=NULL)
    if(strcmp(token,f_type)==0)
    {
           strcat(tokencpy, " ");
           write(fd, tokencpy, strlen(tokencpy)); //printf("%s\n",tokencpy);
    }
free(tokencpy);
}

}

//This function is the main executive of prog_three
void print_stock_data(char* name,int fd)
{
FILE* stock;
char buffer[BUFFER_SIZE];
stock=fopen(name, "r");
assert(stock);
while (fscanf(stock,"%[^\n] ", buffer) != EOF)
{
        strcat(buffer , "\n");
        write(fd, buffer, strlen(buffer));
}
fclose(stock);
}

bool creat_csv_file(char* name){

    char stock_ending[5]="data",*token;
    char* cpyname=(char*)malloc(sizeof(char)*(strlen(name)));
    assert(cpyname);
    strcpy(cpyname,name);
    token=strtok(cpyname,".");
    token=strtok(NULL,".");
    if(token!=NULL)
        if(strcmp(token,stock_ending)==0){
            print_stock_data_to_csv(name);//printing the stock data 
            free(cpyname);
            return true;
        }
    free(cpyname);
    return false;
}

void print_stock_data_to_csv(char* name)
{
char buffer[BUFFER_SIZE],*token,*csvname,stock_ending[5]=".csv";
char* cpyname=(char*)malloc(sizeof(char)*(strlen(name)));
assert(cpyname);
strcpy(cpyname,name);
token=strtok(cpyname,".");
csvname=(char*)malloc(sizeof(char)*(strlen(token)+strlen(stock_ending)));
assert(csvname);
strcpy(csvname,token);
strcat(csvname,stock_ending);
FILE* stock,*newfile;
stock=fopen(name, "r");
assert(stock);
newfile=fopen(csvname,"w");
assert(newfile);
while (fscanf(stock,"%[^\n] ", buffer) != EOF){
        fprintf(newfile,"%s",buffer);
        fprintf(newfile, "\n");
}
fclose(stock);
fclose(newfile);
free(csvname);
free(cpyname);
}

//This function add files that match to postfix ".data" to archive 
void enter_files_to_zip(char* name,zip_t *zipper,struct zip_source * source,int errorp)
{
char *token,stock_ending[6]="data";
char* cpyname=(char*)malloc(sizeof(char)*(strlen(name)));
assert(cpyname);
strcpy(cpyname,name);
token=strtok(cpyname,".");
token=strtok(NULL,".");
if(token!=NULL){
        if(strcmp(token,stock_ending)==0){
          if(source == NULL || zip_add(zipper, name, source) == -1)//adding the file to the zip archive
            {
               printf("Failed to add file to zip archive");
               zip_source_free(source);
            }
      }
       
  }   
free(cpyname);
}


