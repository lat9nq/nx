#include <stdio.h>

#include <switch.h>
#include <switch/services/applet.h>
#include <switch/services/hid.h>

int main(int argc, char **argv) {
  consoleInit(NULL);

  padConfigureInput(1, HidNpadStyleSet_NpadStandard);

  PadState pad;
  padInitializeDefault(&pad);

  printf("Press PLUS to exit.\n");

  while (appletMainLoop()) {
    padUpdate(&pad);

    const u64 kDown = padGetButtonsDown(&pad);

    if (kDown & HidNpadButton_Plus) {
      break;
    }

    consoleUpdate(NULL);
  }

  return 0;
}
