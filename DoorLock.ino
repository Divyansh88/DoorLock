#include<SoftwareSerial.h>

SoftwareSerial Ser(3,4);

int row[]={13,12,11,10};// Defining row pins of keypad connected to Aeduino pins
int col[]={9,8,7,6};//Defining column pins of keypad connected to Arduino
int i,j,k=0; // Two counter variables to count inside for loop
int col_scan; // Variable to hold value of scanned columns
char tag1pin[5]={'A','1','2','3','4'};
char tag2pin[5]={'B','5','6','7','8'};
char arr[5];

int SPEAKER = 5;     

int data1 = 0;
int ok = -1;
int ok11 = 0;
int ok21 = 0;
int ok12 = 0;
int ok22 = 0;
int yes = 13;
int no = 12;
int count=0;

int tag1[12] = {49,52,48,48,55,48,69,48,68,52,53,48};
int tag2[12] = {49,52,48,48,55,50,52,51,51,51,49,54};
int newtag[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; // used for read comparisons

void setup()
{
  pinMode(SPEAKER, OUTPUT);
  Ser.begin(9600);
  Serial.begin(9600);  // start serial to PC 
  pinMode(2, OUTPUT); // for status LEDs
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  for(i=0;i<=3;i++)
  {
    pinMode(row[i],OUTPUT);
    pinMode(col[i],INPUT);
    digitalWrite(col[i],HIGH);
  } 
}

boolean comparetag(int aa[12], int bb[12])
{
  boolean ff = false;
  int fg = 0;
  for (int cc = 0 ; cc < 12 ; cc++)
  {
    if (aa[cc] == bb[cc])
    {
      fg++;
    }
  }
  if (fg == 12)
  {
    ff = true;
  }
  return ff;
}

void checkmytags() // compares each tag against the tag just read
{
  ok = 0; // this variable helps decision-making,
  // if it is 1 we have a match, zero is a read but no match,
  // -1 is no read attempt made
  if (comparetag(newtag, tag1) == true)
  {
    ok++;
    ok11++;
  }
  else if (comparetag(newtag, tag2) == true)
  {
    ok++;
    ok12++;
  }
}

void readTags()
{
  ok = -1;

  if (Serial.available() > 0) 
  {
    // read tag numbers
    delay(100); // needed to allow time for the data to come in from the serial buffer.

    for (int z = 0 ; z < 12; z++) // read the rest of the tag
    {
      data1 = Serial.read();
      newtag[z] = data1;
    }
    Serial.flush(); // stops multiple reads

    // do the tags match up?
    checkmytags();
  }

  // now do something based on tag type
  if (ok > 0) // if we had a match
  {
    Serial.println("Card Accepted");
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    
    ok = -1;
    readkeypad();
    //exit();
  }
  else if (ok == 0) // if we didn't have a match
  {
    Serial.println("Card Rejected");
    digitalWrite(3, HIGH);
    delay(1000);
    digitalWrite(3, LOW);
    valid();
    ok = -1;
  }
}
char keypress(int i, int j)
{
           
if(i==0&&j==0)
{
Serial.println("1");
return '1';
}
if(i==0&&j==1)
{
Serial.println("2");
return '2';
}
if(i==0&&j==2)
{
Serial.println("3");
return '3';
}
if(i==0&&j==3)
{
Serial.println("A");
return 'A';
}
if(i==1&&j==0)
{
Serial.println("4");
return '4';
}
if(i==1&&j==1)
{
Serial.println("5");
return '5';
}
if(i==1&&j==2)
{
Serial.println("6");
return '6';
}
if(i==1&&j==3)
{
Serial.println("B");
return 'B';
}
if(i==2&&j==0)
{
Serial.println("7");
return '7';
}
if(i==2&&j==1)
{
Serial.println("8");
return '8';
}
if(i==2&&j==2)
{
Serial.println("9");
return '9';
}
if(i==2&&j==3)
{
Serial.println("C");
return 'C';
}
if(i==3&&j==0)
{
Serial.println("*");
return '*';
}
if(i==3&&j==1)
{
Serial.println("0");
return '0';
}
if(i==3&&j==2)
{
Serial.println("#");
return '#';
}
if(i==3&&j==3)
{
Serial.println("D");
return 'D';
}
}


void readkeypad()
{
  while(k<5)
  {
  for(i=0; i<=3; i++)
  {
    digitalWrite(row[0],HIGH);
    digitalWrite(row[1],HIGH);
    digitalWrite(row[2],HIGH);
    digitalWrite(row[3],HIGH);
    digitalWrite(row[i],LOW);
    for(j=0; j<=3; j++)
     {
        col_scan=digitalRead(col[j]);
          if(col_scan==LOW)
          {
            arr[k]=keypress(i,j);
            delay(300);
            k++;
          }
        
      }
  }
  }
    if(k==5)
            {
              k=0;
              checkmykey();
             }
  
}


boolean comparekey(char aa[5], char bb[5])
{
  boolean ff = false;
  int fg = 0;
  for (int cc = 0 ; cc < 5 ; cc++)
  {
    if (aa[cc] == bb[cc])
    {
      fg++;
    }
  }
  if (fg == 5)
  {
    ff = true;
  }
  return ff;
}

void checkmykey()
{
  ok = 0; // this variable helps decision-making,
  // if it is 1 we have a match, zero is a read but no match,
  // -1 is no read attempt made
  if (comparekey(arr, tag1pin) == true)
  {
    ok++;
    ok21++;
  }
  if (comparekey(arr, tag2pin) == true)
  {
    ok++;
    ok22++;
  }
  
 if (ok > 0) // if we had a match
  {
    Serial.println("Key Accepted");
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    valid();
    ok = -1;
  }
  else if(ok==0) // if we didn't have a match
  {
    Serial.println("Key Rejected");
    digitalWrite(3, HIGH);
    delay(1000);
    digitalWrite(3, LOW);
    Serial.println("You can't enter.");
    buzzercall();
    smssend();
    ok = -1;
  }
}

void valid()
{
  if((ok11>0)&&(ok21>0))
  {
    
    Serial.println("You can enter.");
    digitalWrite(2,HIGH);
    delay(1000);
    digitalWrite(2, LOW);
  }
  if((ok12>0)&&(ok22>0))
  {
    
    Serial.println("You can enter.");
    digitalWrite(2,HIGH);
    delay(1000);
    digitalWrite(2, LOW);
  }
  else if((ok11<=0)&&(ok12<=0)&&(ok21<=0)&&(ok22<=0))
  {
    Serial.println("You can't enter.");
    digitalWrite(3,HIGH);
    delay(1000);
    digitalWrite(3, LOW);
    buzzercall();
    smssend();
  } 
}

void buzzercall()
{
  digitalWrite(5,HIGH);
  delay(10000);
  digitalWrite(5,LOW);
  delay(1000);
}

void smssend()
{
  Ser.print("\r"); 
  delay(1000);
  Ser.print("AT+CMGF=1\r"); 
  delay(1000);
  Ser.print("AT+CMGS=\"999999999\"\r"); //add your phone number here
  delay(1000);
  Ser.print("You have unauthorized door access.");
  delay(1000);
  Ser.write(0x1A);
  delay(1000);
}

void loop()
{
  readTags();
}
