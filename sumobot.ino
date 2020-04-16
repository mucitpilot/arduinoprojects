//SUMOBOT Nisan 2020 Adana
//değişkenleri tanımlıyorum

#define IRsol 2
#define IRsag 3
#define IRarka 4

#define MotorR1 6
#define MotorR2 7
#define MotorRE 5

#define MotorL1 8
#define MotorL2 9
#define MotorLE 10

#define Echoorta 11
#define Trigorta 12

#define Echosag A0
#define Trigsag A1

#include <NewPing.h>

NewPing sonar_on(12,11,80); // NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
NewPing sonar_sag(15,14,80);


 int distance_on;
 int distance_sag;
 int IR_sol;
 int IR_sag;
 int IR_arka;
 int deger;


//setup kodu bir defa çalışacak
void setup() {

  pinMode(IRsol, INPUT); // 3 adet çizgi tespit sensörü
  pinMode(IRsag, INPUT);
  pinMode(IRarka, INPUT);
  
  pinMode(Echoorta, INPUT); // önde ve sağda olmak üzere 2 adet ultrasound sensör
  pinMode(Echosag, INPUT);
  pinMode(Trigorta, OUTPUT);
  pinMode(Trigsag, OUTPUT);
  
  pinMode(MotorR1, OUTPUT); //motor çıkış pinleri
  pinMode(MotorR2, OUTPUT);
  pinMode(MotorL1, OUTPUT);
  pinMode(MotorL2, OUTPUT);
  
  Serial.begin(9600);
 
}

//ana gövde devamlı çalışıyor
void loop() {

 IR_sol = digitalRead(IRsol);
 IR_sag = digitalRead(IRsag);
 IR_arka = digitalRead(IRarka);

 while (IR_sol == 0 && IR_sag == 0 && IR_arka == 0) {  //rakip tespit etmeyi bu while içinde yapıcam. eğer IR sensörlerden birinde çizgi sinyali almışsa zaten öncelik kendini kurtarmak olacak
        // ve bu while içine girmeyecek
   //Serial.println("çizgiye basmıyorum ringin içindeyim cisim aramaya başla"); 
    //SonarSensor(Trigorta, Echoorta); //ses sensörleri için tanımlanan fonksiyon
    //distance_on=distance; //ön sensörden ölçülen mesafe
    distance_on=sonar_on.ping_cm(); //ön sensörden ölçüm alıyoruz
    distance_sag=sonar_sag.ping_cm(); 
    

   // Serial.print("ön mesafe :");
   // Serial.print(distance_on);
   // Serial.print("sağ mesafe :");
  //  Serial.println(distance_sag);
     
   while (distance_on < 30 && distance_on > 0) {
         IR_sol = digitalRead(IRsol);
        IR_sag = digitalRead(IRsag);
        IR_arka = digitalRead(IRarka);
          if (IR_sol == 1 || IR_sag == 1 || IR_arka == 1){
            break;
            
            }
         saldir();
         delay(100);//hedefe git
         //Serial.println("hedefe git");
          distance_on=sonar_on.ping_cm();

    } // ön sensör cisim algılıyor. ileri gidecek

        
    while ((distance_sag < 30 && distance_sag !=0)&&(distance_on > 30 || distance_on == 0)){ // sadece sağdan bilgi alıyor sağa dönecek
         IR_sol = digitalRead(IRsol);
        IR_sag = digitalRead(IRsag);
        IR_arka = digitalRead(IRarka);
          if (IR_sol == 1 || IR_sag == 1 || IR_arka == 1){
            break;
            
            }
            geri();
            delay(200);
            yerindesag();
            delay(400);      
          //  Serial.println("sağa dön");
           distance_on=sonar_on.ping_cm(); //ön sensörden ölçüm alıyoruz
            distance_sag=sonar_sag.ping_cm(); 
    
        }
    while ((distance_sag > 30 || distance_sag ==0)&&(distance_on > 30 || distance_on == 0)){
          IR_sol = digitalRead(IRsol);
        IR_sag = digitalRead(IRsag);
        IR_arka = digitalRead(IRarka);
          if (IR_sol == 1 || IR_sag == 1 || IR_arka == 1){
            break;
            
            }
          ileri();
          delay(100);
         //delay(100);// sensörler hedef bulamadı hafif ileri gidip sola dönecek
          //yerindesol();
         //delay(100);
         // Serial.println("cisim bulunamadı ileri ve sola git");
           distance_on=sonar_on.ping_cm(); //ön sensörden ölçüm alıyoruz
          distance_sag=sonar_sag.ping_cm(); 
    
    }  
          
       
  IR_sol = digitalRead(IRsol);//whiledan çıkış için okutmalıyız
  IR_sag = digitalRead(IRsag);
  IR_arka = digitalRead(IRarka);
 }//while çıkış

  
  if ((IR_sag == 1 || IR_sol ==1)&& IR_arka==0){  //herhangi bir ön çizgi sensörü çizgi algılıyorsa geri gidecek
  // Serial.println("sadece herhangi bir ön çizgi tespit edildi geri gidiyorum"); 
      geri();
      delay(800);
      yerindesag();
      delay(400);

 }   
 else if (IR_arka == 1){
 // else if (IR_arka == 1 && (IR_sol ==0 && IR_sag==0)){ //sadece arka sensör gördü o zaman ileri gitmeli
   //Serial.println("sadece arka çizgi tespit edildi ileri gidiyorum");
   ileri();
   delay(800);

  }   

/*
    else if (IR_arka == 1 && (IR_sol ==1 || IR_sag==1)){
        //Serial.println("arka çizgi ve en az 1 adet ön çizgi tespit edildi ");
        if (IR_sol==1){
         // Serial.println("arka çizgi ve sol ön çizgi tespit edildi yerinde sağa dön");
          yerindesag();
          delay(500);
          
          }
        else if (IR_sag==1){
          //Serial.println("arka çizgi ve sag ön çizgi tespit edildi yerinde ssola dön");
          yerindesol();
           delay(500);
          }
   */


 // }

} //loop

