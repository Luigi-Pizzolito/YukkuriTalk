#include "synthcall.h"
#include <AquesTalk.h>
#include <stdio.h>

// Synthesize function
unsigned char* synth(const char *str, int *size) {
    if(AquesTalk_SetDevKey("!AACAAACAAAAAAPmCAAACAAAAAAPmIAA")!=0) {
		fprintf(stderr, "# ERR DEV LICENCE KEY\n");
        *size = -1; // Set size to -1 to indicate error
		return NULL;
	}

    AQTK_VOICE voice;
    // Set voice parameters
    voice = gVoice_F2;
    voice.spd = 120;
    voice.pit = 150;
    voice.lmd = 120;

    // Perform speech synthesis
    unsigned char *wav = AquesTalk_Synthe_Utf8(&voice, str, size);
    if (wav == NULL) {
        fprintf(stderr, "ERR:%d\n", *size);
        *size = -1; // Set size to -1 to indicate error
    }

    return wav;
}

// Free mem function
void free_synth(unsigned char *wav) {
    // Free the wave data buffer
    AquesTalk_FreeWave(wav);
}