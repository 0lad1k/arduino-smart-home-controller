#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SparkFun_APDS9960.h>                   
SparkFun_APDS9960 apds = SparkFun_APDS9960();                   
                                                                
uint16_t lightAmbient = 0;                                     

SFE_BMP180 pressure;
const String getRasberiReqest = "giveMeData";

double getPressure(){
    char status;
    double T,P,p0,a;

    status = pressure.startTemperature();
    if (status != 0){
        delay(status);
        status = pressure.getTemperature(T);
        if (status != 0){
            status = pressure.startPressure(3);
            if (status != 0){
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
      Serial.begin(9600);                                                                       
    if(apds.init()){                                            
          Serial.println("Initialization OK!");                
    }else{Serial.println("Initialization ERROR!");}             
                                                                
                  
    if(apds.enableLightSensor(false)){                         
          Serial.println("Start light sensor OK!");             
    }else{Serial.println("Start light sensor ERROR!");}         
                                                               
             
    delay(500);
    Serial.begin(9600);
    Serial.println("Init"); 
    pressure.begin();
    Serial.println("Sucess");
}

void loop(){

    if(apds.readAmbientLight (lightAmbient))
    {                  
        //Serial.println((String) "Ambient=" + lightAmbient); 
    }
    else
    {
        //Serial.println("Read light ERROR!");
    }

    if (Serial.available() > 0) 
    {
        String data = Serial.readStringUntil('\n');
        Serial.print("You sent me: ");
        if (data == getRasberiReqest)
        {
            apds.readAmbientLight (lightAmbient);
            Serial.println("{\"P\":\"" + String(getPressure()) +
              "\", \"lightAmbient\":\"" + String(lightAmbient) +
              "\"}");
        }
        
  }
}
