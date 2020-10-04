#include <SPI.h>
#include <MFRC522.h>//nfc头文件
#include <Servo.h>//舵机头文件

MFRC522 rfid(10, 9);//创建一个rc522读卡器对象
Servo mg90s;//创建一个舵机控制对象
// 初始化数组用于存储读取到的NUID 
byte nuidPICC[4];
void(* resetFunc) (void) = 0; //制造重启命令
void setup()
{
    delay(200); 
    pinMode(LED_BUILTIN, OUTPUT);
    SPI.begin(); // 初始化SPI总线
    rfid.PCD_Init();
    
    Serial.begin(9600);
    mg90s.attach(6);//设定舵机的接口为6
    mg90s.write(0);//设定舵机旋转角度复位为0.(可调0~180)
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);//设置Arduino的5,6引脚为输出(OUTPUT)模式

    return;
}

void loop()
{
      // 找卡
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
 
  // 验证NUID是否可读
  if ( ! rfid.PICC_ReadCardSerial())
    return;
 
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  
  // 将NUID保存到nuidPICC数组
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    Serial.println(nuidPICC[i]);
  }   
  if (nuidPICC[0]==10 && nuidPICC[1]==35 && nuidPICC[2]==30 && nuidPICC[3]==50)//改成自己卡的uid
  {
    mg90s.write(180);
    delay(3000);
    digitalWrite(LED_BUILTIN, HIGH);
    mg90s.write(0);
    resetFunc();      //重启程序开始
  }
}
