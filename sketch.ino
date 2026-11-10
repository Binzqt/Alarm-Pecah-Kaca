// Alarm pecah kaca simulasi - Arduino Uno
// Sensor suara: suara keras -> LED kedip + buzzer bunyi
// Tombol reset: matikan alarm

const int SOUND_PIN = 2;   // tombol kuning "SUARA" (pengganti sensor suara)
const int RESET_BTN = 3;   // tombol ke GND
const int LED = 7;
const int BUZZER = 8;

// Tombol ditekan = LOW (pakai INPUT_PULLUP)
const int SOUND_ACTIVE = LOW;

const unsigned long ALARM_MS = 10000;    // alarm mati sendiri setelah 10 detik
const unsigned long COOLDOWN_MS = 2000;  // jeda sebelum bisa terpicu lagi

bool alarmOn = false;
unsigned long alarmStart = 0;
unsigned long alarmStopped = 0;

void startAlarm() {
  alarmOn = true;
  alarmStart = millis();
  Serial.println("ALARM! Suara keras terdeteksi (mungkin kaca pecah)");
}

void stopAlarm(const char* alasan) {
  alarmOn = false;
  alarmStopped = millis();
  digitalWrite(LED, LOW);
  noTone(BUZZER);
  Serial.print("Alarm mati: ");
  Serial.println(alasan);
}

void setup() {
  pinMode(SOUND_PIN, INPUT_PULLUP);
  pinMode(RESET_BTN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  Serial.println("Alarm siap. Menjaga ruangan...");
}

void loop() {
  // Deteksi suara
  if (!alarmOn &&
      millis() - alarmStopped > COOLDOWN_MS &&
      digitalRead(SOUND_PIN) == SOUND_ACTIVE) {
    startAlarm();
  }

  if (alarmOn) {
    // Kedip dan bunyi tiap 200 ms
    bool on = (millis() / 200) % 2 == 0;
    digitalWrite(LED, on);
    if (on) tone(BUZZER, 1000);
    else noTone(BUZZER);

    // Matikan alarm
    if (digitalRead(RESET_BTN) == LOW) {
      stopAlarm("tombol reset");
    } else if (millis() - alarmStart > ALARM_MS) {
      stopAlarm("waktu habis");
    }
  }
}
