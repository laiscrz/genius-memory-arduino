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
  const char* cor;  // Nome da cor
  int botao;        // Número do botão
  int led;          // Número do LED
  int tom;          // Frequência do tom em Hz
};

Componente componentes[] = {
  {"Vermelho", BUTTONS[0], LED_PINS[0], 262},  // C4 - Dó
  {"Verde", BUTTONS[1], LED_PINS[1], 294},     // D4 - Ré
  {"Azul", BUTTONS[2], LED_PINS[2], 330},      // E4 - Mi
  {"Amarelo", BUTTONS[3], LED_PINS[3], 392},   // G4 - Sol
  {"Main", BUTTON_MAIN, LED_MAIN, 440}         // Botão e LED principal - Lá
};


// Estrutura para armazenar dados de dificuldade
struct Dificuldade {
    int ledDelay;      // Atraso em milissegundos
    const char* nome;  // Nome da dificuldade
};

// Definição dos níveis de dificuldade
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
    
    // Inicializar LEDs e Botões
    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(LED_PINS[i], OUTPUT);             
        pinMode(BUTTONS[i], INPUT_PULLUP);        
    }

    // Inicializar Buzzer
    pinMode(BUZZER_PIN, OUTPUT);               

    // Inicializar LED Main
    pinMode(LED_MAIN, OUTPUT);                   
    digitalWrite(LED_MAIN, LOW);                 

    // Inicializar Botão Main
    pinMode(BUTTON_MAIN, INPUT_PULLUP);          

    // Inicializar LCD1
    lcd1.init(); // Inicia a comunicação com o display                            
    lcd1.backlight();                          
    lcd1.clear();

    // Exibir informações iniciais para iniciar o jogo
    lcd1.setCursor(0, 0);
    lcd1.print("Genius Memory");
    lcd1.setCursor(0, 1);
    lcd1.print("Start Game...");

    // Semear o gerador de números aleatórios
    randomSeed(analogRead(0));

    // iniciamos um loop para aguardar a pressão do botão
    while (digitalRead(BUTTON_MAIN) == HIGH) {
      delay(100);
    }

    // botão foi pressionado chamamos iniciarJogo
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

        delay(2000); // Pequena pausa para que a pontuação seja visível

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

        // Perguntar ao jogador se deseja continuar
        continuarJogo = perguntarContinuarJogo();

        if (continuarJogo) {
            reiniciarJogo(); // Reinicia o jogo se o jogador quiser continuar
        } else {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print("Jogo Terminado!");
            lcd1.setCursor(0, 1);
            lcd1.print("Ate a proxima!");
            delay(2000); 

            // Entrar em um loop infinito para encerrar o programa (simular)
            while (true) {
                digitalWrite(LED_MAIN, HIGH);
                delay(500);
                digitalWrite(LED_MAIN, LOW);
                delay(500);

                // Verificar se o botão MAIN foi pressionado para reiniciar o jogo
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


void tocarMelodiaInicio() {
    int melody[] = {262, 294, 330, 349, 392, 440}; // Notas da melodia
    int durations[] = {200, 200, 200, 200, 200, 400}; // Duração de cada nota

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


void jogarJogo() {
    currentLevel = 1; // Começa sempre no nível 1
    isGameOver = false; // Garante que o jogo não está terminado
    sequenceLength = 0; // Reinicia a sequência

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

            while (true) { // Loop para o jogador atual
                verificarEntradaJogador();

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
                        Serial.print(" e ganhou ");
                        Serial.print(POINTS_PER_LEVEL);
                        Serial.println(" pontos. Pontuação total: ");
                        Serial.println(rounds[player]);
                    } else if (correctCount > 0) {
                        rounds[player] += correctCount; // Adiciona os pontos por acertos antes do erro
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

        // Adicionar verificação de isGameOver após o loop dos jogadores
        if (isGameOver) {
            break;
        }

        // Verificar se o nível atual é o último
        if (currentLevel >= MAX_LEVELS) {
            isGameOver = true;
            Serial.println("Nivel maximo atingido. Fim do jogo.");
            break; 
        }

        // Aumentar o nível apenas após todos os jogadores completarem a rodada atual
        if (currentLevel < MAX_LEVELS) {
            currentLevel++; // Aumenta o nível do jogo
            Serial.print("Avancando para o nivel: ");
            Serial.println(currentLevel);
        }
    }
}

void gerarSequencia() {
    // Adicionar um novo LED à sequência se ainda não atingiu o nível máximo
    if (sequenceLength < MAX_LEVELS) {
        sequence[sequenceLength] = random(0, NUM_LEDS); 
        sequenceLength++; 
        Serial.print("Sequencia gerada para o nivel ");
        Serial.println(currentLevel);
    }
}

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

void verificarEntradaJogador() {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (digitalRead(componentes[i].botao) == LOW) {
            Componente componenteAtual = componentes[i];
            
            digitalWrite(componenteAtual.led, HIGH);
            tone(BUZZER_PIN, componenteAtual.tom); 
            delay(200);
            noTone(BUZZER_PIN);
            digitalWrite(componenteAtual.led, LOW);
            
            
            lcd1.setCursor(0, 1);
            lcd1.print("Botao: ");
            lcd1.print(componenteAtual.cor); 
            
            // Verifique se o jogador acertou
            if (sequence[playerPosition] == i) {
                playerPosition++; 
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
                break; 
            }
            delay(200); 
        }
    }
}

void iniciarJogo() {
    Serial.println("Iniciando o jogo! Pressione o Button Main para começar.");
    lcd1.print("Pressione Main");
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Preparando...");
    lcd1.setCursor(0, 1);
    lcd1.print("Modo de Jogo :");

    // Aguarda até o botão Main ser pressionado
    while (true) {
        if (digitalRead(BUTTON_MAIN) == LOW) {
            delay(200); // Debounce

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

void reiniciarJogo() {
    isGameOver = false;
    rounds[0] = 0;
    rounds[1] = 0;
    sequenceLength = 0;
    currentLevel = 1; // Reiniciar o nível atual
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
        // Verificar se o botão Vermelho (BUTTONS[0]) está pressionado para 1 Jogador
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
        // Verificar se o botão Verde (BUTTONS[1]) está pressionado para 2 Jogadores
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


void nivelDificuldade() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Qual Dificuldade?");
    lcd1.setCursor(0, 1);
    // Vermelho/RED (R) - Iniciante
    // Verde/GREEN (G) - Media
    // Azul/BLUE (B) - Dificil
    // Amarelo/YELLOW (Y) - Muito DIficil
    lcd1.print("R:I G:M B:D Y:H");

    while (true) {
        for (int i = 0; i < NUM_LEDS; i++) {
            if (digitalRead(componentes[i].botao) == LOW) {
                delay(200); 

                // Define a dificuldade com base no botão pressionado
                ledDelay = dificuldades[i].ledDelay; 
                Serial.print("Dificuldade: ");
                Serial.println(dificuldades[i].nome); 

                // Exibe a dificuldade selecionada no LCD
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


// Função para perguntar ao jogador se deseja continuar
bool perguntarContinuarJogo() {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Continuar? (S/N)");
    lcd1.setCursor(0, 1);
    lcd1.print("GREEN:S  RED:N");

    // Aguardar a escolha do jogador
    while (true) {
        // Verificar botão Verde (S)
        if (digitalRead(componentes[1].botao) == LOW) {  
            tone(BUZZER_PIN, componentes[0].tom);
            delay(200); 
            noTone(BUZZER_PIN);
            return true; 
        }
        // Verificar botão Vermelho (N)
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
    // Carregar pontuações do EEPROM
    // Endereços:
    // 0: Player 1 - 1 Jogador (modo 1)
    // 1: Player 1 - 2 Jogadores (modo 2)
    // 2: Player 2 - 2 Jogadores (modo 2)
   // Carregar pontuações do EEPROM
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
        // Salvar pontuação para 1 jogador
        if (score > EEPROM.read(0)) {
            EEPROM.write(0, score);
        }
    } else {
        // Salvar pontuação para 2 jogadores
        if (player == 0 && score > EEPROM.read(1)) {
            EEPROM.write(1, score);
        } else if (player == 1 && score > EEPROM.read(2)) {
            EEPROM.write(2, score);
        }
    }
}
