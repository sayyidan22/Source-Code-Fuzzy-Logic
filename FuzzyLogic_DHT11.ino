// Library yang dibutuhkan 
#include  "DHT.h"
#include  <FirebaseESP8266.h> 
#include  <ESP8266WiFi.h> 
#include  <WiFiClient.h> 

//  Mendefinisikan  pin  OUTPUT  dan  tipe  sensor  DHT #define led D0
#define DHTTYPE DHT11 #define  DHTPIN  D3
DHT  dht11(DHTPIN,  DHTTYPE);

//  Variabel  untuk  humidity  dan  tempertature float h, t;

// akses wifi client WiFiClient client;

//  key  database  pada  Firebase
#define  FIREBASE_HOST  "https://miotprak-ba710-default- rtdb.firebaseio.com/"
#define  FIREBASE_AUTH  "D2suzyrgSqGMQIsoijSd0oLLVBlIcf3tErbGBqCf"

//Nama Wifi
#define  WIFI_SSID  "kos pak dodo" 
#define  WIFI_PASSWORD "safirahkrikilan"

// mendeklarasikan objek data dari FirebaseESP8266 FirebaseData firebaseData;


void setup() { 
  Serial.begin(9600); 
  ThingSpeak.begin(client); 
  dht11.begin();
  pinMode  (led,  OUTPUT);

// Koneksi ke Wifi WiFi.begin(WIFI_SSID,  WIFI_PASSWORD);
Serial.print("connecting");
while  (WiFi.status()  !=  WL_CONNECTED)  { 
  delay(500);
}
Serial.println(); Serial.print("Connected  with  IP:  "); 
Serial.println(WiFi.localIP()); Serial.println();
Firebase.begin(FIREBASE_HOST,  FIREBASE_AUTH);
}


void loop(){
// Sensor DHT11 membaca suhu dan kelembaban float  t  =  dht11.readTemperature();
float  h  =  dht11.readHumidity();

//  pembacaan  kode  dari  database  untuk  switch  lampu  LED
if  (Firebase.getString(firebaseData,  "/Database/ledd"))  { if	(firebaseData.dataType()  ==  "string")
{
String  nilai  =  firebaseData.stringData(); if  (nilai  ==  "1")
{
Serial.println("led  ON"); digitalWrite(led,  HIGH);  }
else  if  (nilai  ==  "0")
{
Serial.println("led  OFF"); digitalWrite(led,
LOW);
}
else  {Serial.println("Salah  kode!  isi  dengan  data  0/1");}
}
}

// Memeriksa apakah sensor berhasil mambaca suhu dan kelembaban if  (isnan(t)  ||  isnan(h)  )  {
Serial.println("Gagal  membaca  sensor  DHT11"); delay(1000);
return;
}

// Menampilkan suhu dan kelembaban kepada serial monitor Serial.print("Suhu:  ");
Serial.print(t);
Serial.println("  *C");
Serial.print("Kelembaban:  "); Serial.print(h); Serial.println("  %"); Serial.println();

// logic fuzy
float  tV  =  (100-t); float  hV  =  (100-h); float  diV  =  100; float  v1  =  (tV/diV); float  v2  =(hV/diV); float  v3  =(v1  +  v2); float  v4  =  (v3  /  3);

//  Memberikan  status  suhu,  kelembaban  dan  value  logic  fuzzy kepada firebase
if  (Firebase.setFloat(firebaseData,  "/Database/suhu",  t)){ Serial.println("Suhu  terkirim");
} else{
Serial.println("Suhu tidak terkirim"); Serial.println("Karena: " + firebaseData.errorReason());
}
if  (Firebase.setFloat(firebaseData,  "/Database/kelembapan",  h)){ Serial.println("Kelembaban terkirim");
Serial.println();
} else{
Serial.println("Kelembaban tidak terkirim"); Serial.println("Karena:  "  +  firebaseData.errorReason());
}
if  (Firebase.setFloat(firebaseData,  "/Database/value",  v4)){ Serial.println("value fuzzy logic terkirim"); Serial.println();
} else{
Serial.println("value  fuzzy  logic  tidak  terkirim"); Serial.println("Karena: " + firebaseData.errorReason());
}
delay(1000);
}

