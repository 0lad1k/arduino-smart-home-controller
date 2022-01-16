#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SparkFun_APDS9960.h>                                  // Для работы с датчиком APDS-9960
SparkFun_APDS9960 apds = SparkFun_APDS9960();                   // Определяем объект apds, экземпляр класса SparkFun_APDS9960
                                                                //
// Объявляем переменные:                                        //
uint16_t lightAmbient = 0;                                      // Определяем переменную для хранения освещённости общей            в люксах
uint16_t lightRed     = 0;                                      // Определяем переменную для хранения освещённости красного спектра в люксах
uint16_t lightGreen   = 0;                                      // Определяем переменную для хранения освещённости зелёного спектра в люксах
uint16_t lightBlue    = 0; 
uint16_t general    = 0; 

SFE_BMP180 pressure;

double getPressure(){
    char status;
    double T,P,p0,a;

    status = pressure.startTemperature();
    if (status != 0){
        // ожидание замера температуры
        delay(status);
        status = pressure.getTemperature(T);
        if (status != 0){
            status = pressure.startPressure(3);
            if (status != 0){
                // ожидание замера давления
                delay(status);
                status = pressure.getPressure(P,T);
                if (status != 0){
                    return(P);
                }
            }
        }
    }
}


void setup(){
      Serial.begin(9600);                                         // Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек
//  Инициируем работу датчика:                                  // 
    if(apds.init()){                                            // Если инициализация прошла успешно, то ...
          Serial.println("Initialization OK!");                 // Выводим сообщение об успешной инициализации датчика
    }else{Serial.println("Initialization ERROR!");}             // Иначе, выводим сообщение об ошибке инициализации датчика
                                                                //
//  Разрешаем режим определения освещённости:                   //
    if(apds.enableLightSensor(false)){                          // Если режим определения освещённости запущен (false - без прерываний на выходе INT), то ...
          Serial.println("Start light sensor OK!");             // Выводим сообщение об успешном запуске режима определения освещённости
    }else{Serial.println("Start light sensor ERROR!");}         // Иначе, выводим сообщение об ошибке запуска режима определения освещённости
                                                                // Запретить режим определения освещённости можно так: bool j = apds.disableLightSensor(); // в переменную j сохранится результат выполнения функции (true/false)
//  Ждём завершение инициализации и калибровки:                 //
    delay(500);
    Serial.begin(9600);
    Serial.println("Init"); 
    pressure.begin();
    Serial.println("Sucess");
}

void loop(){

    if(apds.readAmbientLight (lightAmbient)
    && apds.readAmbientLight(general)
    && apds.readRedLight     (lightRed)                         // И    прочитано значение освещённости красного спектра, и
    && apds.readGreenLight   (lightGreen)                       // И    прочитано значение освещённости зелёного спектра, и
    && apds.readBlueLight    (lightBlue)   ){                   // И    прочитано значение освещённости синего   спектра, то ...
          Serial.println((String) "Ambient=" + lightAmbient + ", Red=" + lightRed + ", Green=" + lightGreen + ", Blue=" + lightBlue + "General" + general + " lx"); // Выводим все прочитанные значения
    }else{Serial.println("Read light ERROR!");}                 // Иначе, выводим сообщение об ошибке чтения освещённости
                                                                //           //
    delay(4000); 
  
  Serial.println("Start");
    double P;
    P = getPressure();
    Serial.println(P, 4); 
    delay(4000);
}
