#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <switch.h>
#include <switch/services/hid.h>
#include <switch/services/time.h>

#include "time/time_zone.h"

static void RandomizeRule(TimeZoneRule *rule) {
  for (u32 i = 0; i < sizeof(*rule); i++) {
    ((u8 *)rule)[i] = rand();
  }
}

int main() {
  consoleInit(NULL);

  padConfigureInput(1, HidNpadStyleSet_NpadStandard);

  PadState pad;
  padInitializeDefault(&pad);

  printf("\x1b[1;1HPress PLUS to exit.\n");

  TimeZoneRule *rule = malloc(sizeof(*rule));

  u32 index = 0;
  time_data data;
  index = InitializeTZData(&data);

  Result result;

  result = timeLoadTimeZoneRule(&data.name_array[index], rule);

  u8 first_time = 0;
  int ats_offset = 0;

  while (appletMainLoop()) {
    padUpdate(&pad);

    const u64 kDown = padGetButtonsDown(&pad);

    if (kDown & HidNpadButton_Plus) {
      break;
    }

    bool dup = kDown & HidNpadButton_Up;
    bool ddown = kDown & HidNpadButton_Down;
    bool dleft = kDown & HidNpadButton_Left;
    bool dright = kDown & HidNpadButton_Right;
    bool dpad = dup || ddown || dleft || dright;

    bool f_a = kDown & HidNpadButton_A;
    bool f_b = kDown & HidNpadButton_B;
    bool f_x = kDown & HidNpadButton_X;
    bool f_y = kDown & HidNpadButton_Y;
    bool f_r = kDown & HidNpadButton_R;
    bool f_l = kDown & HidNpadButton_L;
    bool face_button = f_a || f_b || f_x || f_y || f_r || f_l;

    if (dpad || !first_time || face_button) {
      first_time = 1;
      index = index + dup - ddown + 10 * (dright - dleft);

      ats_offset += f_a * 1 + f_b * -1 + f_x * 100 + f_y * -100;
      if (f_r) {
        ats_offset = 0;
      }

      if ((s32)index < 0) {
        index = 0;
      } else if (index >= (u32)data.location_count) {
        index = data.location_count - 1;
      }

      if (dpad) {
        result = timeLoadTimeZoneRule(&data.name_array[index], rule);
      } else if (f_l) {
        TimeLocationName name;
        strncpy(name.name, "INVALID", 36);
        RandomizeRule(rule);
        result = timeLoadTimeZoneRule(&name, rule);
      }

      PrintTimezones(&data, rule, index);
      if (result == 0) {
        TimeDateCtl(rule, ats_offset);
      } else {
        FILE *f = fopen("rule_data", "w");
        fwrite(rule, sizeof(*rule), 1, f);
        fclose(f);
      }
    }

    consoleUpdate(NULL);
  }

  consoleExit(NULL);
  return 0;
}
