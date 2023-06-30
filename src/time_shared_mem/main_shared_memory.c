#include <string.h>
#include <stdio.h>
#include <time.h>

#include <switch.h>

int main(int argc, char **argv)
{
    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

    
    u8* shared_mem=timeGetSharedmemAddr();
    int offset =0;
    // Main loop
    while(appletMainLoop())
    {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break; // break in order to return to hbmenu
        if (kDown & HidNpadButton_A && offset<0xD00){
            offset+=0x100;
            
        }
        if (kDown & HidNpadButton_B && offset>=0x100){
            offset-=0x100;
        }

        printf("\x1b[1;1H");
        for(int e=0;e<42;e++){
            printf("%04x: ",24*e + offset);
            for(int i=0;i<24;i++){
                if(i+ (24*e) + offset < 0x1000){
                    printf("%02x ",*(shared_mem+ i+ offset +(24*e)) );
                }
            }
            printf("\n");
        }
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
