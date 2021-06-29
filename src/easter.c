/**
 * @file easter.c
 * @author maszynatrurla
 * @brief Programming exercise: Calculate date of Mardi Gras.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/24/mardi-gras/
 */

#include <stdio.h>
#include <time.h>

/* Constants ripped off from: http://www.ben-daglish.net/moon.shtml */
#define MOON_PERIOD_S           2551443
#define FULL_MOON_YEAR          1970
#define FULL_MOON_MONTH         0
#define FULL_MOON_DAY           22
#define FULL_MOON_HOUR          20
#define FULL_MOON_MINUTE        35

#define EQUINOX_MONTH           2
#define EQUINOX_DAY             21

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
} MyDate;


static void calc(int year, struct tm * date);


static struct tm fullMoon = {
    .tm_year = FULL_MOON_YEAR,
    .tm_mon  = FULL_MOON_MONTH,
    .tm_mday = FULL_MOON_DAY,
    .tm_hour = FULL_MOON_HOUR,
    .tm_min  = FULL_MOON_MINUTE,
    .tm_sec  = 0,
};

static void calc(int year, struct tm * date) {
    time_t then, now, dd;
    struct tm * oj;
    
    date->tm_year = year;
    date->tm_mon  = EQUINOX_MONTH;
    date->tm_mday = EQUINOX_DAY;
    date->tm_hour = FULL_MOON_HOUR;
    date->tm_min  = FULL_MOON_MINUTE;
    date->tm_sec  = 0;
    
    then = mktime(&fullMoon);
    now  = mktime(date);
    dd = (now - then) % MOON_PERIOD_S;
    if (dd > 0) {
        now += (MOON_PERIOD_S - dd);
    }
    oj = localtime(&now);
    if (0 != oj->tm_wday) {
        now += 24 * 3600 * (7 - oj->tm_wday);
    }
    now -= 54 * 24 * 3600;
    oj = localtime(&now);
    
    date->tm_mon = oj->tm_mon;
    date->tm_mday = oj->tm_mday;
    date->tm_wday = oj->tm_wday;
    printf("%d\n", date->tm_wday);
}


int main(int argc, char ** argv) {
    int year;
    struct tm result;
    if (argc != 2) {
        printf("Calculate date of Mardi Gras.\nUsage:\n%s YEAR\n", argv[0]);
        return 1;
    }
    sscanf(argv[1], "%d", &year);
    if (year < 1800 || year >2200) {
        printf("Unable to calculate for year %d.\n", year);
        return 1;
    }
    calc(year, &result);
    printf("%02d %02d %d\n", result.tm_mday, result.tm_mon + 1, result.tm_year);
    return 0;
}

