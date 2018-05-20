int beepBekleme = 10, beepSabiti = 10, maxUzaklik = 70,volCount = 0, sesDurumu = 0, uzaklikDurumu = 0;
int sesButton = 3;
int uzaklikButton = 4;
int trigPin = 6; /* Sensorun trig pini Arduinonun 6 numaralı ayağına bağlandı */
int echoPin = 7;  /* Sensorun echo pini Arduinonun 7 numaralı ayağına bağlandı */

int motorGndPin = 10 ;
int motorPin = 9;
int buzzerPin = 11;
unsigned int mesafe;
boolean sesCiksinmi = true, titresimCiksinmi = true,uzunlukMode = true;
int time;

void setup(){
  pinMode(buzzerPin, OUTPUT);
  pinMode(motorGndPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(sesButton, INPUT);
  pinMode(uzaklikButton, INPUT);
  pinMode(trigPin, OUTPUT); /* trig pini çıkış olarak ayarlandı */
  pinMode(echoPin,INPUT); /* echo pini giriş olarak ayarlandı */

  guncelle();
  beep();
  delay(200);
  time = millis();
  Serial.begin(9600);
}
 
void loop(){
  guncelle();
  if(titresimCiksinmi){
    titresim();
  }
  else{
    titresimKapali();
  }
  if(sesCiksinmi){
    beep();
  }
  bekle();
}
void guncelle(){
  buttonKontrol();
  ping();
  if(mesafe > maxUzaklik){
  /*
  ping() fonsiyonu ile mesafe ölçülmüştü. Şimdi, eğer ölçülen uzaklık daha önceden belirttiğimiz maxUzaklıktan büyükse uzaklığa maxUzaklik değişkeni atanıor.Yani uzun lafın kısası, ÖRNEĞİN: Eğer biz 50 cm'den büyük uzaklığı ölçmek istemiyoruz diyelim. ve ölçülen uzaklık 70 cm olsun bu durumda biz ölçülen uzaklığı 70 değilde 50 cm olarak alacaz.  
  */
    mesafe = maxUzaklik;
  }
  beepBekleme = mesafe * (1.0 * beepSabiti);
  //beep olarak ses çıkarma süresini ölçülen uzaklıkla orantılı olarak değişiyor.
}
void buttonKontrol(){
  if(digitalRead(sesButton) == HIGH || digitalRead(uzaklikButton) == HIGH){
    //İki butondan minimum birine basılmışsa
    if(digitalRead(sesButton) == HIGH && digitalRead(uzaklikButton) == HIGH){
    // Eğer İki butonada basılmışsa
      uzaklikDegisimi();
      sesDegisimi();
    }
    else if (digitalRead(sesButton) == HIGH){
    //Ses buonuna basılmışsa
      sesDegisimi();
    }
    else{
    //Uzaklık butonunna basılmışsa
      uzaklikDegisimi();
    }
  }
}
 
void ping(){
  //Mesafe sensöründen sinyal gönderilip alınıyor
  
  digitalWrite(trigPin, LOW);//Eğer daha önceden sinyal varsa o sinyal kesiliyor
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);//Yeni bir sinyal gönderiliyor
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);//gönderilen sinyal kesiliyor
  pinMode(echoPin, INPUT);
  mesafe = pulseIn(echoPin, HIGH) / 37;//Dışardan tekrar sensöre gelen sinyalin süresi ölçülüyor ve Ölçülen süre inç cinsinden veriye çevriliyor
  /*
  Burda tek pinli mesafe sensörü kullanılmış.
  pin önce OUTPUT olarak ayarllanıp sinyal göndeiliyor.
  Daha sonra pin INPUT yapılıp gelen sinyal ölçülüyor.
  Muhtemelen siz 4 pinli olan Mesafe sensürünü kullanacaksınız. Orada trig pinine ve Echo pinine dikkat edin.
  Küçük bi mantık farklılığı var. Eğer anlayamazsanın ben size yine yardımcı olmaya çalışırım.
  */
}
void beep(){
   analogWrite(buzzerPin, 150);
   delay(50);
   analogWrite(buzzerPin, 0);
}
 
void titresim(){
  
    analogWrite(motorGndPin, 0);
    analogWrite(motorPin, 250- mesafe * 175.0 / maxUzaklik);
}
 
void titresimKapali(){
  analogWrite(motorGndPin, 0);
  analogWrite(motorPin, 0);
}


void sesDegisimi(){
  /*
  Bu fonksiyonun ilk çağrılmasında titreşim var ses yok şeklinde ayarlanıyor.
  ikinci çağrılmada ses var titreşim yok şeklinde ayarlanıyor.
  Üçüncü çağrılmada ise titreşim ve ses var şeklinde ayarlanıyor.
  Ve bu bu şekilde devam ediyor. yani 4. çağrılmada yine başa sarıp devam edecek.
  */
  if(volCount == 0){
    titresimCiksinmi = true;
    sesCiksinmi = false;
  }
  else if(volCount == 1){
    titresimCiksinmi = false;
    sesCiksinmi = true;
  }
  else{
    titresimCiksinmi = true;
    sesCiksinmi = true;
  } 
  volCount++;
  if(volCount > 2){
    volCount = 0;
  }
}
 void uzaklikDegisimi(){
   /*
   Bu fonksiyn ilk çağrıldığında  kısa ses çıkması ve kısa mesafeyi ölçmek için ayarlanıyor.
   İkinci çağrılmada ise uzun beep sesi ve uzun mesafeyi ölçmek için ayarlanıyor.
   */
  if(uzunlukMode){
    uzunlukMode = false;
    maxUzaklik = 30;
    beepSabiti = 25;
  }
  else{
    uzunlukMode = true;
    maxUzaklik = 70;
    beepSabiti = 10;
  }
}
 

 void bekle(){
   /*
   Sanırsam sistemin çalışma süresini bulup bi önceki çalışma süresini çıkartıyor,
   ve bu deger beepBekleme süresinden daha kısa ise 10ms zaman geçirtip tekrar butonları kontrol ediyor.
   */
  while((millis() - time) < beepBekleme){
    delay(10);
    buttonKontrol();
  }
  time = millis();
}
