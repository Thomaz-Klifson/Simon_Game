#include <Arduino.h>

// Pinos dos LEDs e botões
const int pinosLeds[] = {2, 4, 12, 13};
const int pinosBotoes[] = {14, 15, 16, 17, 18}; // O último botão é o botão de reset
const int botaoReset = 18;
const int numLeds = sizeof(pinosLeds) / sizeof(pinosLeds[0]);
const int numBotoes = sizeof(pinosBotoes) / sizeof(pinosBotoes[0]) - 1;

int sequencia[20];
int comprimentoSequencia = 0;
int indexImput = 0;

void setup() {
  Serial.begin(115200);

  // Configurar LEDs como saída e desligá-los
  for (int i = 0; i < numLeds; i++) {
    pinMode(pinosLeds[i], OUTPUT);
    digitalWrite(pinosLeds[i], LOW);
  }
  // Configurar botões como entrada com resistor interno pull-up
  for (int i = 0; i < numBotoes + 1; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }

  // Inicializar a semente aleatória com uma leitura analógica
  randomSeed(analogRead(0));
}

void loop() {
  // Verificar se o botão de reset foi pressionado
  if (digitalRead(botaoReset) == LOW) {
    delay(1000);
    comprimentoSequencia = 0;
    comecarNovoRound();
  }

  // Verificar se algum botão foi pressionado
  for (int i = 0; i < numBotoes; i++) {
    if (digitalRead(pinosBotoes[i]) == LOW) {
      delay(20); // Debounce
      if (digitalRead(pinosBotoes[i]) == LOW) {
        // Acender LED correspondente ao botão pressionado
        digitalWrite(pinosLeds[i], HIGH);
        delay(300);
        digitalWrite(pinosLeds[i], LOW);

        // Verificar se o botão pressionado corresponde à sequência atual
        if (i == sequencia[indexImput]) {
          indexImput++;
          // Verificar se a sequência foi completada
          if (indexImput == comprimentoSequencia) {
            delay(1000);
            comecarNovoRound();
          }
        } else {
          // Sequência incorreta, fim de jogo
          fimDeJogo();
        }
      }
    }
  }
}

// Iniciar um novo round
void comecarNovoRound() {
  indexImput = 0;
  comprimentoSequencia++;

  // Verificar se o jogador venceu
  if (comprimentoSequencia > 20) {
    vitoria();
  } else {
    // Mostrar a sequência atual
    for (int i = 0; i < comprimentoSequencia; i++) {
      if (i == comprimentoSequencia - 1) {
        sequencia[i] = random(0, numBotoes);
      }
      digitalWrite(pinosLeds[sequencia[i]], HIGH);
      delay(300);
      digitalWrite(pinosLeds[sequencia[i]], LOW);
      delay(300);
    }
  }
}

// Fim de jogo, mostrar animação de derrota e reiniciar o jogo
void fimDeJogo() {
  // Piscar todos os LEDs três vezes
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(pinosLeds[j], HIGH);
    }
    delay(250);
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(pinosLeds[j], LOW);
    }
    delay(250);
  }
  // Reiniciar o jogo
  comprimentoSequencia = 0;
  delay(1000);
}

// Função para mostrar animação de vitória e reiniciar o jogo
void vitoria() {
  // Piscar todos os LEDs 10 vezes rapidamente
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(pinosLeds[j], HIGH);
      delay(50);
      digitalWrite(pinosLeds[j], LOW);
    }
  }
  // Reiniciar o jogo
  comprimentoSequencia = 0;
  delay(1000);
}

