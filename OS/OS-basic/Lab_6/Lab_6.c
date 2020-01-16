/******************************************************************************
     * File: Lab_6.c
     * Description: Лабораторная работа 6
     * Created: 18 jan 2020
     * Author: Melnikov Valery
     * Email: m-valeron-n@mail.ru
     * Task: Задание: Написать приложение, которое при помощи системных 
     *      вызовов выбранной операционой системы совершит рекурсивный обход 
     *      заданного каталога. Для каждого файла, найденного в каком-либо каталоге, 
     *      нужно собрать следующую информацию: имя файла, тип файла, владелец файла 
     *      и права доступа на файл. Вся собранная информация записывается в 
     *      какой-нибудь текстовый лог-файл, формат записи разработать самостоятельно. 
     *      Кроме того, при получении информации от файла, необходимо изменить 
     *      время создания файла на текущее.
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
//система не находит - <linux/dirent.h>
//нужны для - readdir
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <utime.h>
#include "Lab_6.h"

int main(int argc, char const *argv[])
{
    int fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC);

    // получает рабочую папку 
    // char h[100];
    // getcwd(h, 99);
    // printf("%s\n", h);

    char current_dir[256];
    if (argc > 1) {
        strcpy(current_dir, argv[1]);
    } else { 
        strcpy(current_dir, ".");
    }
    
    write_log(current_dir, fd, 0);
    close(fd);
    return 0;
}

DIR *get_directory(const char* current_dir) {
    DIR *dir = opendir(current_dir); 
    if (!dir) {
        perror("directory can't be open");
        exit(-1);
    }
    return dir;
}

void write_log(const char* dir_addr, int fd, int tab_count) {
    DIR *dir = get_directory(dir_addr);
    struct dirent *entry; 
    char *tabulate = get_tabulator(tab_count);
    struct stat info;

    while ( (entry = readdir(dir)) != NULL) {
        //address current file or directory
        char *wrk = malloc(sizeof(char) * 255);
        sprintf(wrk, "%s/%s", dir_addr, entry->d_name);       
        get_stat_info(wrk, &info);

        char *log = malloc(sizeof(char) * 255);

        if (S_ISDIR(info.st_mode)) {
            if (!strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".")) continue;
            sprintf(log, "%sDirectory: %s\n", tabulate, entry->d_name);
            make_record(fd, log);

            //РЕКУРСИЯ
            write_log(wrk, fd, tab_count + 1);
            
        } else {  
            //В МОЕЙ СИСТЕМЕ, СУДЯ ПО ВСЕМУ ДАННАЯ ФУНКЦИЯ ИЗМЕНЯЕТ ВЕМЯ ПОСЛЕДНЕЙ МОДИФИКАЦИИ ФАЙЛА
            utime(wrk, NULL);

            char mode[11];
            mode_to_letters(info.st_mode, mode);
            sprintf(log, "%s%s uid - %d, gid - %d, name -- %s\n",
             tabulate, mode, info.st_uid, info.st_gid, entry->d_name);
            make_record(fd, log);
        }  
        free(wrk); 
        free(log);     
    }
    free(tabulate);   
}

char *get_tabulator(int count) {
    char *tabs = malloc(sizeof(char) * count);
    for (int i = 0; i < count; i++) {
        tabs[i] = '\t';
    }
    return tabs;
}

void get_stat_info(const char *addr, struct stat *info) {
    int rc = stat(addr, info);
    if (rc != 0 ) {
        printf("Error with called address ('%s')", addr);
        exit(-1);
    }
}

void mode_to_letters(int mode,char *buf){
    strcpy(buf,"----------");
    if(mode & S_IRUSR)buf[1]='r';
    if(mode & S_IWUSR)buf[2]='w';
    if(mode & S_IXUSR)buf[3]='x';
    if(mode & S_IRGRP)buf[4]='r';
    if(mode & S_IWGRP)buf[5]='w';
    if(mode & S_IXGRP)buf[6]='x';
    if(mode & S_IROTH)buf[7]='r';
    if(mode & S_IWOTH)buf[8]='w';
    if(mode & S_IXOTH)buf[9]='x';
}

void make_record(int fd, char *log) {
    int res = write(fd, log, sizeof(char) * strlen(log));
    if (!res) {
        printf("fail record\n");
        exit(-1);
    }
}
