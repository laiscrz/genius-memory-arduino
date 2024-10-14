#include <EEPROM.h>
#include <LiquidCrystal.h>

// Definições dos pinos
const int LED_PINS[] = {8, 9, 10, 11};        // LEDs conectados nos pinos 8, 9, 10, 11
const int BUTTONS[] = {2, 3, 4, 5};          // Botões conectados nos pinos 2, 3, 4, 5
const int BUZZER_PIN = 6;                     // Buzzer conectado no pino 6
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const long SERIAL_TIMEOUT = 10000;            // Tempo de espera para entrada serial

int sequence[100];                            // Sequência de LEDs
int playerPosition = 0;                       // Posição atual do jogador na sequência
int sequenceLength = 0;                       // Comprimento atual da sequência
int rounds = 0;                               // Número de rodadas completadas
int difficulty = 1;                           // Dificuldade inicial (1 a 3)
bool isGameOver = false;                      // Estado do jogo

// Inicialização do LCD1602 padrão (paralelo)
LiquidCrystal lcd1(7, 13, A0, A1, A2, A3);     // RS, E, D4, D5, D6, D7

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
  
  // Exibir pontuação alta inicial
  lcd1.setCursor(0, 0);
  lcd1.print("Pontuação Alta:");
  int highScore = loadHighScore();
  lcd1.setCursor(0, 1);
  lcd1.print(highScore);
  
  delay(2000);                                 // Pausa para o usuário ver a pontuação alta
  
  // Exibir informações iniciais para iniciar o jogo
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Simon Says");
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
    lcd1.print("Jogar Novamente?");
    
    Serial.println("Game Over! Você deseja jogar novamente? (s/n)");
    String response = readSerialInput();
    if (response.equalsIgnoreCase("s")) {
      resetGame();
    } else {
      Serial.println("Obrigado por jogar!");
      lcd1.clear();
      lcd1.setCursor(0, 0);
      lcd1.print("Obrigado!");
      while (true); // Para o jogo
    }
  }
}

void playGame() {
  generateSequence();
  showSequence();
  
  playerPosition = 0; // Resetar posição do jogador
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Sua Vez!");
  lcd1.setCursor(0, 1);
  lcd1.print("Pos: 0");
  
  while (playerPosition < sequenceLength) {
    checkPlayerInput();
    // Atualizar a posição no display
    lcd1.setCursor(0, 1);
    lcd1.print("Pos: ");
    lcd1.print(playerPosition + 1);
  }
  
  rounds++;
  Serial.print("Parabéns! Você completou o nível ");
  Serial.println(rounds);
  
  // Atualizar displays
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Pontuação Alta:");
  int highScore = loadHighScore();
  lcd1.setCursor(0, 1);
  lcd1.print(highScore);
  
  delay(2000);
  
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Nível Completo!");
  lcd1.setCursor(0, 1);
  lcd1.print("Nível: ");
  lcd1.print(rounds);
  
  delay(2000);
}

void generateSequence() {
  sequenceLength++;
  if (sequenceLength > 100) sequenceLength = 100; // Limitar a 100
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
    delay(1000 / difficulty); // Dificuldade afeta o tempo
    noTone(BUZZER_PIN);
    digitalWrite(LED_PINS[led], LOW);
    delay(500 / difficulty); // Dificuldade afeta o tempo
    
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
        saveHighScore(rounds);
        break;
      }
      playerPosition++;
      delay(200); // Debounce
    }
  }
}

String readSerialInput() {
  String input = "";
  long startTime = millis();
  while (Serial.available() == 0 && millis() - startTime < SERIAL_TIMEOUT) {}
  if (Serial.available()) {
    input = Serial.readString();
    input.trim();
  } else {
    Serial.println("Timeout! Resposta não recebida.");
    lcd1.setCursor(0, 1);
    lcd1.print("Timeout!");
  }
  return input;
}

void startGame() {
  Serial.println("Iniciando o jogo! Pressione (s) para começar.");
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Pressione 'S' para");
  lcd1.setCursor(0, 1);
  lcd1.print("Começar");
  
  String response = readSerialInput();
  if (response.equalsIgnoreCase("s")) {
    Serial.println("Jogo começando...");
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Jogo Começou!");
    lcd1.setCursor(0, 1);
    lcd1.print("Boa Sorte!");
    delay(2000);
    isGameOver = false;
    rounds = 0;
  } else {
    isGameOver = true;
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Jogo Cancelado");
  }
}

void resetGame() {
  isGameOver = false;
  rounds = 0;
  sequenceLength = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Pontuação Alta:");
  int highScore = loadHighScore();
  lcd1.setCursor(0, 1);
  lcd1.print(highScore);
  
  delay(2000);
  
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Jogo Reiniciado");
  lcd1.setCursor(0, 1);
  lcd1.print("Boa Sorte!");
  delay(2000);
  startGame();
}

void saveHighScore(int score) {
  int currentHighScore = EEPROM.read(0);
  if (score > currentHighScore) {
    EEPROM.write(0, score);
    Serial.print("Novo recorde! Pontuação alta: ");
    Serial.println(score);
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Novo Recorde!");
    lcd1.setCursor(0, 1);
    lcd1.print(score);
  }
}

int loadHighScore() {
  int highScore = EEPROM.read(0);
  Serial.print("Pontuação alta: ");
  Serial.println(highScore);
  return highScore;
}
