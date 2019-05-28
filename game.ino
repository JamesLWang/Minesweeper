// testcolors demo for Adafruit RGBmatrixPanel library.
// Renders 512 colors on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420
// Library supports 4096 colors, but there aren't that many pixels!  :)

// Written by Limor Fried/Ladyada & Phil Burgess/PacharYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.
#include <Arduino.h>
#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constrachars.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2


#include<stdio.h>
#include<stdlib.h>
#include<time.h>

using namespace std;



byte button1 = 13;
byte button2 = 11;



bool buttonState1 = false;
bool buttonState2 = false;

struct zone{
    bool isReveal;
    bool isFlag;
    byte number;

};

struct coordinate{
    char x;
    char y;
};


RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

zone boardreal[10][10];

zone numberBoard[8][8];

char rn_x;
char rn_y;
bool isChange;


void setup() {
  
  //matrix.dumpMatrix();
  //drawBoard(8,8);
  matrix.begin();
  rn_x=0;
  rn_y=0;
  isChange=true;
  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  for (char i = 0; i < 10; i++) {
      for (char j = 0; j < 10; j++) {

          boardreal[i][j].number = 0;
          boardreal[i][j].isReveal = false;
          boardreal[i][j].isFlag= false;  

      }
  }
  
  for (char i = 0; i < 6; i++) {

      //seed random number generator
      char x = rand()%(8)+1;
      //Serial.print(x);
      //cout << x << endl;
      char y = rand()%(8)+1;
      //Serial.println(y);
      //cout << y << endl;
      boardreal[x][y].number = 1;
  }
  
  for (char i = 1; i < 9; i++) {
      for (char j = 1; j < 9; j++) {

          
          
          if (boardreal[i][j].number == 1) {
              //cout<< i << endl;
              //cout<<j << endl;
              numberBoard[(i - 1)][(j - 1)].number = 10;
              numberBoard[(i - 1)][(j - 1)].isReveal=false;
              numberBoard[(i - 1)][(j - 1)].isFlag= false;


          } else {

              //check index values
              char a = boardreal[(i - 1)][(j + 1)].number;
              char b = boardreal[(i - 1)][j].number;
              char c = boardreal[(i - 1)][(j - 1)].number;
              char d = boardreal[(i + 1)][(j + 1)].number;
              char e = boardreal[(i + 1)][(j)].number;
              char f = boardreal[(i + 1)][(j - 1)].number;
              char g = boardreal[i][(j + 1)].number;
              char h = boardreal[i][(j - 1)].number;

              //if (i==1 && j==8){
              //    cout << a + b + c + d + e + f + g + h << endl;
              //}
              numberBoard[(i - 1)][(j - 1)].number = a + b + c + d + e + f + g + h;
              numberBoard[(i - 1)][(j - 1)].isReveal= false;
              numberBoard[(i - 1)][(j - 1)].isFlag= false;


          }
      }
      
      
  }

  //Serial.print(numberBoard[0][0].number);
  printArrayZoneSmall();
  
 
  //printZoneRevealed(numberBoard);


  //printZoneRevealed(numberBoard);
  

  
}

