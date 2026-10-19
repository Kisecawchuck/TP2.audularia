#include "audio.h"
#include "fft_audio.h"

int main() {
    Audio *ring = criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3", 44100);
    audio_passa_baixo(ring, 1000.0);

    return 0;
}
