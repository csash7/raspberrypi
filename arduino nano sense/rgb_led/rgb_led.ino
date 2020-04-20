const int ledPin = 22;
const int ledPin2 = 23;
const int ledPin3 = 24;

void setup() {

pinMode(22, OUTPUT);
pinMode(23, OUTPUT);
pinMode(24, OUTPUT);
}

void loop() {
     digitalWrite(ledPin, LOW);
     digitalWrite(ledPin2, LOW);
     digitalWrite(ledPin3, LOW);
  delay(1000);          
     digitalWrite(ledPin, LOW);
     digitalWrite(ledPin2, LOW);
     digitalWrite(ledPin3, HIGH);
  delay(1000);    
     digitalWrite(ledPin, LOW);
     digitalWrite(ledPin2, HIGH);
     digitalWrite(ledPin3, HIGH);
  delay(1000);  
       digitalWrite(ledPin, HIGH);
       digitalWrite(ledPin2, LOW);
       digitalWrite(ledPin3, LOW);
  delay(1000);  
       digitalWrite(ledPin, HIGH);
       digitalWrite(ledPin2, LOW);
       digitalWrite(ledPin3, HIGH);
  delay(1000);  
       digitalWrite(ledPin, HIGH);
       digitalWrite(ledPin2, HIGH);
       digitalWrite(ledPin3, LOW);
  delay(1000);  

         digitalWrite(ledPin, HIGH);
       digitalWrite(ledPin2, HIGH);
       digitalWrite(ledPin3, HIGH);
  delay(1000);  
}