void loop() {
  
  if (buttons()==1){
    numberBoard[rn_x][rn_y].isFlag=1;
    takeXStep();
    isChange=true;
    
  }

  if (buttons()==2){
    
    openZone(rn_x, rn_y);
    takeXStep();
    isChange=true;
  }

  if (buttons()==3){
    numberBoard[rn_x][rn_y].isFlag=0;
    matrix.drawPixel(rn_x, rn_y, matrix.Color333(7, 7, 7));
    isChange=true;
  }
  
  if (isChange==true){
      matrix.fillScreen(0);
  for (char i=0;i<8;i++){
    for (char j=0;j<8;j++){
      //Serial.print(i);
      //Serial.print("  ");
      //Serial.print(j);
      //Serial.print("  ");
      //Serial.println(numberBoard[i][j].number);
      //delay(100);
      //matrix.drawPixel(7-j,7-i,matrix.Color333(7,7,7));




      if(numberBoard[i][j].isFlag==1){
        matrix.drawPixel(7-j, 7-i, matrix.Color333(0,0,0));
      }
      else{
              if(numberBoard[i][j].isReveal==0){
        matrix.drawPixel(7-j, 7-i, matrix.Color333(7, 7, 7));
        Serial.println("it has not been released");
        
      }
      else{
             if (numberBoard[i][j].number == 0){
      matrix.drawPixel(7-j, 7-i, matrix.Color333(0, 0, 7));
      Serial.println("case 1 was triggered");
      
    }

    //orange 
    else if(numberBoard[i][j].number == 1){
      matrix.drawPixel(7-j, 7-i, matrix.Color333(7, 7, 0));
      Serial.println("case 2 was triggered");
      
    }
    //green
    else if(numberBoard[i][j].number == 2){
      matrix.drawPixel(7-j, 7-i, matrix.Color333(0, 7, 0));
      
      Serial.println("case 3 was triggered");
      
    }
    //orange
    else if(numberBoard[i][j].number==3){
      matrix.drawPixel(7-j, 7-i, matrix.Color333(7, 4, 0));
      Serial.println("case 4 was triggered");
      
    }
    //red
    else if(numberBoard[i][j].number==10){
      matrix.drawPixel(7-j, 7-i, matrix.Color333(7, 0, 0));
      Serial.println("case 5 was triggered");
    }
        
      }

        //matrix.drawPixel(7-j, 7-i, matrix.Color333(0, 0, 0));
        
      //blue

    
        
      }
      //black

      

      //pink
      if(i==rn_x && j==rn_y){
        matrix.drawPixel(7-j, 7-i, matrix.Color333(7, 0, 4));
      }


      isChange=false;
      
    
  }

      //delay(10);
      
    }
    
  }
  


  //printZoneRevealed(numberBoard);

  //openZone(6, 5, numberBoard);

  //printZoneRevealed(numberBoard);
  
  // put your setup code here, to run once:
  

}

/*
void convertToGame(){
  
}

void printZoneArray(zone arr[][10]){
    for(char i=1; i<9; i++){
        for(char j=1; j<9; j++){
            if (j==8){
                Serial.print(" ");
                Serial.println(arr[i][j].number);
                if (i==8){
                    Serial.println("`````````````````````");
                }
            }

            else{
                Serial.print(" ");
                Serial.print(arr[i][j].number);
            }

        }
    }

}

*/

void printArrayZoneSmall(){
    for(char k=0; k<8; k++){
        for(char l=0; l<8; l++){
            if (l==7){
                //cout << " " << k << endl;
                //cout << " " << l  << endl;
                Serial.print(" ");
                Serial.println(numberBoard[k][l].number);
                if (k==7){
                    Serial.println("`````````````````````");
                }
            }
            else{
                Serial.print(" ");
                Serial.print(numberBoard[k][l].number);
            }

        }
    }
}

void printZoneRevealed(){

  
    for (char i=0; i<8; i++){
        for(char j=0; j<8; j++){
            if (numberBoard[i][j].isReveal== 1){
                if (j==7){
                    //cout << " " << k << endl;
                    //cout << " " << l  << endl;
                    //cout << " " << arr[i][j].number  <<  endl;
                    Serial.print(" ");
                    Serial.println(numberBoard[i][j].number);
                    if (i==7){
                        //cout <<  "`````````````````````" << endl;
                        Serial.println("`````````````````````");
                    }
                }
                else{
                    //cout << " " << arr[i][j].number;
                    Serial.print(" ");
                    Serial.print(numberBoard[i][j].number);
                    
                }
            }
            else{
                if (j==7){
                    //cout << " " << k << endl;
                    //cout << " " << l  << endl;
                    //cout << " x"  << endl;
                    Serial.println(" x");
                    if (i==7){
                        //cout <<  "`````````````````````" << endl;
                        Serial.println("`````````````````````");
                    }
                }
                else{
                    //cout << " x";
                    Serial.print(" x");
                }


            }
        }
    }
}


