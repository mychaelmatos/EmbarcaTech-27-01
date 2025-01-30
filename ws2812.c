/*
 * Código desenvolvido por Mychael Antonio Matos
 * Matrícula: TIC370100452
 * 
 */

#include <stdio.h> // Biblioteca para funções de entrada e saída
#include <stdlib.h> // Biblioteca padrão do C
#include "pico/stdlib.h" // Biblioteca padrão do Pico
#include "hardware/pio.h" // Biblioteca para controle de PIO
#include "hardware/clocks.h" // Biblioteca para controle de clocks
#include "ws2812.pio.h" // Biblioteca para controle dos LEDs WS2812

#define IS_RGBW false // Define se os LEDs são RGBW (falso, são apenas RGB)
#define NUM_PIXELS 25 // Número de LEDs na matriz 5x5
#define WS2812_PIN 7 // Pino de controle dos LEDs WS2812
#define RED_LED_PIN 13 // Pino do LED vermelho
#define tempo 200 // Tempo de piscar o LED vermelho (200 ms)

const uint button_A = 5; // Botão A conectado ao pino 5
const uint button_B = 6; // Botão B conectado ao pino 6

// Variáveis globais
static volatile uint8_t current_number = 0; // Número atual exibido na matriz de LEDs
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

// Prototipação da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events); // Função de interrupção para os botões

// Buffer para armazenar quais LEDs estão ligados na matriz 5x5
bool led_buffer[NUM_PIXELS]; // Buffer de LEDs

// Mapas de bits para números de 0 a 9
const bool number_patterns[10][NUM_PIXELS] = {
    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}, // Padrão para o número 0

    {0, 1, 1, 1, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 0, 0,
     0, 0, 1, 0, 0}, // Padrão para o número 1

    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0}, // Padrão para o número 2

    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0}, // Padrão para o número 3

    {0, 1, 0, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0}, // Padrão para o número 4

    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0}, // Padrão para o número 5

    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0}, // Padrão para o número 6

    {0, 1, 0, 0, 0,
     0, 0, 0, 1, 0,
     1, 1, 1, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0}, // Padrão para o número 7

    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}, // Padrão para o número 8

    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}  // Padrão para o número 9
};

// Função para enviar um pixel para o PIO
static inline void put_pixel(uint32_t pixel_grb){
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); // Envia o pixel para o PIO
}

// Função para converter valores RGB para um único valor de 32 bits
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b){
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b); // Converte RGB para 32 bits
}

// Função para definir o número a ser exibido na matriz de LEDs
void set_number(uint8_t number){
    for (int i = 0; i < NUM_PIXELS; i++){
        led_buffer[i] = number_patterns[number][i]; // Atualiza o buffer de LEDs com o padrão do número
    }
}

// Função para exibir o número na matriz de LEDs
void display_number(){
    uint32_t color = urgb_u32(100, 0, 0); // Define a cor vermelha

    for (int i = 0; i < NUM_PIXELS; i++){
        if (led_buffer[i]){
            put_pixel(color); // Liga o LED com um no buffer
        }else
            put_pixel(0);  // Desliga os LEDs com zero no buffer
    }
}


// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // Obtém o tempo atual em microssegundos

    if (current_time - last_time > 400000){ // 400 ms de debouncing
        last_time = current_time; // Atualiza o tempo do último evento

        if (gpio == button_A){
            current_number = (current_number + 1) % 10; // Incrementa o número e volta para 0 após 9
        }
        else if (gpio == button_B){
            current_number = (current_number == 0) ? 9 : current_number - 1; // Decrementa o número e volta para 9 após 0
        }

        set_number(current_number); // Atualiza o buffer de LEDs com o novo número
    }
}

// Função principal
int main()
{
    stdio_init_all(); // Inicializa todas as funções de entrada e saída

    // Inicializa os botões
    gpio_init(button_A); // Inicializa o botão A
    gpio_set_dir(button_A, GPIO_IN); // Define o botão A como entrada
    gpio_pull_up(button_A); // Ativa o pull-up no botão A
    gpio_init(button_B); // Inicializa o botão B
    gpio_set_dir(button_B, GPIO_IN); // Define o botão B como entrada
    gpio_pull_up(button_B); // Ativa o pull-up no botão B

    // Inicializa o LED vermelho
    gpio_init(RED_LED_PIN); // Inicializa o pino do LED vermelho
    gpio_set_dir(RED_LED_PIN, GPIO_OUT); // Define o pino do LED vermelho como saída

    // Configuração das interrupções com callback
    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // Configura a interrupção para o botão A
    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // Configura a interrupção para o botão B

    PIO pio = pio0; // Define o PIO a ser usado
    int sm = 0; // Define o state machine a ser usado
    uint offset = pio_add_program(pio, &ws2812_program); // Adiciona o programa WS2812 ao PIO

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW); // Inicializa o programa WS2812

    while (1){
        // Pisca o LED vermelho
        gpio_put(RED_LED_PIN, true); // Liga o LED vermelho
        sleep_ms(tempo); // Espera 100 ms
        gpio_put(RED_LED_PIN, false); // Desliga o LED vermelho
        sleep_ms(tempo); // Espera 100 ms

        // Exibe o número atual na matriz de LEDs
        display_number(); // Exibe o número na matriz de LEDs
    }

    return 0; // Retorna 0 ao sistema operacional
}