#include <stdio.h>
#include <stdlib.h>

#include <switch.h>
#include <switch/services/hid.h>
#include <switch/services/time.h>

#include "time/time_zone.h"

int main() {
  consoleInit(NULL);

  padConfigureInput(1, HidNpadStyleSet_NpadStandard);

  PadState pad;
  padInitializeDefault(&pad);

  printf("\x1b[1;1HPress PLUS to exit.\n");

  TimeZoneRule *rule = malloc(sizeof(*rule));

  time_data data;
  InitializeTZData(&data);

  timeLoadTimeZoneRule(&data.name_array[0], rule);

  u32 index = 0;
  u8 first_time = 0;

  while (appletMainLoop()) {
    padUpdate(&pad);

    const u64 kDown = padGetButtonsDown(&pad);

    if (kDown & HidNpadButton_Plus) {
      break;
    }

    bool up = kDown & HidNpadButton_AnyUp;
    bool down = kDown & HidNpadButton_AnyDown;

    if (up || down || !first_time) {
      first_time = 1;
      index = index + down - up;

      if ((s32)index > data.location_count) {
        index = data.location_count;
      }

      timeLoadTimeZoneRule(&data.name_array[index], rule);
      PrintTimezones(&data, rule, index);
    }

    // PrintTimezones(&data, rule, index);

    consoleUpdate(NULL);
  }

  consoleExit(NULL);
  return 0;
}