char * GetZones_X(char current_x, char current_y){

    static char r[8];

    if (current_x==0){
        if (current_y==0){
            r[0]= 100;
            r[1]= 100;
            r[2]= 100;
            r[3]= current_x;
            r[4]= current_x+1;
            r[5]= current_x+1;
            r[6]= 100;
            r[7]= 100;


        }
        else if (current_y==7){
            r[0]= 100;
            r[1]= 100;
            r[2]= 100;
            r[3]= 100;
            r[4]= 100;
            r[5]= current_x+1;
            r[6]= current_x+1;
            r[7]= current_x;



        }
        else{
            r[0]= 100;
            r[1]= 100;
            r[2]= 100;
            r[3]= current_x;
            r[4]= current_x+1;
            r[5]= current_x+1;
            r[6]= current_x+1;
            r[7]= current_x;



        }
    }

    else if (current_x==7){
        if (current_y==0){
            r[0]= 100;
            r[1]= current_x-1;
            r[2]= current_x-1;
            r[3]= current_x;
            r[4]= 100;
            r[5]= 100;
            r[6]= 100;
            r[7]= 100;


        }
        else if (current_y==7){
            r[0]= current_x-1;
            r[1]= current_x-1;
            r[2]= 100;
            r[3]= 100;
            r[4]= 100;
            r[5]= 100;
            r[6]= 100;
            r[7]= current_x;


        }
        else{
            r[0]= current_x-1;
            r[1]= current_x-1;
            r[2]= current_x-1;
            r[3]= current_x;
            r[4]= 100;
            r[5]= 100;
            r[6]= 100;
            r[7]= current_x;


        }
    }
    else{
        if (current_y==0){
            r[0]= 100;
            r[1]= current_x-1;
            r[2]= current_x-1;
            r[3]= current_x;
            r[4]= current_x+1;
            r[5]= current_x+1;
            r[6]= 100;
            r[7]= 100;
        }

        else if(current_y==7){
            r[0]= current_x-1;
            r[1]= current_x-1;
            r[2]= 100;
            r[3]= 100;
            r[4]= 100;
            r[5]= current_x+1;
            r[6]= current_x+1;
            r[7]= current_x;
        }

        else{
            r[0]= current_x-1;
            r[1]= current_x-1;
            r[2]= current_x-1;
            r[3]= current_x;
            r[4]= current_x+1;
            r[5]= current_x+1;
            r[6]= current_x+1;
            r[7]= current_x;
        }



    }

    return r;
}

char * GetZones_Y(char current_x, char current_y){
    static char r[8];

    if (current_x==0){
        if (current_y==0){
            r[0]= 100;
            r[1]= 100;
            r[2]= 100;
            r[3]= current_y+1;
            r[4]= current_y+1;
            r[5]= current_y;
            r[6]= 100;
            r[7]= 100;



        }
        else if (current_y==7){
            r[0]= 100;
            r[1]= 100;
            r[2]= 100;
            r[3]= 100;
            r[4]= 100;
            r[5]= current_y;
            r[6]= current_y-1;
            r[7]= current_y-1;



        }
        else{
            r[0]= 100;
            r[1]= 100;
            r[2]= 100;
            r[3]= current_y+1;
            r[4]= current_y+1;
            r[5]= current_y;
            r[6]= current_y-1;
            r[7]= current_y-1;



        }
    }

    else if (current_x==7){
        if (current_y==0){
            r[0]= 100;
            r[1]= current_y;
            r[2]= current_y+1;
            r[3]= current_y+1;
            r[4]= 100;
            r[5]= 100;
            r[6]= 100;
            r[7]= 100;

        }
        else if (current_y==7){
            r[0]= current_y-1;
            r[1]= current_y;
            r[2]= 100;
            r[3]= 100;
            r[4]= 100;
            r[5]= 100;
            r[6]= 100;
            r[7]= current_y-1;


        }
        else{
            r[0]= current_y-1;
            r[1]= current_y;
            r[2]= current_y+1;
            r[3]= current_y+1;
            r[4]= 100;
            r[5]= 100;
            r[6]= 100;
            r[7]= current_y-1;


        }
    }
    else{
        if(current_y==0){
            r[0]= 100;
            r[1]= current_y;
            r[2]= current_y+1;
            r[3]= current_y+1;
            r[4]= current_y+1;
            r[5]= current_y;
            r[6]= 100;
            r[7]= 100;

        }
        else if(current_y==7){
            r[0]= current_y-1;
            r[1]= current_y;
            r[2]= 100;
            r[3]= 100;
            r[4]= 100;
            r[5]= current_y;
            r[6]= current_y-1;
            r[7]= current_y-1;

        }
        else{
            r[0]= current_y-1;
            r[1]= current_y;
            r[2]= current_y+1;
            r[3]= current_y+1;
            r[4]= current_y+1;
            r[5]= current_y;
            r[6]= current_y-1;
            r[7]= current_y-1;

        }

    }

    return r;
}

