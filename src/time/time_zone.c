#include "time/time_zone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <switch/services/time.h>
#include <time.h>

int InitializeTZData(time_data *data) {
  Result result = timeGetTotalLocationNameCount(&data->location_count);

  // yuzu has broken tz listing
  if (result != 0) {
    data->name_array = malloc(sizeof(*data->name_array));
    timeGetDeviceLocationName(data->name_array);
    data->location_count = 1;
    return 0;
  }

  TimeLocationName device_name;
  timeGetDeviceLocationName(&device_name);

  data->name_array = (TimeLocationName *)malloc(sizeof(*data->name_array) *
                                                data->location_count);

  timeLoadLocationNameList(0, data->name_array, data->location_count,
                           &data->location_count);

  for (int i = 0; i < data->location_count; i++) {
    if (strcmp(data->name_array[i].name, device_name.name) == 0) {
      return i;
    }
  }
  return 0;
}

void PrintTimezones(const time_data *data, const TimeZoneRule *rule,
                    int index) {
  printf("\x1b[2;1H");

  printf("%-25s\n", data->name_array[index].name);
  const TzRuleData *rule_data = (TzRuleData *)rule;
  const ttinfo *ttis = &rule_data->ttis[0];

  printf("timecnt: %-4u, typecnt: %-4u, charcnt: %-4u\n", rule_data->timecnt,
         rule_data->typecnt, rule_data->charcnt);
  printf("gmtoff: %.2f (%d), isdst: %08x, abbrind: %08x, isstd: %02x, isgmt: "
         "%02x\n",
         ttis->tt_gmtoff / 3600.0, ttis->tt_gmtoff, ttis->tt_isdst,
         ttis->tt_abbrind, ttis->tt_ttisstd, ttis->tt_ttisgmt);
  printf("%s\n", rule_data->chars);
}

void TimeDateCtl(const TimeZoneRule *rule) {
  printf("\x1b[10;1H");

  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);

  TimeCalendarTime nx_caltime;
  TimeCalendarAdditionalInfo nx_caltime_info;

  timeToCalendarTime(rule, ts.tv_sec, &nx_caltime, &nx_caltime_info);

  struct tm cal_time;
  cal_time.tm_sec = nx_caltime.second;
  cal_time.tm_min = nx_caltime.minute;
  cal_time.tm_hour = nx_caltime.hour;
  cal_time.tm_mday = nx_caltime.day;
  cal_time.tm_mon = nx_caltime.month;
  cal_time.tm_year = nx_caltime.year - 1900;
  cal_time.tm_wday = nx_caltime_info.wday;
  cal_time.tm_yday = nx_caltime_info.yday;
  cal_time.tm_isdst = nx_caltime_info.DST;

  struct tm gmt;
  gmtime_r(&ts.tv_sec, &gmt);

  char s[255];
  asctime_r(&cal_time, s);
  printf("%10s: %s", "local", s);
  asctime_r(&gmt, s);
  printf("%10s: %s", "gmt", s);
}
