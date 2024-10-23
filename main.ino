#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <EEPROM.h>

// configuracoes do DISPLAY
#define col 16 
#define lin  2 
#define ende  0x27 

// Definições dos pinos
const int LED_PINS[] = {8, 9, 10, 11};        
const int BUTTONS[] = {2, 3, 4, 5};          
const int BUZZER_PIN = 6;                    
const int BUTTON_MAIN = 0;                    
const int LED_MAIN = 12;                      
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const int MAX_LEVELS = 5;                     
const int POINTS_PER_LEVEL = 5;               
const int POINTS_FOR_CORRECT = 1;             
const long SERIAL_TIMEOUT = 10000;            
int sequence[100];                            
int playerPosition = 0;                       
int sequenceLength = 0;                      
int rounds[2] = {0, 0};                      
int currentPlayer = 0;                        
int currentLevel = 1;                      
int ledDelay;
int numPlayers = 2; 
int correctCount = 0;
bool isGameOver = false;                      
bool continuarJogo = true;

// Diplay
LiquidCrystal_I2C lcd1(ende, col, lin); 

// Componentes do jogo
struct Componente {
  const char* cor;  
  int botao;       
  int led;          
  int tom;         
};

Componente componentes[] = {
  {"Vermelho", BUTTONS[0], LED_PINS[0], 262},  
  {"Verde", BUTTONS[1], LED_PINS[1], 294},     
  {"Azul", BUTTONS[2], LED_PINS[2], 330},      
  {"Amarelo", BUTTONS[3], LED_PINS[3], 392},   
  {"Main", BUTTON_MAIN, LED_MAIN, 440}         
};


// Estrutura para armazenar dados de dificuldade
struct Dificuldade {
    int ledDelay;      
    const char* nome; 
};

// Níveis de dificuldade
Dificuldade dificuldades[] = {
    {1000, "Iniciante"},  
    {500, "Media"},        
    {250, "Dificil"},      
    {125, "Muito Dificil"} 
};

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
void selecionarModoJogo();


void setup() {
    Serial.begin(9600);                         
    
    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(LED_PINS[i], OUTPUT);             
        pinMode(BUTTONS[i], INPUT_PULLUP);        
    }

    pinMode(BUZZER_PIN, OUTPUT);               

    pinMode(LED_MAIN, OUTPUT);                   
    digitalWrite(LED_MAIN, LOW);                 

    pinMode(BUTTON_MAIN, INPUT_PULLUP);          

    lcd1.init();                          
    lcd1.backlight();                          
    lcd1.clear();

    randomSeed(analogRead(0));

    iniciarJogo();
}


void loop() {
    if (!isGameOver) {
        jogarJogo();
    } else {
        lcd1.clear();
        lcd1.setCursor(0, 0);
         lcd1.print("Jogo Completo!");
        lcd1.setCursor(0, 1);
        lcd1.print("P1: ");
        lcd1.print(rounds[0]);
        if (numPlayers == 2) {
            lcd1.print(" P2: ");
            lcd1.print(rounds[1]);
        }

        delay(2000); 

        if (numPlayers == 1) {
            lcd1.setCursor(0, 1);
            lcd1.print("Jogador 1: ");
            lcd1.print(rounds[0]);
        } else {
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
        }

        tocarMelodiaFinal(); 
        delay(3000); 

        continuarJogo = perguntarContinuarJogo();

        if (continuarJogo) {
            reiniciarJogo(); 
        } else {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print("Jogo Terminado!");
            lcd1.setCursor(0, 1);
            lcd1.print("Ate a proxima!");
            delay(2000); 

            while (true) {
                digitalWrite(LED_MAIN, HIGH);
                delay(500);
                digitalWrite(LED_MAIN, LOW);
                delay(500);

                if (digitalRead(BUTTON_MAIN) == LOW) { 
                    delay(200); 
                    lcd1.clear();
                    lcd1.setCursor(0, 0);
                    lcd1.print("Reiniciando");
                    lcd1.setCursor(0, 1);
                    lcd1.print("Jogo...");
                    delay(2000);
                    reiniciarJogo(); 
                    break; 
                }
            }
        }
    }
}

// Função para tocar a melodia de início do jogo.
void tocarMelodiaInicio() {
    int melody[] = {262, 294, 330, 349, 392, 440}; 
    int durations[] = {200, 200, 200, 200, 200, 400}; 

    for (int i = 0; i < 6; i++) {
        tone(BUZZER_PIN, melody[i], durations[i]);
        delay(durations[i] * 1.4);
        noTone(BUZZER_PIN);
    }
}

// Função para tocar a melodia final
void tocarMelodiaFinal() {
    int melody[] = {330, 330, 330, 392, 330, 523, 494,440};
    int durations[] = {250, 250, 250, 250, 250, 250, 250, 500};

    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
        if (melody[i] != 0) { 
            tone(BUZZER_PIN, melody[i], durations[i]);
        }
        delay(durations[i] * 1.3);
        noTone(BUZZER_PIN); 
        delay(50); 
    }
}


