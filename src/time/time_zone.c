#include "time/time_zone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <switch/services/time.h>
#include <time.h>

void InitializeTZData(time_data *data) {
#ifdef YUZU
  data->name_array = malloc(sizeof(*data->name_array));
  timeGetDeviceLocationName(data->name_array);
  data->location_count = 1;
#else
  timeGetTotalLocationNameCount(&data->location_count);

  data->name_array = (TimeLocationName *)malloc(sizeof(*data->name_array) *
                                                data->location_count);

  timeLoadLocationNameList(0, data->name_array, data->location_count,
                           &data->location_count);
#endif
}

void PrintTimezones(const time_data *data, const TimeZoneRule *rule,
                    int index) {
  printf("\x1b[2;1H");

  printf("%-25s\n", data->name_array[index].name);
  const TzRuleData *rule_data = (TzRuleData *)rule;
  const ttinfo *ttis = &rule_data->ttis[0];

  printf("timecmt: %-4u, typecnt: %-4u, charcnt: %-4u, goback: %-4u\n",
         rule_data->timecnt, rule_data->typecnt, rule_data->charcnt,
         rule_data->goback);
  printf("gmtoff: %.1f (%d), isdst: %08x, abbrind: %08x, isstd: %02x, isgmt: "
         "%02x\n",
         ttis->tt_gmtoff / 3600.0, ttis->tt_gmtoff, ttis->tt_isdst,
         ttis->tt_abbrind, ttis->tt_ttisstd, ttis->tt_ttisgmt);
  printf("%s\n", rule_data->chars);
}