void saldir() { // Robotun ileri yönde hareketi için fonksiyon tanımlıyoruz.

  digitalWrite(MotorR1, HIGH); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, LOW); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, 200); // Sağ motorun hızı 150

  digitalWrite(MotorL1, HIGH); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, LOW); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, 200); // Sol motorun hızı 150


}


void ileri() { // Robotun ileri yönde hareketi için fonksiyon tanımlıyoruz.

  digitalWrite(MotorR1, HIGH); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, LOW); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, 130); // Sağ motorun hızı 150

  digitalWrite(MotorL1, HIGH); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, LOW); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, 130); // Sol motorun hızı 150


}


void yerindesag() { // Robotun sağa dönme hareketi için fonksiyon tanımlıyoruz.

  digitalWrite(MotorR1, LOW); // Sağ motorun ileri hareketi pasif
  digitalWrite(MotorR2, HIGH); // Sağ motorun geri hareketi aktif
  analogWrite(MotorRE, 200); // Sağ motorun hızı 0 (Motor duruyor)

  digitalWrite(MotorL1, HIGH); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, LOW); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, 200); // Sol motorun hızı 150


}
void yerindesol() { // Robotun sola dönme hareketi için fonksiyon tanımlıyoruz.

  digitalWrite(MotorR1, HIGH); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, LOW); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, 200); // Sağ motorun hızı 150

  digitalWrite(MotorL1, LOW); // Sol motorun ileri hareketi pasif
  digitalWrite(MotorL2, HIGH); // Sol motorun geri hareketi aktif
  analogWrite(MotorLE, 200); // Sol motorun hızı 0 (Motor duruyor)

}


void dur() { // Robotun durma hareketi için fonksiyon tanımlıyoruz.

  digitalWrite(MotorR1, HIGH);
  digitalWrite(MotorR2, LOW);
  digitalWrite(MotorRE, LOW);

  digitalWrite(MotorL1, HIGH);
  digitalWrite(MotorL2, LOW);
  digitalWrite(MotorLE, LOW);

}
void geri() { // Robotun geri hareketi için fonksiyon tanımlıyoruz.

  digitalWrite(MotorR1, LOW); // Sağ motorun ileri hareketi pasif
  digitalWrite(MotorR2, HIGH); // Sağ motorun geri hareketi aktif
  analogWrite(MotorRE, 120); // Sağ motorun hızı 150

  digitalWrite(MotorL1, LOW); // Sol motorun ileri hareketi pasif
  digitalWrite(MotorL2, HIGH); // Sol motorun geri hareketi aktif
  analogWrite(MotorLE, 120); // Sol motorun hızı 150

}
void ilerisol() { // Robotun ileri yönde hareketi için fonksiyon tanımlıyoruz.

  digitalWrite(MotorR1, HIGH); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, LOW); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, 220); // Sağ motorun hızı 150

  digitalWrite(MotorL1, HIGH); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, LOW); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, LOW); // Sol motorun hızı 150


}

void ilerisag() { // Robotun ileri yönde hareketi için fonksiyon tanımlıyoruz.

  digitalWrite(MotorR1, HIGH); // Sağ motorun ileri hareketi aktif
  digitalWrite(MotorR2, LOW); // Sağ motorun geri hareketi pasif
  analogWrite(MotorRE, LOW); // Sağ motorun hızı 150

  digitalWrite(MotorL1, HIGH); // Sol motorun ileri hareketi aktif
  digitalWrite(MotorL2, LOW); // Sol motorun geri hareketi pasif
  analogWrite(MotorLE, 220); // Sol motorun hızı 150


}