coordinate * CombineArrays(char current_x, char current_y){
    char *x;
    char *y;

    static coordinate final[8];


    x= GetZones_X(current_x, current_y);
    y= GetZones_Y(current_x, current_y);

    for ( char i = 0; i < 8; i++ ) {
        char final_x= *(x+i);
        char final_y= *(y+i);
        final[i].x= final_x;
        final[i].y= final_y;
    }

    return final;
}

/*
void printCoordinate(coordinate *arr, zone arr2[][8]){
    for (char i=0; i<8; i++){
        coordinate temp= *(arr+i);

        if (temp.x==100 && temp.y==100){
            //cout<< "there is no such node" << endl;
        }
        else{
            //cout<< "( " << temp.x << ", " << temp.y << "), and the value at this node is " << arr2[temp.x][temp.y].number << endl;
        }


    }
}

*/

void openZone(char current_x, char current_y){

    if (numberBoard[current_x][current_y].isReveal==0 && numberBoard[current_x][current_y].isFlag==0){
        numberBoard[current_x][current_y].isReveal= true;
        /*
        if (numberBoard[current_x][current_y].number==10){
          gameOver();
        }
        */

        //cout<< current_x<< ", ";
        //cout<<current_y<<endl;
        if (numberBoard[current_x][current_y].number==0){



            for (char i=0; i<8; i++){
                coordinate *final= CombineArrays(current_x, current_y);
                coordinate temp= *(final+i);
                char temp_x= temp.x;
                //cout<< temp_x<< ", ";

                char temp_y= temp.y;
                //cout<<temp_y<< endl;


                if (temp_x==100 && temp_y==100){
                    continue;
                }
                else{
                    if ((numberBoard[temp_x][temp_y].isReveal==0)){
                        //arr[temp_x][temp_y].isReveal;
                        openZone(temp_x, temp_y);
                    }
                }
            }
        }

    }

}

/*

void setColorPixel(char x, char y, String color) {
  //sets a pixel at (x, y) to a specific color
  //color is limited to white, black, red, orange, yellow, green, blue, purple, and pink

  if (color.equalsIgnoreCase("white")) {
    matrix.drawPixel(7-y, 7-x, matrix.Color333(7, 7, 7));
  }

  if (color.equalsIgnoreCase("black")) {
    matrix.drawPixel(7-y,7-x, matrix.Color333(0, 0, 0));
  }

  if (color.equalsIgnoreCase("red")) {
    matrix.drawPixel(7-y, 7-x, matrix.Color333(7, 0, 0));
  }

  if (color.equalsIgnoreCase("orange")) {
    matrix.drawPixel(7-y, 7-x, matrix.Color333(7, 4, 0));
  }

  if (color.equalsIgnoreCase("yellow")) {
    matrix.drawPixel(7-y, 7-x, matrix.Color333(7, 7, 0));
  }

  if (color.equalsIgnoreCase("green")) {
    matrix.drawPixel(7-y, 7-x, matrix.Color333(0, 7, 0));
  }

  if (color.equalsIgnoreCase("blue")) {
    matrix.drawPixel(7-y, 7-x, matrix.Color333(0, 0, 7));
  }

  if (color.equalsIgnoreCase("purple")) {
    matrix.drawPixel(7-y, 7-x, matrix.Color333(4, 0, 7));
  }

  if (color.equalsIgnoreCase("pink")) {
    matrix.drawPixel(7-y, 7-x, matrix.Color333(7, 0, 4));
  }

}

void drawBoard(char width, char height) {
  //draws a white rectangle of size width x height on the led matrix, positioned two spaces from the left, with height centered
  for(char i=0; i< width; i++){
    for(char j=0; j<height; j++){
      setColorPixel(i, j, "white");
    }
  }

}
*/

