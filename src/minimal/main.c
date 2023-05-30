#include <switch/runtime/devices/console.h>
#include <switch/runtime/pad.h>
#include <switch/services/applet.h>

int main() {
  consoleInit(NULL);

  padConfigureInput(1, HidNpadStyleSet_NpadStandard);

  PadState pad;
  padInitializeDefault(&pad);

  while (appletMainLoop()) {
    padUpdate(&pad);
    const u64 kDown = padGetButtonsDown(&pad);

    if (kDown & HidNpadButton_Plus) {
      break;
    }

    consoleUpdate(NULL);
  }

  consoleExit(NULL);
  return 0;
}
