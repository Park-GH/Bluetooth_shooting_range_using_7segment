/*
  광운대학교 전자공학과 기초전자회로실험2 팀프로젝트
  박건희 이동준 우여진
*/

//점수랑 핀번호는 상황에 맞게 바꿈
//세그먼트 출력함수 만들기
//타임어택, pvp 추가하기


#include <Servo.h>
#include <SoftwareSerial.h>
#include <TM1637Display.h>

#define CLK 12
#define DIO 13

#define Count_CLK 8
#define Count_DIO 9


#define button10 5  //각 점수에 해당하는 입력 핀 번호
#define button50 7
#define button80 6

TM1637Display display(CLK, DIO);
TM1637Display display2(Count_CLK, Count_DIO);
SoftwareSerial mySerial(10,  11); // TX, RX  (블루투스 통신)
Servo point10;  // 서보모터 제어를 위한 오브젝트
Servo point50;
Servo point80; 

unsigned long snt=0;
unsigned long cnt=0;
int flag_t=0;

bool start=1;
int count;
int score = 0;
int Highscore = 0;
int flag[4] = {0,};

void setup() {
  display.setBrightness(3);
  display2.setBrightness(3);
  
  mySerial.begin(9600); //블루투스 통신 시작
  Serial.begin(9600); //시리얼모니터 통신 시작
  
  point10.attach(2);  //각 점수에 해당하는 서보모터 핀 번호
  point50.attach(3);
  point80.attach(4);
  
  pinMode(button10,INPUT);  //각 버튼들 신호 입력 설정
  pinMode(button50,INPUT);
  pinMode(button80,INPUT);

  display.showNumberDec(0,true,4,0);
  display2.showNumberDec(0,true,4,0);
  AllMotor();

}

void loop() {
    if(start==1)
    {
    display2.showNumberDec(0000,true,4,0);
    Serial.println("Game select");
    Serial.println("1=Highscore");
    Serial.println("2=Timeattack");
    start=0;
    }

 
    if (mySerial.available()) {
      char Bt;
      Serial.println("Game is choosen");
      mySerial.println("Game is choosen");
      Bt=mySerial.read();
      Serial.println(Bt);
      
      if(Bt==49){ //블루투스 입력에 따라 게임모드 다르게 실행, 아스키코드 1,2
        mySerial.println("HighScore");
        HighScore();  //Run HighScore
      }              
      else if(Bt==50){
        mySerial.println("TimeAttack");
        TimeAttack(); //Run TimeAttack
      /*else if(Bt=='Score')
        PvP();  //Run PlayeVsPlayer
      */
      }

      else
        Serial.println("Game is not choosen");
    }
}

void HighScore(){  

  /*
    버튼이 입력되고 flag가 0일때 점수 올라감
    버튼이 1일때 계속 점수가 올라갈 수 있으니 변수 flag로 방지
    flag가 전부 1이 되었을때 표적 위치 초기화 후 flag 0으로 되돌림
    총알이 다 떨어지면 Endgame 입력
    최고점수면은 메시지 출력
  */
  
  score = 0;
  //AllMotor();
  while(1){
      display2.showNumberDec(1111,true,4,0);
      display.showNumberDec(score,true,4,0);
    if(digitalRead(button10) && flag[0]==0){
      score+=10;
      Serial.println(score);
      mySerial.println(score);
      flag[0]=1;
      }
    if(digitalRead(button50) && flag[1]==0){
      score+=50;
      Serial.println(score);
      mySerial.println(score);
      flag[1]=1;
      }
    if(digitalRead(button80) && flag[2]==0){
      score+=80;
      Serial.println(score);
      mySerial.println(score);
      flag[2]=1;
      }
      
      flag[3]=flag[0]*flag[1]*flag[2];
      
    if(flag[3]){
      display.showNumberDec(score,true,4,0);
      AllMotor();
      flag[0]=0;
      flag[1]=0;
      flag[2]=0;
    }
    /*if(mySerial.available()){
      char Bt;
      Bt=mySerial.read();
      Serial.println(Bt);
      if(Bt=='E'){
        if(score>=Highscore){
          Highscore=score;
          Serial.print("You get Highscore!!  ");
          Serial.println(Highscore);
          AllMotor();
        }
        start=1;
        break;
      }
    }*/
  }   
 }  //고득점 목표 게임