void displayNumber(char x) {
  //displays the number on the right side in red, centered on the y-axis

  String num = String(x);
  if (num.length() == 1) {
    num = String("0" + num);
  }
  
  matrix.setCursor(20, 4);
  matrix.setTextColor(matrix.Color333(7, 0, 0));
  matrix.setTextSize(1);

  matrix.print(num);
}

void gameOver() {
  //displays game over across the screen
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  
  matrix.setCursor(6, 0);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(3, 3, 3));

  matrix.print("GAME");

  matrix.setCursor(2, 8);
  matrix.print("OVER");
}

/*
void matchStrategy(char current_x, char current_y){
    //Serial.println("entered here")

    Serial.println(numberBoard[current_x][current_y].number);
    delay(1000); 
    if (numberBoard[current_x][current_y].number == 0){
      setColorPixel(current_x, current_y, "yellow");
      Serial.println("case 1 was triggered");
      
      
    }
    else if(numberBoard[current_x][current_y].number == 1){
      setColorPixel(current_x, current_y, "blue");
      Serial.println(F("case 2 was triggered"));
      
    }
    else if(numberBoard[current_x][current_y].number == 2){
      setColorPixel(current_x, current_y, "green");
      Serial.println("case 3 was triggered");
      
    }
    else if(numberBoard[current_x][current_y].number==3){
      setColorPixel(current_x, current_y, "purple");
      Serial.println("case 4 was triggered");
      
    }
    else if(numberBoard[current_x][current_y].number==4){
      setColorPixel(current_x, current_y, "pink");
      Serial.println("case 5 was triggered");
    }

    else if(numberBoard[current_x][current_y].number==10){
      setColorPixel(current_x, current_y, "red");
      Serial.println("case 6 was triggered");
    }
    else{
      
      Serial.println("edge case");
    }

     
    
  
  
}


void matchStrategyLarge(zone arr[][10], char current_x, char current_y){

  if(arr[current_x][current_y].number==1){//this is overridden and needs a better solution
    setColorPixel(current_x, current_y, "red");
  }
  else{
    setColorPixel(current_x, current_y, "white");
  }
  
}
*/


char takeXStep(){
  rn_x= rn_x+1;
  while (numberBoard[rn_x][rn_y].isReveal==1){
    rn_x= rn_x+1;
    if (rn_x==7){
      
      //go to the next value fo y and repeat 
      rn_x=0;
    }
  }
  return rn_x;
  
}

char takeYStep(){
  rn_y= rn_y+1;
  while (numberBoard[rn_x][rn_y].isReveal==1){
    rn_y= rn_y+1;
    if (rn_y==7){
      //go to the next value of x and repeat
      rn_y=0;
    }
  }
  return rn_y;
}


byte buttons(){
  byte buttonRead1 = digitalRead(button1);
  byte buttonRead2 = digitalRead(button2);

  if(buttonRead1 == 0){
    //Serial.println("Button 1 pressed!");
    if(buttonRead2 == 0){
      return 3;
    }
    else if(buttonState1 == false){
      buttonState1 = true;
      return 1;
    }
    else{
      buttonState1 = false;
      return 1;
    }
    delay(200);
  }

  else if(buttonRead2 == 0){
    if(buttonRead1 == 0){
      return 3;
    }
     //Serial.println("Button 2 pressed!");
    else if(buttonState2 == false){
      buttonState2 = true;
      return 2;
    }
    else{
      buttonState2 = false;
      return 2;
    }
    delay(200);
  }

  else if(buttonRead1 == 0 && buttonRead2 == 0){
    return 3;
    delay(200);
  }
  else{

  return 0;
  }
}
