const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  //analogWrite(16,200);
  Serial.begin(115200);
  
  ledcAttachPin(18, ledChannel);
  ledcSetup(ledChannel, freq, resolution);
}


void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<255;i+=10)
  {
    ledcWrite(0,i);
    Serial.println(i);
    delay(100);
  }
  for(int i=255;i>=0;i-=10)
  {
    ledcWrite(0,i);
    Serial.println(i);
    delay(100);
  }
}
