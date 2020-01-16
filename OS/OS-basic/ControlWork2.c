/******************************************************************************
     * File: ControlWork2
     * Description: Контрольная работа 2
     * Created: 16 jan 2020
     * Author: Melnikov Valery
     * Email: m-valeron-n@mail.ru
     * Task: Задание в текстовом файле.
     * Note: Вначале вся работа делалась "на бумаге", до тех пор пока не
     *      не выяснилось, что объем ручной работы будет слишком велик.
     *      Будут использоваться данные предапрительно полученные предыдущей
     *      ручной работой. 
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MEMORY_LIMIT 10

typedef enum _Page_Status {
    Uninvolved = 0,
    Physical = 1,
    Hard = 2
} Page_Status;

typedef struct ControlWork2 {
    Page_Status in_memory;
    //int in_memory;
} Page;

Page **get_pages_for_req(int *idate, int N, int *count, int size, Page **pages);
void inizialize(Page **pages, int count);
void optimal_alg(Page **requests, int point, int count, Page **phusical);
int working(Page **requests, int count, Page ***virtual);
void monitor(Page ***virtual);

int main(int argc, char const *argv[]) {
    //1. Самарина Татьяна Акакеевна 	N1 = 7.
    int idate1[] = {19, 1, 14, 1, 18, 10, 15, 1, 20, 1, 20, 30, 33, 15, 1, 1, 12, 1, 12, 6, 6, 3, 15, 1};
    int N1 = 7;
    Page **pages1 = malloc((sizeof(Page) + sizeof(int)) * N1);//WARNING
    inizialize(pages1, N1);

    // for (int i = 0; i < N1; i++)
    // {
    //     printf("inizializied -- %d\n", pages1[i]->in_memory);
    // }
    

    int count1 = 0;
    Page **pages_for_req1 = get_pages_for_req(idate1, N1, &count1, 24, pages1);

    //2. Герман Кирилл Чеевич 		    N2 = 6.
    int idate2[] = {4, 6, 18, 14, 1, 15, 12, 10, 18, 10, 13, 13, 25, 6, 6, 3, 10, 25};
    int N2 = 6;
    Page **pages2 = malloc((sizeof(Page) + sizeof(int)) * N2);
    inizialize(pages2, N2);
    int count2 = 0;
    Page **pages_for_req2 = get_pages_for_req(idate2, N2, &count2, 18, pages2);

    //3. Мельников Валерий Николаевич 	N3 = 7.
    int idate3[] = {14, 6, 13, 30, 15, 10, 12, 16, 3, 3, 1, 13, 6, 18, 10, 11, 15, 10, 12, 16, 13, 1, 6, 3, 10, 25};
    int N3 = 7;
    Page **pages3 = malloc((sizeof(Page) + sizeof(int)) * N3);
    inizialize(pages3, N3);
    int count3 = 0;
    Page **pages_for_req3 = get_pages_for_req(idate3, N3, &count3, 26, pages3);

    //4. Трушков Антон Киркорович 		N4 = 5.
    int idate4[] = {20, 18, 21, 26, 12, 16, 3, 1, 15, 20, 16, 15, 12, 10, 18, 12, 16, 18, 16, 3, 10, 25};
    int N4 = 5;
    Page **pages4 = malloc((sizeof(Page) + sizeof(int)) * N4);
    inizialize(pages4, N4);
    int count4 = 0;
    Page **pages_for_req4 = get_pages_for_req(idate4, N4, &count4, 22, pages4);

    //5. Колмаков Дмитрий Обамович 	    N5 = 7.
    int idate5[] = {12, 16, 13, 14, 1, 12, 16, 3, 5, 14, 10, 20, 18, 10, 11, 16, 2, 1, 14, 16, 3, 10, 25};
    int N5 = 7;
    Page **pages5 = malloc((sizeof(Page) + sizeof(int)) * N5);
    inizialize(pages5, N5);
    int count5 = 0;
    Page **pages_for_req5 = get_pages_for_req(idate5, N5, &count5, 23, pages5);

/******************************************************************************/

    int total_count = count1 + count2 + count3 + count4 + count5;
    Page **request_list = malloc(sizeof(Page *) * total_count);

    for (int i = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0; i < total_count; i++) {
        if (i1 < count1) {
            request_list[i] = pages_for_req1[i1]; 
            i++; i1++;
        }
        if (i2 < count2) {
            request_list[i] = pages_for_req2[i1]; 
            i++; i2++;
        }
        if (i3 < count3) {
            request_list[i] = pages_for_req3[i1]; 
            i++; i3++;
        }
        if (i4 < count4) {
            request_list[i] = pages_for_req4[i1]; 
            i++; i4++;
        }
        if (i5 < count5) {
            request_list[i] = pages_for_req5[i1]; 
            i++; i5++;
        }
    }
    
/******************************************************************************/

    Page ***virtual_memory = malloc(sizeof(Page **) * 5);
    virtual_memory[0] = pages1;
    virtual_memory[1] = pages2;
    virtual_memory[2] = pages3;
    virtual_memory[3] = pages4;
    virtual_memory[4] = pages5;

    
    monitor(virtual_memory);
    // printf("%d\n", pages1[1]->in_memory);
    // printf("%d\n", virtual_memory[0][0]->in_memory);

    //working(request_list, total_count, virtual_memory);


    return 0;
}

Page **get_pages_for_req(int *idate, int N, int *count, int size, Page **pages) {
    Page **requests = malloc(size * sizeof(Page *));
    for (int i = 0, j = 0; i < size; i++) {
        if (idate[i] % N == 0) continue;
        requests[j] = pages[(idate[i] % N)];
        j++;
        *count = *count + 1;
    }
    return requests;
}

void inizialize(Page **pages, int count) {
    for (int i = 0; i < count; i++) {
        pages[i] = malloc(sizeof(Page));
        pages[i]->in_memory = Uninvolved;
        
    }  
}

int working(Page **requests, int count, Page ***virtual) {
    Page **physical = malloc(sizeof(Page *) * MEMORY_LIMIT);
    int memory = 0, page_miss = 0;

    for (int i = 0; i < count; i++) {
        monitor(virtual);
        if (requests[i]->in_memory == Physical) continue;
        if (memory < MEMORY_LIMIT) {
            requests[i]->in_memory = Physical;
            physical[memory] = requests[i];
            memory++;
        } else {
            optimal_alg(requests, i, count, physical);
            page_miss++;
        }
    }
    return page_miss;
}

void optimal_alg(Page **requests, int point, int count, Page **physical) {
    int page_to_swap, longest = 0, length;
    
    for (int i = 0; i < MEMORY_LIMIT; i++) {
        length = 0;
        for (int j = point + 1; j < count; j++) {
            if (physical[i] != requests[j]) {
                length++;
                continue;
            }
            if (length > longest) {
                page_to_swap = i;
                longest = length;
            }
        }   
    }
    physical[page_to_swap]->in_memory = Hard;
    physical[page_to_swap] = requests[point];
    physical[page_to_swap]->in_memory = Physical;
}

void monitor(Page ***virtual) {
    for (int i = 0; i < 5; i++) {
        printf("process %d\n", i + 1);
        for (int j = 0; j < 7; j++) {
            printf("\tpage %d, status -- %d\n", j, virtual[i][j]->in_memory);
        }
    }
    printf("\n");
}