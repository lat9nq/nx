#pragma once

#include <switch/services/time.h>

typedef struct {
  s32 tt_gmtoff;
  u32 tt_isdst;
  u8 padding[3];
  u32 tt_abbrind;
  u8 tt_ttisstd;
  u8 tt_ttisgmt;
  u8 padding_b[2];
} ttinfo;

typedef struct {
  u32 timecnt;
  u32 typecnt;
  u32 charcnt;
  u8 goback;
  u8 goahead;
  u8 padding[2];
  u64 ats[1000];
  u8 types[1000];
  ttinfo ttis[128];
  char chars[512];
  u32 defaulttype;
  u8 unused[0x12c4];
} TzRuleData;

typedef struct {
  s32 location_count;
  TimeLocationName *name_array;
} time_data;

int InitializeTZData(time_data *data);
void PrintTimezones(const time_data *data, const TimeZoneRule *rule, int index);

void TimeDateCtl(const TimeZoneRule *rule, int ats_offset);
