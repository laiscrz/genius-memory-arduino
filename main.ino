#include <LiquidCrystal.h>
#include <EEPROM.h>

// Definições dos pinos
const int LED_PINS[] = {8, 9, 10, 11};        // LEDs conectados nos pinos 8, 9, 10, 11
const int BUTTONS[] = {2, 3, 4, 5};          // Botões conectados nos pinos 2, 3, 4, 5
const int BUZZER_PIN = 6;                     // Buzzer conectado no pino 6
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const long SERIAL_TIMEOUT = 10000;            // Tempo de espera para entrada serial

int sequence[100];                            // Sequência de LEDs
int playerPosition = 0;                       // Posição atual do jogador na sequência
int sequenceLength = 0;                       // Comprimento atual da sequência
int rounds[2] = {0, 0};                       // Contador de rodadas para dois jogadores
int currentPlayer = 0;                        // Jogador atual (0 ou 1)
bool isGameOver = false;                      // Estado do jogo
const int MAX_LEVELS = 5;                     // Limite de níveis
const int POINTS_PER_LEVEL = 3;               // Pontos ganhos por nível completo
const int POINTS_FOR_CORRECT = 1;             // Pontos por LED correto antes do erro
int currentLevel = 1;                          // Nível atual do jogo

int ledDelay; // Variável para armazenar a velocidade dos LEDs

// Inicialização do LCD1602 padrão (paralelo)
LiquidCrystal lcd1(7, 13, A0, A1, A2, A3);

// Declaração de 'correctCount' para uso em ambas as funções
int correctCount = 0;

// Protótipos das funções
void iniciarJogo();
void jogarJogo();
void gerarSequencia();
void mostrarSequencia();
void verificarEntradaJogador();
void reiniciarJogo();
void nivelDificuldade();
void carregarPontuacoesAltas();
void salvarPontuacaoAlta(int player, int score);

void setup() {
    Serial.begin(9600);                         // Inicializar comunicação serial
    
    // Inicializar LEDs e Botões
    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(LED_PINS[i], OUTPUT);             // Configurar LEDs como saída
        pinMode(BUTTONS[i], INPUT_PULLUP);        // Configurar Botões como entrada com resistor de pull-up
    }

    // Inicializar Buzzer
    pinMode(BUZZER_PIN, OUTPUT);                // Configurar Buzzer como saída

    // Inicializar LCD1
    lcd1.begin(16, 2);                           // Inicializar LCD paralelo com 16 colunas e 2 linhas
    lcd1.clear();

    // Carregar pontuações ao iniciar o jogo
    carregarPontuacoesAltas();

    // Exibir informações iniciais para iniciar o jogo
    lcd1.setCursor(0, 0);
    lcd1.print("Genius Memory - Game");
    lcd1.setCursor(0, 1);
    lcd1.print("Pressione S");

    // Semear o gerador de números aleatórios
    randomSeed(analogRead(0));

    // Iniciar o jogo
    iniciarJogo();
}

void loop() {
    if (!isGameOver) {
        jogarJogo();
    } else {
        // Exibir mensagem de Game Over no LCD1
        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print("Game Over!");
        lcd1.setCursor(0, 1);
        lcd1.print("P1: ");
        lcd1.print(rounds[0]);
        lcd1.print(" P2: ");
        lcd1.print(rounds[1]);

        delay(2000); // Pequena pausa para que a pontuação seja visível

        if (rounds[0] > rounds[1]) {
            lcd1.setCursor(0, 1);
            lcd1.print("Jogador 1 Ganhou!");
        } else if (rounds[1] > rounds[0]) {
            lcd1.setCursor(0, 1);
            lcd1.print("Jogador 2 Ganhou!");
        } else {
            lcd1.setCursor(0, 1);
            lcd1.print("Empate!");
        }
        delay(3000); // Aguardar 3 segundos para mostrar a mensagem final
        reiniciarJogo();
    }
}

