// Radar de proximidade para feira de profissões
// Arduino Uno + HC-SR04 + buzzer piezo
// Ligacoes:
// HC-SR04 VCC  -> 5V
// HC-SR04 TRIG -> pino 9
// HC-SR04 ECHO -> pino 10
// HC-SR04 GND  -> GND
// Buzzer +     -> pino 8
// Buzzer -     -> GND

const byte PINO_TRIG = 9;
const byte PINO_ECHO = 10;
const byte PINO_BUZZER = 8;

long medirDistanciaCm() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  unsigned long duracao = pulseIn(PINO_ECHO, HIGH, 30000);

  if (duracao == 0) {
    return 400;
  }

  long distancia = duracao / 58;

  if (distancia > 400) {
    distancia = 400;
  }

  return distancia;
}

void tocarAlerta(long distancia) {
  if (distancia <= 10) {
    // Muito proximo: alarme agudo quase continuo
    tone(PINO_BUZZER, 2200);
    delay(100);
  }
  else if (distancia <= 20) {
    // Proximo: bipes medios
    tone(PINO_BUZZER, 1400, 100);
    delay(200);
  }
  else if (distancia <= 30) {
    // Alguem se aproximando: bipes graves e espacados
    tone(PINO_BUZZER, 700, 80);
    delay(300);
  }
  else {
    // Area segura: sem som
    noTone(PINO_BUZZER);
    delay(100);
  }
}

void setup() {
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  long distancia = medirDistanciaCm();

  // A pagina de radar recebe somente este numero, uma linha por leitura.
  Serial.println(distancia);

  tocarAlerta(distancia);
}
