#include <stdio.h>
#include <dirent.h>

///Wrapper "opendir" for error controls
DIR *get_directory(const char* __name);

/**
 * Main function
 * @param fd - file descriptor 
 * @param tab_count - count tabulate for good readable in text file
**/
void write_log(const char *__dir_addr, int fd, int tab_count);

///Wrapper "write" for error controls
void make_record(int fd, char *log);

/**
 * @return - string containing tabs with count "count"
**/
char *get_tabulator(int count);

///Wrapper "stat" for error controls
void get_stat_info(const char *addr, struct stat *stat);

//Кусок кода взял отсюда http://www.cyberforum.ru/c-linux/thread283711.html
void mode_to_letters(int mode,char *str);

