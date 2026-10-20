#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include "audio.h"
#include "operacoes.h"
#include "fft.h"

#define EPSILON 1e-9
#define PI 3.14159265358979323846
#define SAMPLE_RATE 44100

#define NEAR(a, b) (fabs((a) - (b)) < EPSILON)

static void test_fft_ifft(void) {
    enum { N = 64, K = 5 };
    double complex x[N], X[N], y[N];

    /* argumentos inválidos */
    assert(fft(x, X, 0) != 0);
    assert(fft(x, X, 6) != 0);        /* não é potência de 2 */
    assert(fft(NULL, X, 8) != 0);
    assert(fft(x, NULL, 8) != 0);
    assert(ifft(x, X, 0) != 0);

    /* seno com K ciclos: energia só nos bins K e N-K */
    for (int n = 0; n < N; n++) x[n] = sin(2.0 * PI * K * n / N);
    assert(fft(x, X, N) == 0);
    for (int k = 0; k < N; k++) {
        double esperado = (k == K || k == N - K) ? N / 2.0 : 0.0;
        assert(fabs(cabs(X[k]) - esperado) < 1e-8);
    }

    /* ida e volta (também in-place) */
    assert(ifft(X, y, N) == 0);
    for (int n = 0; n < N; n++) assert(cabs(y[n] - x[n]) < EPSILON);
    assert(fft(y, y, N) == 0 && ifft(y, y, N) == 0);
    for (int n = 0; n < N; n++) assert(cabs(y[n] - x[n]) < EPSILON);
}

static void test_fft_analise(void) {
    enum { N = 1024 };
    const double sr = 8000.0;
    double complex x[N], X[N];
    double mag[N], pot[N];

    /* magnitude e potência */
    double complex v[2] = {3.0 + 4.0 * I, 2.0 * I};
    double m2[2], p2[2];
    fft_magnitude(v, m2, 2);
    fft_power(v, p2, 2);
    assert(NEAR(m2[0], 5.0) && NEAR(m2[1], 2.0));
    assert(NEAR(p2[0], 25.0) && NEAR(p2[1], 4.0));

    /* frequência dos bins */
    assert(NEAR(fft_bin_frequency(128, N, sr), 1000.0));
    assert(NEAR(fft_bin_frequency(N / 2, N, sr), sr / 2));
    assert(fft_bin_frequency(N - 1, N, sr) < 0);      /* frequência negativa */
    assert(fft_bin_frequency(N, N, sr) == -1);        /* bin fora do intervalo */
    assert(fft_bin_frequency(1, N, 0.0) == -1);

    /* pico: senoide de 1000 Hz sobre um DC grande */
    for (int n = 0; n < N; n++) x[n] = 5.0 + sin(2.0 * PI * 128 * n / N);
    assert(fft(x, X, N) == 0);
    fft_magnitude(X, mag, N);
    fft_power(X, pot, N);
    assert(fabs(pot[128] - mag[128] * mag[128]) < 1e-6);
    assert(fabs(fft_peak_frequency(mag, N, sr) - 1000.0) < 1e-6);
}

static void test_fft_janelas(void) {
    double w[5] = {1, 1, 1, 1, 1};
    fft_window_hann(w, 5);
    assert(NEAR(w[0], 0.0) && NEAR(w[2], 1.0) && NEAR(w[4], 0.0));

    double h[5] = {1, 1, 1, 1, 1};
    fft_window_hamming(h, 5);
    assert(NEAR(h[0], 0.08) && NEAR(h[2], 1.0) && NEAR(h[4], 0.08));

    double b[3] = {1, 2, 3};
    fft_window_hann(b, 1);            /* n < 2: não altera */
    assert(b[0] == 1 && b[1] == 2 && b[2] == 3);
}