void jogarJogo() {
    currentLevel = 1; // Começa sempre no nível 1
    isGameOver = false; // Garante que o jogo não está terminado
    sequenceLength = 0; // Reinicia a sequência

    while (true) { // Loop principal do jogo
        gerarSequencia(); // Gera a sequência antes de cada rodada

        for (int player = 0; player < 2; player++) { // Loop para os dois jogadores
            mostrarSequencia();     // Mostra a sequência para o jogador
            playerPosition = 0; // Reinicia a posição do jogador
            correctCount = 0;   // Reinicia o contador de acertos
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print("Jogador ");
            lcd1.print(player + 1);
            lcd1.setCursor(0, 1);
            lcd1.print("Sua Vez!");

            while (true) { // Loop para o jogador atual
                verificarEntradaJogador(); // Verifica a entrada do jogador

                // Atualiza a posição no display
                lcd1.setCursor(0, 1);
                lcd1.print("Pos: ");
                lcd1.print(playerPosition + 1);

                // Verifica se o jogador completou a sequência
                if (playerPosition >= sequenceLength) {
                    // O jogador completou a sequência corretamente
                    if (correctCount == sequenceLength) {
                        rounds[player] += POINTS_PER_LEVEL; // Incrementa a pontuação por completar o nível
                        Serial.print("Parabéns! Jogador ");
                        Serial.print(player + 1);
                        Serial.print(" completou o nível ");
                        Serial.print(currentLevel);
                        Serial.print(" Pontuação: ");
                        Serial.println(rounds[player]);
                    } else if (correctCount > 0) {
                        rounds[player] += correctCount; // Adiciona os pontos por acertos antes do erro
                        Serial.print("Jogador ");
                        Serial.print(player + 1);
                        Serial.print(" acertou ");
                        Serial.print(correctCount);
                        Serial.println(" LED(s) antes do erro.");
                    } else {
                        Serial.print("Jogador ");
                        Serial.print(player + 1);
                        Serial.println(" não acertou nada.");
                    }
                    break; // Saia do loop se o jogador completou a sequência
                }

                // Remover a verificação redundante abaixo
                /*
                if (playerPosition >= sequenceLength) {
                    // Jogador errou; exibir mensagem e passar para o próximo jogador
                    Serial.println("Erro! Jogador passa a vez.");
                    lcd1.setCursor(0, 1);
                    lcd1.print("Erro! Proxima vez.");
                    delay(2000);
                    break; // Passar para o próximo jogador
                }
                */
            }

            // Remover a condição de fim de jogo baseada na soma das pontuações
            /*
            if (rounds[0] + rounds[1] >= MAX_LEVELS * POINTS_PER_LEVEL) {
                isGameOver = true; // Fim do jogo
                break; // Saia do loop se o jogo está acabado
            }
            */
        }

        // Adicionar verificação de isGameOver após o loop dos jogadores
        if (isGameOver) {
            break;
        }

        // Verificar se o nível atual é o último
        if (currentLevel >= MAX_LEVELS) {
            isGameOver = true;
            Serial.println("Nível máximo atingido. Fim do jogo.");
            break; // Encerrar o loop principal do jogo
        }

        // Aumentar o nível apenas após ambos os jogadores completarem a rodada atual
        if (currentLevel < MAX_LEVELS) {
            currentLevel++; // Aumenta o nível do jogo
            Serial.print("Avançando para o nível: ");
            Serial.println(currentLevel);
        }
    }
}

void gerarSequencia() {
    // Adicionar um novo LED à sequência se ainda não atingiu o nível máximo
    if (sequenceLength < MAX_LEVELS) {
        sequence[sequenceLength] = random(0, NUM_LEDS); // Adicionar um novo LED
        sequenceLength++; // Aumentar o comprimento da sequência após adicionar
        Serial.print("Sequência gerada para o nível ");
        Serial.println(currentLevel);
    }
}

void mostrarSequencia() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Sequência:");

    for (int i = 0; i < sequenceLength; i++) {
        int led = sequence[i];
        digitalWrite(LED_PINS[led], HIGH);
        tone(BUZZER_PIN, 440 * (1 << led)); // Nota para o LED
        delay(ledDelay); // Usar ledDelay aqui
        noTone(BUZZER_PIN);
        digitalWrite(LED_PINS[led], LOW);
        delay(500); // Pausa entre LEDs

        // Atualizar sequência no display
        lcd1.setCursor(0, 1);
        lcd1.print("L");
        lcd1.print(led + 1);
        delay(200);
        lcd1.setCursor(0, 1);
        lcd1.print("         "); // Limpar a linha
    }
}

