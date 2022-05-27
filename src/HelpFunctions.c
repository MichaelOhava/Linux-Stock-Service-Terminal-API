#include"HelpFunctions.h"


//This function catch signals from son and father in order to synchronize between the proccesses
void FinishedSignal(int dummy)
{
    sleep(1);
}

//This function checks if user input is valid
bool Check_Validation(char* input){
  if(strlen(input)>1||input[0]=='\n')
    { 
      printf("too long input, please choose from 1 to 4 \n");
      return false;
    }
    
  else if(input[0]<'0'||input[0]>'9'){
      printf("Please enter number!!  \n");
      return false;
  }
  else if(input[0]>'5'){
    printf("Please enter number between  1-4! \n");
      return false;
  }
  
  return true;
}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap( (str+start), (str+end) , sizeof(char));
        start++;
        end--;
    }
}
 
// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

//Implementation of swap
void  swap(void  *v1, void  *v2, size_t  size) 
    {
    void *temp = malloc(size);
    assert(temp != NULL);
    memcpy(temp, v1, size);
    memcpy(v1, v2, size);
    memcpy(v2, temp, size);
    free(temp);
}

//Main menu input validation function
void check_choice_valid(int * choice)
{
    while(*choice < 1 || *choice > 5)
    {
        getchar();
        printf("Invalid input, enter a number between 1 and 4.");
        scanf("%d", choice);
        getchar();
    }
}

//This function responsible for check if there any zip data available for the stock database in directory
int CheckAndUnzip(){

    FILE*fp,*created;
    const char *archive;
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[100],path[BUFFER_SIZE];
    int err;
    int i, len;
    long long sum;
    getcwd(path,BUFFER_SIZE);//getting current directory path for the zip_source_file function
 
    archive = "stocks_db.zip";
    if ((za = zip_open(archive, 0, &err)) == NULL) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        fprintf(stderr, "%s: can't open zip archive `%s': %s\n", path ,archive, buf);
        return OPENFAILURE;
    }
 
    for (i = 0; i < zip_get_num_entries(za, 0); i++) 
    {
        if (zip_stat_index(za, i, 0, &sb) == 0) 
        {
          
                 zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    
                    fprintf(stderr, "zf faliure");
                    exit(100);
                }
                
               fp = fopen(archive,"r");
               if(fp==NULL)
                     exit(OPENFAILURE);
                created=fopen(sb.name,"w+");
                 if(created==NULL)
                     exit(OPENFAILURE);
                sum = 0;
                while (sum != sb.size) {
                    len = zip_fread(zf, buf, 100);
                    if (len < 0) {
                        fprintf(stderr, "error in zipread/n");
                        exit(102);
                    }
                    fprintf(created,"%s",buf);
                    sum += len;
                }
                zip_fclose(zf);
                if(fclose(fp)==-1)
                return CLOSEFAILURE;
                fclose(created);
        }
        
    }  
 
    if (zip_close(za) == -1) {
        fprintf(stderr, "%s: can't close zip archive `%s'/n", path, archive);
        return CLOSEFAILURE;
    }
 
    return DONE;
    
}

//This function prints the user menu 
void user_menu()
{
    printf("1 - Fetch stock data \n");
    printf("2 - List fetched stocks \n");
    printf("3 - Print stock data \n");
    printf("4 - Save all stock data \n");
    printf("5 - Exit \n");
    printf("your choice: ");

}

