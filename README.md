# 🎮 Jogo da Memória Genius com Arduino

> **Submetido como Checkpoint 5 da disciplina de DISRUPTIVE ARCHITECTURES: IOT, IOB & GENERATIVE IA.** 📚
 
> **Professor: [Arnaldo Jr.](https://github.com/arnaldojr)** 👨🏻‍🏫

## 🎯 Objetivo
Desenvolver um protótipo do jogo "Genius" utilizando Arduino, com o intuito de avaliar a compreensão dos alunos em programação, montagem de circuitos e integração de projetos.

## 🔧 Materiais Necessários
- 🖥️ **Arduino UNO**
- 💡 **4 (ou mais) LEDs** de cores variadas
- 🔘 **4 (ou mais) botões**
- 🎵 **1 Buzzer**
- 🔌 **Resistores, jumpers e uma protoboard**
- 🖥️ **Display LCD**

## 📑 Instruções de Montagem
1. **Conecte os LEDs**: Ligue cada LED em um pino digital do Arduino.
2. **Conecte os Botões**: Conecte os botões aos pinos digitais usando resistores de pull-up.
3. **Conecte o Buzzer**: Conecte o buzzer a um pino digital do Arduino.
4. **Conecte o Display LCD**: Conecte o display LCD para mostrar informações relevantes.

## ⚙️ Programação
O código para o jogo "Genius" está disponível no seguinte link:

[**🚀 Projeto Genius no Wokwi**](https://wokwi.com/projects/411729434682390529)

### 📜 Explicação do Código
- O jogo gera uma sequência aleatória de LEDs que o jogador deve reproduzir.
- O jogador interage com os botões para inserir a sequência correta.
- O buzzer emite sons específicos para cada LED aceso.
- O display LCD exibe mensagens e pontuações, aprimorando a experiência do usuário.

## ⚙️ Fases do Jogo
O jogo possui 4 níveis de dificuldade, com aumento progressivo na velocidade dos LEDs e na complexidade da sequência a cada nível. 🚀📈

## 📡 Monitor Serial
O jogo pode ser jogado pelos botões físicos e visualizar pelo Display LCD, permitindo maior flexibilidade na interação. 🖥️🔄

## 🔧 Montagem do Circuito
![Diagrama do Circuito](https://github.com/user-attachments/assets/6d437544-e65d-49b5-854e-a01f5c2dc503)

## 💡 Requisitos Funcionais
### Requisitos Básicos
- **LEDs**: 4 LEDs de cores variadas. 🌈
- **Botões**: 4 botões, cada um correspondente a um LED. 🔘🔲
- **Buzzer**: Emissão de som correspondente a cada LED aceso. 🎶🔊

### Requisitos Avançados
- **Níveis de Dificuldade**: Implementar a função `nivelDificuldade()` para ajustar a velocidade dos LEDs conforme o progresso do jogo. ⚙️⚡
- **Salvar Pontuações**: Utilizar EEPROM para armazenar as pontuações mais altas e permitir reset. 💾🏆
- **Modo de Jogo**: Permitir a escolha entre 1 ou 2 jogadores, alternando entre os modos de forma intuitiva. 📱🔗

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
