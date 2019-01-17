#define DTMF_OFF  31
#define STAR  11
#define POUND 12
#define A_KEY 13
#define B_KEY 14
#define C_KEY 15
#define D_KEY 0

#define CLOCK_PIN  2                    // Clock (serial)
#define DATA_PIN   3                    // Data (serial)
#define CE_PIN     4                    // Chip Enable pin (must control)

char key;

void setup()
{
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(CE_PIN, OUTPUT);
  Init_HT9200();                        // init the chip
  Serial.begin(9600);
}    

void loop()
{
  key = Serial.read();
  if(key != -1)  // Check for a valid key.
  {
    Serial.write((char)key);
    switch (key)
    {
      case '*':
        key=STAR;
        Dialer();
        break;

      case '#':
        key=POUND;
        Dialer();
        break;

      case 'A':
        key=A_KEY;
        Dialer();
        break;

      case 'B':
        key=B_KEY;
        Dialer();
        break;

      case 'C':
        key=C_KEY;
        Dialer();
        break;

      case 'D':
        key=D_KEY;
        Dialer();
        break;

      case '1':
        key = 1;
        Dialer();
        break;

      case '2':
        key = 2;
        Dialer();
        break;

      case '3':
        key = 3;
        Dialer();      
        break;

      case '4':
        key = 4;
        Dialer();
        break;

      case '5':
        key = 5;
        Dialer();
        break;

      case '6':
        key = 6;
        Dialer();
        break;

      case '7':
        key = 7;
        Dialer();
        break;

      case '8':
        key = 8;
        Dialer();
        break;

      case '9':
        key = 9;
        Dialer();
        break;

      case '0':
        key = 0;
        Dialer();
        break;

      default: break;
    }
    delay(1000);
  }
  else
  {
    DTMF_Out(DTMF_OFF,1,0);
  }
}



void Init_HT9200 (){
 digitalWrite(CE_PIN, HIGH);           // start with chip disabled (else you go nuts)
 digitalWrite(CLOCK_PIN, HIGH);        // start with clock pin high
 digitalWrite(CE_PIN, LOW);            // now enable the chip
 delay(10);                            // delay 10ms to ramp up the ocillator
 DTMF_Out (DTMF_OFF,1,0);              // turn off ant tone from previous run
}

void Dialer(){  // cycles through all tones
   DTMF_Out(key,500,0);  // 1/2 sec tome with 1/10 pause
}

void DTMF_Out (byte digit, long duration, long pause)
{  // FOR SERIAL COMM
 if (digit == 0) digit = 10;           // take care of 0 here
 for (byte i=0; i<5; i++)
 {
   digitalWrite(CLOCK_PIN, HIGH);      // clock high while setting data
   digitalWrite(DATA_PIN, bitRead(digit,i)); // set data LSB->MSB
   delayMicroseconds(5);               // 1/2 of 100K Clock speed
   digitalWrite(CLOCK_PIN, LOW);       // clock low to latch data
   delayMicroseconds(5);               // 1/2 of 100K Clock speed
 }
 delay(duration);                      // how long tone will play
 if(pause != 0)
 {                      // tone sounds continuously if zero
   for(byte i=0; i<5; i++)
   {
     digitalWrite(CLOCK_PIN, HIGH);
     digitalWrite(DATA_PIN, bitRead(DTMF_OFF,i));
     delayMicroseconds(5);
     digitalWrite(CLOCK_PIN, LOW);
     delayMicroseconds(5);
   }
   delay(pause);                       // how long pause between tones
 }
 digitalWrite(CLOCK_PIN, HIGH);
}