// Função principal do loop do jogo. Gerando sequências, alternando jogadores e verificando entradas.
void jogarJogo() {
    currentLevel = 1; 
    isGameOver = false; 
    sequenceLength = 0; 

    while (true) { 
        gerarSequencia(); 

        for (int player = 0; player < numPlayers; player++) { 
            mostrarSequencia();     
            playerPosition = 0; 
            correctCount = 0;   
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print("Jogador ");
            lcd1.print(player + 1);
            lcd1.setCursor(0, 1);
            lcd1.print("Sua Vez!");

            while (true) { 
                verificarEntradaJogador();

                lcd1.setCursor(0, 1);
                lcd1.print("Pos: ");
                lcd1.print(playerPosition + 1);

                if (playerPosition >= sequenceLength) {
                  
                    if (correctCount == sequenceLength) {
                        rounds[player] += POINTS_PER_LEVEL; 
                        Serial.print("Parabéns! Jogador ");
                        Serial.print(player + 1);
                        Serial.print(" completou o nível ");
                        Serial.print(currentLevel);
                        Serial.print(" e ganhou ");
                        Serial.print(POINTS_PER_LEVEL);
                        Serial.println(" pontos. Pontuação total: ");
                        Serial.println(rounds[player]);
                    } else if (correctCount > 0) {
                        rounds[player] += correctCount; 
                        Serial.print("Jogador ");
                        Serial.print(player + 1);
                        Serial.print(" acertou ");
                        Serial.print(correctCount);
                        Serial.println(" LED(s) antes de cometer um erro.");
                        Serial.print("Pontuação total: ");
                        Serial.println(rounds[player]);
                    } else {
                        Serial.print("Jogador ");
                        Serial.print(player + 1);
                        Serial.println(" não acertou nenhum LED.");
                        Serial.print("Pontuação total: ");
                        Serial.println(rounds[player]);
                    }
                    break; 
                }
            }
        }

        if (isGameOver) {
            break;
        }

        if (currentLevel >= MAX_LEVELS) {
            isGameOver = true;
            Serial.println("Nivel maximo atingido. Fim do jogo.");
            break; 
        }

        if (currentLevel < MAX_LEVELS) {
            currentLevel++;
            Serial.print("Avancando para o nivel: ");
            Serial.println(currentLevel);
        }
    }
}

// Função para gerar a sequência de LEDs que o jogador terá que memorizar.
void gerarSequencia() {
    if (sequenceLength < MAX_LEVELS) {
        sequence[sequenceLength] = random(0, NUM_LEDS); 
        sequenceLength++; 
        Serial.print("Sequencia gerada para o nivel ");
        Serial.println(currentLevel);
    }
}

// Mostra a sequência de LEDs gerada para o jogador, um por vez.
void mostrarSequencia() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Sequencia:");

    for (int i = 0; i < sequenceLength; i++) {
        int ledIndex = sequence[i];
        Componente componenteAtual = componentes[ledIndex];
        
        digitalWrite(componenteAtual.led, HIGH);
        tone(BUZZER_PIN, componenteAtual.tom); 
        delay(ledDelay);
        noTone(BUZZER_PIN);
        digitalWrite(componenteAtual.led, LOW);
        delay(500); 

        
        lcd1.setCursor(0, 1);
        lcd1.print(componenteAtual.cor);
        delay(200);
        lcd1.setCursor(0, 1);
        lcd1.print("         "); 
    }
}

// Verifica se o jogador pressionou o botão correto na sequência e atualiza o progresso.
void verificarEntradaJogador() {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (digitalRead(componentes[i].botao) == LOW) {
            Componente componenteAtual = componentes[i];
            
            digitalWrite(componenteAtual.led, HIGH);
            tone(BUZZER_PIN, componenteAtual.tom); 
            delay(200);
            noTone(BUZZER_PIN);
            digitalWrite(componenteAtual.led, LOW);
            
            
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print("Jogador Sua Vez!");
            lcd1.setCursor(0, 1);
            lcd1.print("Botao: ");
            lcd1.print(componenteAtual.cor); 
            
            if (sequence[playerPosition] == i) {
                playerPosition++; 
                correctCount++;
            } else {
                Serial.println("Erro! Jogador passa a vez.");
                lcd1.setCursor(0, 1);
                lcd1.print("Erro! Proxima vez.");
                
                delay(2000);
                
                playerPosition = sequenceLength; 
                break; 
            }
            delay(200); 
        }
    }
}

