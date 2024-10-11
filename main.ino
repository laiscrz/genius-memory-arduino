#include <EEPROM.h>
#include <SoftwareSerial.h>
 
const int LED_PINS[] = {3, 4, 5, 6};
const int BUTTONS[] = {10, 11, 12, 13};
const int BUZZER_PIN = 9;
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const long SERIAL_TIMEOUT = 10000;
int sequence[100];
int playerPosition = 0;
int sequenceLength = 0;
int rounds = 0;
int difficulty = 1; // Dificuldade inicial (1 a 3)
bool isGameOver = false;
 
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
  pinMode(BUZZER_PIN, OUTPUT);
  randomSeed(analogRead(0));
  loadHighScore();
  startGame();
}
 
void loop() {
  if (!isGameOver) {
    playGame();
  } else {
    Serial.println("Game Over! Você deseja jogar novamente? (s/n)");
    String response = readSerialInput();
    if (response.equalsIgnoreCase("s")) {
      resetGame();
    } else {
      Serial.println("Obrigado por jogar!");
      while (true); // Para o jogo
    }
  }
}
 
void playGame() {
  generateSequence();
  showSequence();
 
  playerPosition = 0; // Resetar posição do jogador
  while (playerPosition < sequenceLength) {
    checkPlayerInput();
  }
 
  rounds++;
  Serial.print("Parabéns! Você completou o nível ");
  Serial.println(rounds);
  delay(1000);
}
 
void generateSequence() {
  sequenceLength++;
  if (sequenceLength > 100) sequenceLength = 100; // Limitar a 100
  sequence[sequenceLength - 1] = random(0, NUM_LEDS); // Adicionar um novo LED
}
 
void showSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    int led = sequence[i];
    digitalWrite(LED_PINS[led], HIGH);
    tone(BUZZER_PIN, 440 * (1 << led)); // Nota para o LED
    delay(1000 / difficulty); // Dificuldade afeta o tempo
    noTone(BUZZER_PIN);
    digitalWrite(LED_PINS[led], LOW);
    delay(500 / difficulty); // Dificuldade afeta o tempo
  }
}
 
void checkPlayerInput() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (digitalRead(BUTTONS[i]) == LOW) {
      digitalWrite(LED_PINS[i], HIGH);
      tone(BUZZER_PIN, 440 * (1 << i)); // Nota para o botão
      delay(500);
      noTone(BUZZER_PIN);
      digitalWrite(LED_PINS[i], LOW);
 
      if (sequence[playerPosition] != i) {
        isGameOver = true;
        Serial.println("Erro! Game Over!");
        saveHighScore(rounds);
        break;
      }
      playerPosition++;
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
  }
  return input;
}
 
void startGame() {
  Serial.println("Iniciando o jogo! Pressione (s) para começar.");
  String response = readSerialInput();
  if (response.equalsIgnoreCase("s")) {
    Serial.println("Jogo começando...");
    isGameOver = false;
    rounds = 0;
  } else {
    isGameOver = true;
  }
}
 
void resetGame() {
  isGameOver = false;
  rounds = 0;
  sequenceLength = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}
 
void saveHighScore(int score) {
  int currentHighScore = EEPROM.read(0);
  if (score > currentHighScore) {
    EEPROM.write(0, score);
    Serial.print("Novo recorde! Pontuação alta: ");
    Serial.println(score);
  }
}
 
void loadHighScore() {
  int highScore = EEPROM.read(0);
  Serial.print("Pontuação alta: ");
  Serial.println(highScore);
}
