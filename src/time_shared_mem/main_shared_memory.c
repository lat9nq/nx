#include <stdio.h>
#include <string.h>
#include <time.h>

#include <switch.h>

int main() {
  consoleInit(NULL);

  // Configure our supported input layout: a single player with standard
  // controller styles
  padConfigureInput(1, HidNpadStyleSet_NpadStandard);

  // Initialize the default gamepad (which reads handheld mode inputs as well as
  // the first connected controller)
  PadState pad;
  padInitializeDefault(&pad);

  u8 *shared_mem = timeGetSharedmemAddr();
  int offset = 0;
  // Main loop
  while (appletMainLoop()) {
    // Scan the gamepad. This should be done once for each frame
    padUpdate(&pad);

    // padGetButtonsDown returns the set of buttons that have been newly pressed
    // in this frame compared to the previous one
    u64 kDown = padGetButtonsDown(&pad);

    if (kDown & HidNpadButton_Plus)
      break; // break in order to return to hbmenu
    if (kDown & HidNpadButton_A && offset < 0xD00) {
      offset += 0x100;
    }
    if (kDown & HidNpadButton_B && offset >= 0x100) {
      offset -= 0x100;
    }

    const int row_length = 16;
    const int row_count = 42;

    printf("\x1b[1;1H");
    for (int e = 0; e < row_count; e++) {
      printf("%04x: ", row_length * e + offset);
      for (int i = 0; i < row_length; i++) {
        if (i + (row_length * e) + offset < 0x1000) {
          printf("%02x%s", *(shared_mem + i + offset + (row_length * e)),
                 i % 2 == 0 ? "" : " ");
        }
      }
      printf("\n");
    }
    consoleUpdate(NULL);
  }

  consoleExit(NULL);
  return 0;
}
