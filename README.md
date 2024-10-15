# 🎮 Jogo da Memória Genius com Arduino

> Submetido como Checkpoint 5 da disciplina de DISRUPTIVE ARCHITECTURES: IOT, IOB & GENERATIVE IA. 📚

> Professor: [Arnaldo Jr.](https://github.com/arnaldojr) 👨🏻‍🏫

## 🎯 Objetivo
O objetivo deste projeto é criar um protótipo do jogo da memória chamado "Genius" utilizando Arduino. Este projeto permitirá avaliar a compreensão dos alunos sobre os conteúdos ministrados na disciplina, incluindo programação, montagem de circuitos e integração de projetos.

## 🔧 Materiais Necessários
Para construir o protótipo do jogo da memória, você precisará dos seguintes componentes:

- 🖥️ **Arduino UNO**
- 💡 **4 (ou mais) LEDs** de cores diferentes (vermelho, verde, azul, amarelo)
- 🔘 **4 (ou mais) botões** para interação
- 🎵 **1 Buzzer** para sons
- 🔌 **Resistores, jumpers e uma protoboard** para conexões
- 🖥️ **Display LCD** para melhor visualização das informações

## 📑 Instruções de Montagem
1. **Conecte os LEDs**: 🔗 Ligue cada LED em um pino digital do Arduino.
2. **Conecte os Botões**: 🔘 Conecte os botões aos pinos digitais, usando resistores de pull-up para evitar leituras flutuantes.
3. **Conecte o Buzzer**: 🎶 Conecte o buzzer a um pino digital do Arduino para emitir sons.
4. **Conecte o Display LCD**: 🖥️ Se desejar, conecte um display LCD para mostrar informações e pontuações.

## ⚙️ Programação
O código para o jogo da memória "Genius" pode ser encontrado no seguinte link:

[**🚀 Projeto Genius no Wokwi**](https://wokwi.com/projects/411729434682390529)

### 📜 Explicação do Código
- 🎲 O jogo gera uma sequência aleatória de LEDs que o jogador deve reproduzir.
- 🕹️ O jogador interage com os botões para inserir a sequência.
- 🔊 O buzzer emite sons correspondentes a cada LED aceso.
- 📺 O display LCD pode ser utilizado para exibir mensagens e pontuações.

## ⚙️ Fases do Jogo
O jogo possui 4 níveis de dificuldade, e o jogador pode progredir conforme avança. A velocidade dos LEDs e a complexidade da sequência aumentam a cada nível. 🚀📈

## 📡 Monitor Serial
O jogo permite jogar tanto pelos botões físicos quanto pelo monitor serial do Arduino, facilitando a interação. 🖥️🔄

## 🔧 Montagem do Circuito
![Diagrama do Circuito](https://github.com/user-attachments/assets/ea7d0197-6c2d-4bd9-ba1a-37805cbb97ff)

## 💡 Requisitos Funcionais
### Requisitos Básicos
- **LEDs**: O jogo deve possuir 4 LEDs de cores diferentes. 🌈
- **Botões**: O jogo deve possuir 4 botões, cada um correspondente a um LED. 🔘🔲
- **Buzzer**: O buzzer deve emitir uma frequência específica para cada LED, tanto na sequência aleatória quanto ao pressionar os botões. 🎶🔊

### Requisitos Avançados
- **Níveis de Dificuldade**: Implementar uma função `nivelDificuldade` que altera a velocidade dos LEDs. ⚙️⚡
- **Salvar Pontuações**: Usar EEPROM para armazenar as pontuações mais altas. 💾🏆
- **Comunicação Bluetooth**: Integrar comunicação Bluetooth para jogar via notebook ou celular. 📱🔗
- **Comando de Voz**: Utilizar um script em Python para anunciar a cor acionada. 🗣️🔊

## 📦 Construindo uma Caixa Personalizada
Para o nosso protótipo do jogo da memória "Genius," vamos nos inspirar no design elegante e funcional do **Nintendo Switch**. 🎮✨ A nossa caixa personalizada não apenas protegerá os componentes internos, mas também proporcionará uma experiência estética semelhante à de um console de jogos popular.

### Passos para a Construção:
1. **Criação da Caixa**: Utilize o site [OnShape](https://www.onshape.com/) para projetar o nosso protótipo em 3D. Este site oferece uma interface intuitiva que permite personalizar as dimensões e os detalhes da caixa de forma simples e eficaz. 🛠️📐
  
2. **Design Inspirado no Nintendo Switch**: A caixa será projetada com um visual que remete ao Nintendo Switch, incorporando linhas suaves e um espaço adequado para acomodar os LEDs, botões e o display LCD. Esse design não apenas tornará o protótipo visualmente atraente, mas também facilitará a interação do usuário com os controles, melhorando a experiência geral de jogo. 🎨✨

3. **Impressão em 3D**: Após finalizar o design, a caixa será impressa em 3D com um material durável que suportará o uso frequente. A impressão 3D permitirá a criação de formas e detalhes complexos, algo que seria difícil de alcançar com métodos de fabricação tradicionais. 🖨️📦

Ao integrar esse design, buscamos não apenas funcionalidade, mas também uma estética envolvente, criando uma experiência de jogo moderna que remete ao universo dos consoles. 🎉

## 🫂 Integrantes

Aqui estão os membros do grupo que participaram durante desenvolvimento desta Checkpoint.

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
