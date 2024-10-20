# 🎮 Jogo da Memória Genius com Arduino

> **Submetido como Checkpoint 5 da disciplina de DISRUPTIVE ARCHITECTURES: IOT, IOB & GENERATIVE IA.** 📚
 
> **Professor: [Arnaldo Jr.](https://github.com/arnaldojr)** 👨🏻‍🏫

## 🎯 Objetivo
Desenvolver um protótipo do jogo "Genius" utilizando Arduino, com o intuito de avaliar a compreensão dos alunos em programação, montagem de circuitos e integração de projetos.

## 🚀 Funcionalidades

### 📡 Monitor Serial
O jogo pode ser jogado usando botões físicos e visualizado no Display LCD I2C, proporcionando uma interação mais flexível e intuitiva. 🖥️✨🔄

### 🕹️ Modos de Jogo

| **Modo**         | **Botão para Seleção** | **Descrição**                                                                    |
|------------------|------------------------|----------------------------------------------------------------------------------|
| 🔴 **1 Jogador** | Vermelho               | O jogador deve memorizar e repetir a sequência de LEDs sozinho. Ideal para treinar habilidades de memória. |
| 🟢 **2 Jogadores**| Verde                  | Dois jogadores se revezam para repetir a sequência. A sequência cresce a cada rodada, e o jogador que errar perde. |


### 🎮 Níveis de Dificuldade

| **Nível**         | **Botão (Cor)**    | **Descrição**                                  | **Velocidade dos LEDs (ms)** |
|-------------------|--------------------|-----------------------------------------------|-----------------------------|
| 🟠 **Iniciante**   | Vermelho           | Sequências fáceis, tempo de reação longo      | 1000                        |
| 🟢 **Médio**       | Verde              | Sequências moderadas, tempo de reação médio   | 700                         |
| 🔵 **Difícil**     | Azul               | Sequências desafiadoras, tempo de reação rápido| 500                         |
| 🟡 **Muito Difícil**| Amarelo            | Sequências complexas, tempo de reação curto   | 300                         |

### 🏆 Pontuação

| **Ação**                               | **Pontos Ganhados/Perdidos**                | **Descrição**                                                                          |
|---------------------------------------|---------------------------------------------|---------------------------------------------------------------------------------------|
| ✅ **Acertar um LED na sequência**     | +1 ponto por LED acertado                   | O jogador ganha 1 ponto para cada LED corretamente lembrado na sequência.              |
| ❌ **Errar uma sequência**             | 0 pontos                                    | O jogador não ganha pontos por uma sequência incorreta.                                |
| ✅ **Completar um nível**              | +3 pontos por nível completo                | O jogador ganha 3 pontos extras ao completar um nível inteiro com sucesso.             |
| 🏁 **Terminar o jogo**                 | Pontuação total do jogador                  | A pontuação total dos jogadores é exibida ao final, determinando o vencedor.           |

#### 📜 Regras de Pontuação:
- **1 Jogador**: A pontuação vai sendo acumulada com cada sequência correta, com penalidades aplicadas por erros.
- **2 Jogadores**: Os pontos são acumulados individualmente. A vez passa para o outro jogador em caso de erro, e os pontos são ajustados conforme o desempenho de cada um.

## 🛠️ Materiais Necessários e Montagem do Circuito
- 🖥️ **Arduino UNO**
- 🌈 **4 (ou mais) LEDs** de cores variadas
- 🔘 **4 (ou mais) botões**
- 🎵 **1 Buzzer**
- 🧰 **Resistores, jumpers e uma protoboard**
- 🖥️ **Display LCD I2C**
  
![Diagrama do Circuito](https://github.com/user-attachments/assets/e6d9cc21-0a01-4018-a283-f8501478b470) 

## ⚙️ Programação
Confira a simulação online do código e do circuito para o jogo "Genius" no link a seguir:

[**🚀 Projeto Genius no Wokwi**](https://wokwi.com/projects/411729434682390529)

## 💡 Requisitos Funcionais
### Requisitos Básicos
- **LEDs**: 4 LEDs de cores variadas. 🌈
- **Botões**: 4 botões, cada um correspondente a um LED. 🔘🔲
- **Buzzer**: Emissão de som correspondente a cada LED aceso. 🎶🔊
- **Display LCD**: Exibição de mensagens e pontuações durante o jogo. 🖥️📊
- **Monitor Serial**: Visualização de informações diretamente no computador. 🖥️🔄 

### Requisitos Avançados
- **Níveis de Dificuldade**: Implementar a função `nivelDificuldade()` para ajustar a velocidade dos LEDs conforme o progresso do jogo. ⚙️🚀
- **Salvar Pontuações**: Utilizar EEPROM para armazenar as pontuações. 💾🏅
- **Modo de Jogo**: Permitir a escolha entre 1 ou 2 jogadores, alternando entre os modos de forma intuitiva. 🎮👥

## 📦 Construindo uma Caixa Personalizada
A caixa do protótipo será projetada no estilo do **Nintendo Switch**, utilizando [OnShape](https://www.onshape.com/) para um design estético e funcional. A impressão em 3D garantirá durabilidade e um visual atraente. 🎮✨

## 🫂 Integrantes

Aqui estão os membros do grupo que participaram durante desenvolvimento deste Checkpoint.

* **RM 552258 - Laís Alves da Silva Cruz**
  - Turma: 2TDSPH

* **RM 552267 - Bianca Leticia Román Caldeira**
  - Turma: 2TDSPH
    
* **RM 552252 – Charlene Aparecida Estevam Mendes Fialho**
  - Turma: 2TDSPH

* **RM 97916 – Fabricio Torres Antonio**
  - Turma: 2TDSPH

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/laiscrz">
        <img src="https://avatars.githubusercontent.com/u/133046134?v=4" width="100px;" alt="Lais Alves's photo on GitHub"/><br>
        <sub>
          <b>Lais Alves</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/biancaroman">
        <img src="https://avatars.githubusercontent.com/u/128830935?v=4" width="100px;" border-radius='50%' alt="Bianca Román's photo on GitHub"/><br>
        <sub>
          <b>Bianca Román</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/charlenefialho">
        <img src="https://avatars.githubusercontent.com/u/94643076?v=4" width="100px;" border-radius='50%' alt="Charlene Aparecida's photo on GitHub"/><br>
        <sub>
          <b>Charlene Aparecida</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/Fabs0602">
        <img src="https://avatars.githubusercontent.com/u/111320639?v=4" width="100px;" border-radius='50%' alt="Fabricio Torres's photo on GitHub"/><br>
        <sub>
          <b>Fabricio Torres</b>
        </sub>
      </a>
    </td>
  </tr>
</table>
