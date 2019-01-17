
#define D0 6    // HT9170 D0 to digital pin 6
#define D1 7    // HT9170 D1 to digital pin 7
#define D2 8    // HT9170 D2 to digital pin 8
#define D3 9    // HT9170 D3 to digital pin 9
#define DV 10   // HT9170 DV to digital pin 10

void setup()
{
  pinMode(D0,INPUT);
  pinMode(D1,INPUT);
  pinMode(D2,INPUT);
  pinMode(D3,INPUT);
  pinMode(DV,INPUT);
  Serial.begin(9600);
}

void loop()
{
  switch(digitalRead(DV))
  {
    case 1:
    {
      Serial.write("DATA VALID\n\r");
      if(
          ( digitalRead(D0) == 0 )  &&  
          ( digitalRead(D1) == 1 )  && 
          ( digitalRead(D2) == 0 )  && 
          ( digitalRead(D3) == 1 ) 
        )
        {
          Serial.write("0\n\r");
        }
      else if(
                ( digitalRead(D0) == 1 )  &&  
                ( digitalRead(D1) == 0 )  && 
                ( digitalRead(D2) == 0 )  && 
                ( digitalRead(D3) == 0 ) 
              )
              {
                Serial.write("1\n\r");
              }
      else if(
                ( digitalRead(D0) == 0 )  &&  
                ( digitalRead(D1) == 1 )  && 
                ( digitalRead(D2) == 0 )  && 
                ( digitalRead(D3) == 0 ) 
              )
              {
                Serial.write("2\n\r");
              }
      else if(
                ( digitalRead(D0) == 1 )  &&  
                ( digitalRead(D1) == 1 )  && 
                ( digitalRead(D2) == 0 )  && 
                ( digitalRead(D3) == 0 ) 
              )
              {
                Serial.write("3\n\r");
              }
      else if(
                ( digitalRead(D0) == 0 )  &&  
                ( digitalRead(D1) == 0 )  && 
                ( digitalRead(D2) == 1 )  && 
                ( digitalRead(D3) == 0 ) 
              )
              {
                Serial.write("4\n\r");
              }
      else if(
                ( digitalRead(D0) == 1 )  &&  
                ( digitalRead(D1) == 0 )  && 
                ( digitalRead(D2) == 1 )  && 
                ( digitalRead(D3) == 0 ) 
              )
              {
                Serial.write("5\n\r");
              }
      else if(
                ( digitalRead(D0) == 0 )  &&  
                ( digitalRead(D1) == 1 )  && 
                ( digitalRead(D2) == 1 )  && 
                ( digitalRead(D3) == 0 ) 
              )
              {
                Serial.write("6\n\r");
              }
      else if(
                ( digitalRead(D0) == 1 )  &&  
                ( digitalRead(D1) == 1 )  && 
                ( digitalRead(D2) == 1 )  && 
                ( digitalRead(D3) == 0 ) 
              )
              {
                Serial.write("7\n\r");
              }
      else if(
                ( digitalRead(D0) == 0 )  &&  
                ( digitalRead(D1) == 0 )  && 
                ( digitalRead(D2) == 0 )  && 
                ( digitalRead(D3) == 1 ) 
              )
              {
                Serial.write("8\n\r");
              }     
      else if(
                ( digitalRead(D0) == 1 )  &&  
                ( digitalRead(D1) == 0 )  && 
                ( digitalRead(D2) == 0 )  && 
                ( digitalRead(D3) == 1 ) 
              )
              {
                Serial.write("9\n\r");
              }
      else if(
                ( digitalRead(D0) == 0 )  &&  
                ( digitalRead(D1) == 0 )  && 
                ( digitalRead(D2) == 0 )  && 
                ( digitalRead(D3) == 0 ) 
              )
              {
                Serial.write("D\n\r");
              }
      else if(
                ( digitalRead(D0) == 1 )  &&  
                ( digitalRead(D1) == 1 )  && 
                ( digitalRead(D2) == 0 )  && 
                ( digitalRead(D3) == 1 ) 
              )
              {
                Serial.write("*\n\r");
              }
      else if(
                ( digitalRead(D0) == 0 )  &&  
                ( digitalRead(D1) == 0 )  && 
                ( digitalRead(D2) == 1 )  && 
                ( digitalRead(D3) == 1 ) 
              )
              {
                Serial.write("#\n\r");
              }
      else if(
                ( digitalRead(D0) == 1 )  &&  
                ( digitalRead(D1) == 0 )  && 
                ( digitalRead(D2) == 1 )  && 
                ( digitalRead(D3) == 1 ) 
              )
              {
                Serial.write("A\n\r");
              }
      else if(
                ( digitalRead(D0) == 0 )  &&  
                ( digitalRead(D1) == 1 )  && 
                ( digitalRead(D2) == 1 )  && 
                ( digitalRead(D3) == 1 ) 
              )
              {
                Serial.write("B\n\r");
              }
      else if(
                ( digitalRead(D0) == 1 )  &&  
                ( digitalRead(D1) == 1 )  && 
                ( digitalRead(D2) == 1 )  && 
                ( digitalRead(D3) == 1 ) 
              )
              {
                Serial.write("C\n\r");
              }
       delay(1000);
    }
    break;

    case 0: break;
  }
}

