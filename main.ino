#include <LiquidCrystal.h>
#include <EEPROM.h>  // Biblioteca para EEPROM

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

int ledDelay; // Variável para armazenar a velocidade dos LEDs

// Inicialização do LCD1602 padrão (paralelo)
LiquidCrystal lcd1(7, 13, A0, A1, A2, A3);

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
    loadHighScores();

    // Exibir informações iniciais para iniciar o jogo
    lcd1.setCursor(0, 0);
    lcd1.print("Genius Memory - Game");
    lcd1.setCursor(0, 1);
    lcd1.print("Pressione S");

    // Semear o gerador de números aleatórios
    randomSeed(analogRead(0));

    // Iniciar o jogo
    startGame();
}

void loop() {
    if (!isGameOver) {
        playGame();
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
        delay(5000); // Aguardar 5 segundos para mostrar a pontuação final
        resetGame();
    }
}

void playGame() {
    generateSequence();
    showSequence();

    playerPosition = 0; // Resetar posição do jogador
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Jogador ");
    lcd1.print(currentPlayer + 1);
    lcd1.setCursor(0, 1);
    lcd1.print("Sua Vez!");

    while (playerPosition < sequenceLength) {
        checkPlayerInput();
        // Atualizar a posição no display
        lcd1.setCursor(0, 1);
        lcd1.print("Pos: ");
        lcd1.print(playerPosition + 1);
    }

    rounds[currentPlayer]++; // Incrementar rodadas do jogador atual
    saveHighScore(currentPlayer, rounds[currentPlayer]); // Salvar a pontuação mais alta
    Serial.print("Parabéns! Jogador ");
    Serial.print(currentPlayer + 1);
    Serial.print(" completou o nível ");
    Serial.println(rounds[currentPlayer]);

    // Atualizar displays
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Nível Completo!");
    lcd1.setCursor(0, 1);
    lcd1.print("Jogador: ");
    lcd1.print(currentPlayer + 1);
    delay(2000);

    // Passar para o próximo jogador
    currentPlayer = (currentPlayer + 1) % 2;
    if (rounds[0] + rounds[1] >= MAX_LEVELS) {
        isGameOver = true; // Se ambos os jogadores jogaram o limite de níveis
    }
}

void generateSequence() {
    sequenceLength++;
    if (sequenceLength > NUM_LEDS) sequenceLength = NUM_LEDS; // Limitar a quantidade de LEDs
    sequence[sequenceLength - 1] = random(0, NUM_LEDS); // Adicionar um novo LED
}

void showSequence() {
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

void checkPlayerInput() {
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
            
            if (sequence[playerPosition] != i) {
                isGameOver = true;
                Serial.println("Erro! Game Over!");
                lcd1.setCursor(0, 1);
                lcd1.print("Erro no Botao!");
                break;
            }
            playerPosition++;
            delay(200); // Debounce
        }
    }
}

void startGame() {
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
                currentPlayer = 0; // Reiniciar jogador atual
                break;
            }
        }
    }
}

void resetGame() {
    isGameOver = false;
    rounds[0] = 0;
    rounds[1] = 0;
    sequenceLength = 0;

    for (int i = 0; i < NUM_LEDS; i++) {
        digitalWrite(LED_PINS[i], LOW);
    }

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Jogo Reiniciado");
    lcd1.setCursor(0, 1);
    lcd1.print("Pressione 'S'");
    delay(2000);
    startGame();
}

// Função para selecionar o nível de dificuldade
void nivelDificuldade() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Escolha Dificuldade");
    lcd1.setCursor(0, 1);
    lcd1.print("1: F 2: M 3: D");

    while (true) {
        if (Serial.available()) {
            char choice = Serial.read();
            if (choice == '1') {
                ledDelay = 1000; // Dificuldade Fácil
                break;
            } else if (choice == '2') {
                ledDelay = 500; // Dificuldade Média
                break;
            } else if (choice == '3') {
                ledDelay = 250; // Dificuldade Difícil
                break;
            }
        }
    }
}

// Função para carregar as pontuações altas
void loadHighScores() {
    // Carregar pontuações do EEPROM
    rounds[0] = EEPROM.read(0);
    rounds[1] = EEPROM.read(1);
}

// Função para salvar a pontuação mais alta
void saveHighScore(int player, int score) {
    if (score > EEPROM.read(player)) {
        EEPROM.write(player, score);
    }
}
