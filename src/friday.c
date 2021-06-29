/**
 * @file friday.c
 * @author maszynatrurla
 * @brief Programming exercise: Count number of Friday 13s over a few years.
 * 
 * Inspiration: http://programmingpraxis.com/2009/03/13/friday-the-thirteenth/
 */

#include <stdio.h>
#include <time.h>

#define DAY_S           24*3600
#define FRIDAY          5

void printt(struct tm * tim) {
    printf("%d-%02d-%02d\n", 1900 + tim->tm_year, tim->tm_mon + 1, tim->tm_mday);
}

int main(int argc, char ** argv) {
    time_t      now      = time(0);
    struct tm * tim      = localtime(&now);
    int         found    = 0;
    int         nowDay   = tim->tm_mday;
    int         nowYear  = tim->tm_year;
    int         nowMonth = tim->tm_mon;
    
    printf("Today is ");
    printt(tim);
    
    for (now = now + DAY_S;; now += DAY_S) {
        struct tm * tim = localtime(&now);
        
        if (13 == tim->tm_mday && FRIDAY == tim->tm_wday) {
            ++found;
            printt(tim);
        }
        if (tim->tm_mday == nowDay && tim->tm_mon == nowMonth
                && tim->tm_year == 10 + nowYear) {
            break;
        }
    }
    printf("Total: %d\n", found);
    return 0;
}


