# Projeto WS2812

Este projeto é uma tarefa da residência do CEPEDI de Sistemas Embarcados, Embarcatech. O objetivo do projeto é controlar um display de LEDs WS2812 para exibir números de 0 a 9 utilizando botões para incrementar e decrementar o valor exibido.

## LInk de video demostração e explicação geral

link: https://www.youtube.com/watch?v=2twR1R3Yyqs

## Descrição

O projeto utiliza um microcontrolador para controlar um display de LEDs WS2812. Dois botões são usados para incrementar e decrementar o número exibido no display. A cor dos LEDs é configurada para vermelho.

## Estrutura do Projeto

- `ws2812.c`: Contém a lógica principal para controlar o display de LEDs e tratar as interrupções dos botões.
- `pico_sdk_import.cmake`: Arquivo de configuração do SDK do Raspberry Pi Pico.
- `CMakeLists.txt`: Arquivo de configuração do CMake para compilar o projeto.

## Funcionalidades

- Exibir números de 0 a 9 no display de LEDs WS2812.
- Incrementar o número exibido ao pressionar o botão A.
- Decrementar o número exibido ao pressionar o botão B.
- Debouncing de 400 ms para evitar múltiplas leituras de um único pressionamento de botão.

## Como Compilar

1. Clone o repositório.
2. Importe o projeto com a extensão Raspberry Pi Pico.
3. Coloque a placa BitDogLab no modo BOOTSEL, compile e rode o arquivo .c .

## Como Executar

1. Carregue o firmware compilado no Raspberry Pi Pico.
2. Conecte o display de LEDs WS2812 e os botões conforme a configuração do projeto.
3. Utilize os botões para incrementar e decrementar o número exibido no display de LEDs.

## Requisitos

- Placa BitDogLab
- Computador Pessoal

## Licença

Este projeto é licenciado sob a [MIT License](LICENSE).

---

Residência do CEPEDI de Sistemas Embarcados, Embarcatech.