void TimeAttack(){  

  /*
  30초의 제한시간동안 최대한 많은 표적을 맞춰야 된다.
  */
  count=30;
  //AllMotor();
  display2.showNumberDec(2222,true,4,0);
  score = 0;
  snt=millis();
  while(1){

      count=30-int((millis()-snt)/1000);
      display.showNumberDec(score,true,4,0);
      display2.showNumberDec(count,true,4,0);

    if(count<=0){ 
      Highscore = score;
      display.showNumberDec(score,true,4,0);
      display2.showNumberDec(Highscore,true,4,0);
      mySerial.println("break");
      flag[0]=0;
      flag[1]=0;
      flag[2]=0;    
      flag_t=0;
      delay(5000);
      AllMotor();
      break;
      } // 카운팅이 0 되면 종료

    

      
    if(digitalRead(button10) && flag[0]==0){
      score+=10;
      Serial.println(score);
      mySerial.println(score);
      flag[0]=1;
      }
    if(digitalRead(button50) && flag[1]==0){
      score+=50;
      Serial.println(score);
      mySerial.println(score);
      flag[1]=1;
      }
    if(digitalRead(button80) && flag[2]==0){
      score+=80;
      Serial.println(score);
      mySerial.println(score);
      flag[2]=1;
      }
      

    if(flag[0]==1 && flag[1]==1 && flag[2]==1 && flag_t==0){
      flag_t=1;
      Motor10();
      flag[0]=2;
    }


    if(digitalRead(button10) && (flag[0]==2)){
      score+=10;
      Serial.println(score);
      mySerial.println(score);
      flag[0]=0;
      Motor50();
      flag[1]=2;
    }
    
    if(digitalRead(button50) && (flag[1]==2)){
      score+=50;
      Serial.println(score);
      mySerial.println(score);
      flag[1]=0;
      Motor80();
      flag[2]=2;
    }    
    
    if(digitalRead(button80) && (flag[2]==2)){
      score+=80;
      Serial.println(score);
      mySerial.println(score);
      flag[2]=0;
      Motor10();
      flag[0]=2;
    }
    /*
    if(mySerial.available()){
      char Bt;
      Bt=mySerial.read();
      Serial.println(Bt);
      if(Bt=='E'){
        if(score>=Highscore){
          Highscore=score;
          Serial.print("You get Highscore!!  ");
          Serial.println(Highscore);
          AllMotor();
        }
        start=1;
        break;
      }
    }*/
  }   
  
 }  //타임어택

 
 void AllMotor(){
  point10.write(0);
  point50.write(0);
  point80.write(0);
  display.showNumberDec(score,true,4,0);
  delay(800);
  point10.write(120);
  point50.write(120);
  point80.write(120);  
 }  //표적 위치 초기화 함수

 void Motor10(){
  point10.write(0);
  display.showNumberDec(score,true,4,0);
  count=30-int((millis()-snt)/1000);
  display2.showNumberDec(count,true,4,0);
  delay(800);
  point10.write(120);

 }//표적 위치 초기화 함수

 void Motor50(){
  point50.write(0);
  display.showNumberDec(score,true,4,0);
  count=30-int((millis()-snt)/1000);
  display2.showNumberDec(count,true,4,0);
  delay(800);
  point50.write(120);

 }//표적 위치 초기화 함수

 void Motor80(){
  point80.write(0);
  display.showNumberDec(score,true,4,0);
  count=30-int((millis()-snt)/1000);
  display2.showNumberDec(count,true,4,0);
  delay(800);
  point80.write(120);

 }//표적 위치 초기화 함수