static void test_fft_filtros(void) {
    enum { N = 1024, BAIXO = 64, ALTO = 384 };   /* 500 Hz e 3000 Hz com sr = 8000 */
    const double sr = 8000.0;
    double complex x[N], X[N], Y[N];

    for (int n = 0; n < N; n++)
        x[n] = sin(2.0 * PI * BAIXO * n / N) + sin(2.0 * PI * ALTO * n / N);
    assert(fft(x, X, N) == 0);

    /* passa-baixa em 1 kHz: sobra só a de 500 Hz */
    for (int k = 0; k < N; k++) Y[k] = X[k];
    assert(fft_low_pass(Y, N, sr, 1000.0) == 0);
    assert(cabs(Y[BAIXO]) > N / 2.0 - 1e-6 && cabs(Y[ALTO]) < EPSILON);

    /* passa-alta em 1 kHz: sobra só a de 3000 Hz */
    for (int k = 0; k < N; k++) Y[k] = X[k];
    assert(fft_high_pass(Y, N, sr, 1000.0) == 0);
    assert(cabs(Y[BAIXO]) < EPSILON && cabs(Y[ALTO]) > N / 2.0 - 1e-6);

    /* argumentos inválidos */
    assert(fft_low_pass(NULL, N, sr, 1000.0) != 0);
    assert(fft_low_pass(X, N, sr, sr) != 0);          /* cutoff acima de Nyquist */
    assert(fft_high_pass(X, N, 0.0, 100.0) != 0);     /* sample_rate inválido */
    assert(fft_high_pass(X, N, sr, -1.0) != 0);
}

static void test_inverter_audio() {
    /* argumentos inválidos */
    assert(inverter_audio(NULL) == 1);
    Audio *Mario = criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3", SAMPLE_RATE);
    Mario->amostras = NULL;
    assert(inverter_audio(Mario) == 1);
    destruir_audio(Mario);

    Audio *Mario2 = criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3", SAMPLE_RATE);
    assert(inverter_audio(Mario2) == 0);
    destruir_audio(Mario2);
}

static void test_cortar_audio() {
    /* argumentos inválidos */
    assert(cortar_audio(NULL, 0.0, 2.0) == 1);
    Audio *Mario = criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3", SAMPLE_RATE);
    assert(cortar_audio(Mario, -1, -1) == 1);
    assert(cortar_audio(Mario, 2.0, 0.0) == 1);
    assert(cortar_audio(Mario, 2.0, 2.0) == 1);
    double tam_erro = (double) Mario->tamanho / SAMPLE_RATE * 10.0;
    assert(cortar_audio(Mario, 0.0, tam_erro) == 1);

    assert(cortar_audio(Mario, 0.0, 2.0) == 0);
    destruir_audio(Mario);
}

static void test_alterar_intensidade() {
    /* argumentos inválidos */
    assert(alterar_intensidade(NULL, 2.0) == 1);
    Audio *Mario = criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3", SAMPLE_RATE);
    assert(alterar_intensidade(Mario, -1) == 1);

    assert(alterar_intensidade(Mario, 2.0) == 0);
    destruir_audio(Mario);

}

static void test_alterar_velocidade() {
    /* argumentos inválidos */
    assert(alterar_velocidade(NULL, 2.0) == 1);
    Audio *Mario = criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3", SAMPLE_RATE);
    assert(alterar_velocidade(Mario, -1) == 1);

    assert(alterar_velocidade(Mario, 2.0) == 0);

    destruir_audio(Mario);
}

static void test_criar_audio() {
    assert(criar_audio(NULL,SAMPLE_RATE) == NULL);
    assert(criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3",-9) == NULL);
}


static void test_destruir_audio() {
    assert(destruir_audio(NULL) == 1);
}

static void test_carregar_amostras() {
    assert(carregar_amostras(NULL) == 1);

    Audio *Mario = criar_audio("./audio/Pururin ringtone (Welcome to the NHK) [nRpSRP0l4bo].mp3", SAMPLE_RATE);
    Mario->filename = NULL;
    assert(carregar_amostras(Mario) == 1);
}

static void test_remover_arquivo_audio(){
    assert(remover_arquivo_audio(NULL) == 1);

    assert(remover_arquivo_audio(criar_audio("Arquivo inexistente",SAMPLE_RATE)) == 1);
}


static void test_audio() {
    test_criar_audio();
    test_destruir_audio();
    test_carregar_amostras();
    test_remover_arquivo_audio();
    puts("audio: OK");
}

static void test_fft() {
    test_fft_ifft();
    test_fft_analise();
    test_fft_janelas();
    test_fft_filtros();
    puts("fft: OK");
}

static void test_operacoes() {
    test_inverter_audio();
    test_cortar_audio();
    test_alterar_intensidade();
    test_alterar_velocidade();
    puts("operacoes: OK");
}

int main() {
    test_audio();
    test_operacoes();
    test_fft();
}