void verificarEntradaJogador() {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (digitalRead(BUTTONS[i]) == LOW) {
            digitalWrite(LED_PINS[i], HIGH);
            tone(BUZZER_PIN, 440 * (1 << i)); // Nota para o botão
            delay(200);
            noTone(BUZZER_PIN);
            digitalWrite(LED_PINS[i], LOW);
            
            // Atualizar entrada do jogador no display
            lcd1.setCursor(0, 1);
            lcd1.print("Botao: ");
            lcd1.print(i + 1);
            
            // Verifique se o jogador acertou
            if (sequence[playerPosition] == i) {
                playerPosition++; // Mova para a próxima posição se acertar
                correctCount++;
            } else {
                // Jogador errou; exibir mensagem e passar para o próximo jogador
                Serial.println("Erro! Jogador passa a vez.");
                lcd1.setCursor(0, 1);
                lcd1.print("Erro! Proxima vez.");
                
                // Esperar um momento para que a mensagem seja lida
                delay(2000);
                
                // Passar para o próximo jogador
                playerPosition = sequenceLength; // Forçar o fim do loop
                break; // Sair do loop de verificação de botões
            }
            delay(200); // Debounce
        }
    }
}

void iniciarJogo() {
    Serial.println("Iniciando o jogo! Pressione (s) para começar.");
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Pressione 'S' para");
    lcd1.setCursor(0, 1);
    lcd1.print("Começar");

    while (true) {
        if (Serial.available()) {
            String response = Serial.readString();
            response.trim();
            if (response.equalsIgnoreCase("s")) {
                Serial.println("Jogo começando...");
                nivelDificuldade(); // Chama a função para escolher o nível de dificuldade
                lcd1.clear();
                lcd1.setCursor(0, 0);
                lcd1.print("Jogo Começou!");
                lcd1.setCursor(0, 1);
                lcd1.print("Boa Sorte!");
                delay(2000);
                isGameOver = false;
                rounds[0] = 0;
                rounds[1] = 0;
                sequenceLength = 0; // Reiniciar o comprimento da sequência ao iniciar o jogo
                currentLevel = 1;    // Reiniciar o nível atual
                break;
            }
        }
    }
}

void reiniciarJogo() {
    isGameOver = false;
    rounds[0] = 0;
    rounds[1] = 0;
    sequenceLength = 0;
    currentLevel = 1; // Reiniciar o nível atual

    for (int i = 0; i < NUM_LEDS; i++) {
        digitalWrite(LED_PINS[i], LOW);
    }

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Jogo Reiniciado");
    lcd1.setCursor(0, 1);
    lcd1.print("Pressione 'S'");
    delay(2000);
    iniciarJogo();
}

// Função para selecionar o nível de dificuldade
void nivelDificuldade() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Escolha Dificuldade");
    lcd1.setCursor(0, 1);
    lcd1.print("1: I 2: M 3: H");

    while (true) {
        if (Serial.available()) {
            char choice = Serial.read();
            if (choice == '1') {
                ledDelay = 1000; // Dificuldade iniciante
                Serial.println("Dificuldade: Iniciante");
                break;
            } else if (choice == '2') {
                ledDelay = 500; // Dificuldade Média
                Serial.println("Dificuldade: Média");
                break;
            } else if (choice == '3') {
                ledDelay = 250; // Dificuldade Hard
                Serial.println("Dificuldade: Hard");
                break;
            }
        }
    }
}

// Função para carregar as pontuações altas
void carregarPontuacoesAltas() {
    // Carregar pontuações do EEPROM
    rounds[0] = EEPROM.read(0);
    rounds[1] = EEPROM.read(1);
}

// Função para salvar a pontuação mais alta
void salvarPontuacaoAlta(int player, int score) {
    if (score > EEPROM.read(player)) {
        EEPROM.write(player, score);
    }
}
