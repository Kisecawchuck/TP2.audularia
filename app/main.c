#include "../include/audio.h"

int main() {
    Audio *ring = criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3", 44100);
    convert_to_wav(ring, "./audio/ring.wav");

    return 0;
}
