// import library servo
#include <Servo.h>

// import library sensor ultrasonic
#include <NewPing.h>

// pin motor driver
#define ENA 11
#define IN1 8
#define IN2 7
#define IN3 5
#define IN4 4
#define ENB 6

// pin ultrasonic
#define PIN_TRIGGER 2
#define PIN_ECHO 3

// membuat batas jarak maksimal
#define JARAK_MAKSIMAL 200 // Centimeter

// membuat batas kecepatan maksimal
#define MAX_SPEED 50
#define MAX_SPEED_OFFSET 20

// memanggil fungsi library NewPing dengan nama srf
NewPing srf(PIN_TRIGGER, PIN_ECHO, JARAK_MAKSIMAL);

// create servo object
Servo servo;

// variabel boolean untuk mengecek apakah robot sedang melaju atau tidak
boolean sedangMaju = false;

int jarak = 100;
int SPEED = 0;

// membuat variabel untuk menyimpan nilai posisi servo
int lihat_depan = 78;
int lihat_kiri = 165;
int lihat_kanan = 8;

void setup() {
  // sets motor driver pin as OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // sets the trigger pin as OUTPUT
  pinMode(PIN_TRIGGER, OUTPUT);

  // sets the echo pin as INPUT
  pinMode(PIN_ECHO, INPUT);

  // attaches the servo on pin 9 to the servo object
  servo.attach(9);

  // Mengatur posisi servo agar sensor melihat kearah depan
  servo.write(lihat_depan);

  // start serial communication with 9600 of baudrates speed
  Serial.begin(9600);
}

void loop() {
  // membuat variable lokal baru untuk menyimpan
  // hasil pengecekan jarak kanan dan jarak kiri
  int jarak_kanan = 0;
  int jarak_kiri = 0;
  delay(40);

  // jika jarak depan kurang dari 15 cm
  // berarti ada halangan di depan robot
  if (jarak <= 15) {
    // menghentikan laju robot dengan memanggil method berhenti
    berhenti();
    delay(100);

    // kemudian robot mundur
    mundur();

    // selama 300 milisecond
    delay(300);

    // kemudian robot berhenti lagi
    berhenti();

    // selama 200 milisecond
    delay(200);

    // kemudian robot mengecek sisi kanan
    jarak_kanan = cek_kanan();

    // selama 200 milisecond
    delay(200);

    // lalu robot mengecek sisi kiri
    jarak_kiri = cek_kiri();

    // selama 200 milisecond
    delay(200);

    // jika jarak kanan lebih jauh daripada jarak kiri
    if (jarak_kanan >= jarak_kiri) {

      // panggil method belok kanan
      belok_kanan();

      // kemudian berhenti
      berhenti();
    }

    // tapi jika jarak kiri lebih jauh daripada jarak kanan
    else {

      // panggil method belok kiri
      belok_kiri();

      // kemudian berhenti
      berhenti();
    }
  }

  // jika jarak depan lebih dari 15 cm
  else if (jarak > 15) {
    // panggil method maju
    maju();
  }

  // menyimpan nilai baca sensor kedalam variable jarak
  jarak = baca_sensor();
  Serial.println(jarak);
}

// membuat method untuk mengecek sisi kanan robot
int cek_kanan() {
  // robot melihat kearah kanan
  servo.write(lihat_kanan);
  delay(500);

  // menyimpan nilai baca sensor kedalam variable lokal
  int jarak = baca_sensor();
  delay(100);

  // robot melihat kearah depan
  servo.write(lihat_depan);

  // mengembalikan nilai jarak dari variable lokal ke variable global
  return jarak;
}

// membuat method untuk mengecek sisi kiri robot
int cek_kiri() {
  // robot melihat kearah kiri
  servo.write(lihat_kiri);
  delay(500);

  // menyimpan nilai baca sensor kedalam variable lokal
  int jarak = baca_sensor();
  delay(100);

  // robot melihat kearah depan
  servo.write(lihat_depan);

  // mengirim nilai jarak dari scope lokal ke global
  return jarak;
  delay(100);
}

// membuat method untuk membaca jarak
int baca_sensor() {
  // membuat variable lokal untuk menyimpan nilai dari fungsi library NewPing
  delay(70);
  int cm = srf.ping_cm();

  // jika nilai pada variable cm adalah 0
  if (cm == 0) {
    // mengganti nilai variable cm menjadi 250
    // agar robot dapat mengetahui bahwa jarak berada diluar batas deteksi
    cm = 250;
  }
  return cm;
}

void maju() {
  // jika status robot sedangMaju adalah false
  if (!sedangMaju) {

    // mengubah status robot sedangMaju menjadi true
    sedangMaju = true;

    // robot bergerak maju

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    // menambah kecepatan motor dari 0 hingga maksimal
    for (SPEED = 0; SPEED < MAX_SPEED; SPEED += 2) {
      analogWrite(ENA, SPEED);
      analogWrite(ENB, SPEED);
      delay(5);
    }
  }
}

void mundur() {
  // mengubah status sedangMaju menjadi false
  sedangMaju = false;

  // robot bergerak mundur
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  // menambah kecepatan motor dari 0 hingga maksimal
  for (SPEED = 0; SPEED < MAX_SPEED; SPEED += 2) {
    analogWrite(ENA, SPEED);
    analogWrite(ENB, SPEED);
    delay(5);
  }
}

void belok_kanan() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 130);
  analogWrite(ENB, 130);
  delay(250);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void belok_kiri() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 130);
  analogWrite(ENB, 130);
  delay(250);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void berhenti() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