// Inicia o jogo e exibe informações iniciais no LCD. Configurando a dificuldade e o modo de jogo.
void iniciarJogo() {
    Serial.println("Iniciando o jogo! Pressione o Button Main para começar.");
    lcd1.setCursor(0, 0);
    lcd1.print("Genius Memory");
    lcd1.setCursor(0, 1);
    lcd1.print("Start Game...");

    while (digitalRead(BUTTON_MAIN) == HIGH) {
      delay(100);
    }

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Preparando...");
    lcd1.setCursor(0, 1);
    lcd1.print("Modo de Jogo :");

    while (true) {
        if (digitalRead(BUTTON_MAIN) == LOW) {
            delay(200);

            digitalWrite(LED_MAIN, HIGH);
            delay(500); 
            digitalWrite(LED_MAIN, LOW);

            tone(BUZZER_PIN, componentes[4].tom); 
            delay(500); 
            noTone(BUZZER_PIN); 

            Serial.println("Jogo começando...");

            tocarMelodiaInicio(); 
            selecionarModoJogo(); 
            nivelDificuldade();    
            
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print("Jogo Comecou!");
            lcd1.setCursor(0, 1);
            lcd1.print("Boa Sorte!");
            delay(2000);

            isGameOver = false;
            rounds[0] = 0;
            rounds[1] = 0;
            sequenceLength = 0; 
            currentLevel = 1;    
            
            carregarPontuacoesAltas(); 
            break;
        }
    }
}

// Função chamada para reiniciar o jogo após seu término.
void reiniciarJogo() {
    isGameOver = false;
    rounds[0] = 0;
    rounds[1] = 0;
    sequenceLength = 0;
    currentLevel = 1; 
    continuarJogo = true; 

    for (int i = 0; i < NUM_LEDS; i++) {
        digitalWrite(LED_PINS[i], LOW);
    }

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Jogo Genius");
    lcd1.setCursor(0, 1);
    lcd1.print("Reiniciando...");
    delay(2000);
    iniciarJogo();
}

// Função para selecionar o modo de jogo (1 ou 2 jogadores) usando os botões
void selecionarModoJogo() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("1 ou 2 Jogadores?");
    lcd1.setCursor(0, 1);
    lcd1.print("Verm.:1  Verde:2");

    while (true) {
        if (digitalRead(BUTTONS[0]) == LOW) {
            delay(200); 
            numPlayers = 1;
            Serial.println("Modo de jogo: 1 Jogador");
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print("Modo: 1 Jogador");
            lcd1.setCursor(0, 1);
            lcd1.print("Preparando...");
            tone(BUZZER_PIN, componentes[0].tom); 
            delay(500); 
            noTone(BUZZER_PIN); 
            delay(2000); 
            break;
        }
       
        if (digitalRead(BUTTONS[1]) == LOW) {
            delay(200); 
            numPlayers = 2;
            Serial.println("Modo de jogo: 2 Jogadores");
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print("Modo: 2 Jogadores");
            lcd1.setCursor(0, 1);
            lcd1.print("Preparando...");
            tone(BUZZER_PIN, componentes[1].tom); 
            delay(500); 
            noTone(BUZZER_PIN); 
            delay(2000); 
            break;
        }
    }
}


// Seleciona o nível de dificuldade (Iniciante, Médio, Difícil e Muito Difícil).
void nivelDificuldade() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Qual Dificuldade?");
    lcd1.setCursor(0, 1);
    lcd1.print("R:I G:M B:D Y:H");

    while (true) {
        for (int i = 0; i < NUM_LEDS; i++) {
            if (digitalRead(componentes[i].botao) == LOW) {
                delay(200); 

                ledDelay = dificuldades[i].ledDelay; 
                Serial.print("Dificuldade: ");
                Serial.println(dificuldades[i].nome); 

                lcd1.clear();
                lcd1.setCursor(0, 0);
                lcd1.print("Dificuldade: ");
                lcd1.setCursor(0, 1);
                lcd1.print(dificuldades[i].nome); 
                delay(2000); 

                return; 
            }
        }
    }
}


// Função para perguntar ao jogador se deseja continuar jogando após o término do jogo.
bool perguntarContinuarJogo() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Continuar? (S/N)");
    lcd1.setCursor(0, 1);
    lcd1.print("GREEN:S  RED:N");

    while (true) {

        if (digitalRead(componentes[1].botao) == LOW) {  
            tone(BUZZER_PIN, componentes[0].tom);
            delay(200); 
            noTone(BUZZER_PIN);
            return true; 
        }

        if (digitalRead(componentes[0].botao) == LOW) {  
            tone(BUZZER_PIN, componentes[1].tom);
            delay(200); 
            noTone(BUZZER_PIN);
            return false; 
        }
        
    }
}



// Função para carregar as pontuações altas
void carregarPontuacoesAltas() {
    for (int i = 0; i < 3; i++) {
        rounds[i] = EEPROM.read(i);
        if (rounds[i] == 255) { 
            rounds[i] = 0;
        }
    }

}

// Função para salvar a pontuação mais alta
void salvarPontuacaoAlta(int player, int score) {
    if (numPlayers == 1) {
        if (score > EEPROM.read(0)) {
            EEPROM.write(0, score);
        }
    } else {
        if (player == 0 && score > EEPROM.read(1)) {
            EEPROM.write(1, score);
        } else if (player == 1 && score > EEPROM.read(2)) {
            EEPROM.write(2, score);
        }
    }
}
