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

  for (int i = 0; i < numLeds; i++) {
    pinMode(pinosLeds[i], OUTPUT);
    digitalWrite(pinosLeds[i], LOW);
  }
  for (int i = 0; i < numBotoes + 1; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(0));
}

void loop() {
  if (digitalRead(botaoReset) == LOW) {
    delay(1000);
    comprimentoSequencia = 0;
    comecarNovoRound();
  }

  for (int i = 0; i < numBotoes; i++) {
    if (digitalRead(pinosBotoes[i]) == LOW) {
      delay(20); // Debounce
      if (digitalRead(pinosBotoes[i]) == LOW) {
        digitalWrite(pinosLeds[i], HIGH);
        delay(300);
        digitalWrite(pinosLeds[i], LOW);

        if (i == sequencia[indexImput]) {
          indexImput++;
          if (indexImput == comprimentoSequencia) {
            delay(1000);
            comecarNovoRound();
          }
        } else {
          fimDeJogo();
        }
      }
    }
  }
}

void comecarNovoRound() {
  indexImput = 0;
  comprimentoSequencia++;

  if (comprimentoSequencia > 20) {
    vitoria();
  } else {
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

void fimDeJogo() {
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
  comprimentoSequencia = 0;
  delay(1000);
}

void vitoria() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(pinosLeds[j], HIGH);
      delay(50);
      digitalWrite(pinosLeds[j], LOW);
    }
  }
  comprimentoSequencia = 0;
  delay(1000);
}
