#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SparkFun_APDS9960.h>
#include <Thread.h>

SparkFun_APDS9960 apds = SparkFun_APDS9960();

uint16_t lightAmbient = 0;
const int redLedPin = 11;
const int ledPin = 13;
Thread ledThread = Thread();
Thread soundThread = Thread();
SFE_BMP180 pressure;
const String getRasberiReqest = "giveMeData";
bool isError = false;
void ledBlink()
{
    static bool ledStatus = false;
    ledStatus = !ledStatus;
    digitalWrite(ledPin, ledStatus && !isError);
}

double getPressure()
{
    char status;
    double T, P, p0, a;

    status = pressure.startTemperature();
    if (status != 0)
    {
        delay(status);
        status = pressure.getTemperature(T);
        if (status != 0)
        {
            status = pressure.startPressure(3);
            if (status != 0)
            {
                delay(status);
                status = pressure.getPressure(P, T);
                if (status != 0)
                {
                    isError = false;
                    return (P);
                }
            }
        }
    }
    isError = true;
}

void sound()
{
    digitalWrite(redLedPin, isError);
    int P = getPressure();
    if (apds.readAmbientLight(lightAmbient))
    {

        if (!isError)
        {
            if (Serial.available() > 0)
            {
                String data = Serial.readStringUntil('\n');
                if (data == getRasberiReqest)
                {
                    apds.readAmbientLight(lightAmbient);
                    Serial.println("{\"Pressure\":\"" + String(P) +
                                   "\", \"lightAmbient\":\"" + String(lightAmbient) +
                                   "\"}");
                }
            }
        }
    }
    else
    {
        isError = true;
    }
}

void setup()
{
    soundThread.onRun(sound);
    soundThread.setInterval(20);
    ledThread.onRun(ledBlink);
    ledThread.setInterval(1000);
    pinMode(redLedPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
    if (apds.init())
    {
    }
    else
    {
        isError = true;
    }

    if (apds.enableLightSensor(false))
    {
    }
    else
    {
        isError = true;
    }

    Serial.begin(9600);
    pressure.begin();
}

void loop()
{

    if (ledThread.shouldRun())
        ledThread.run();
    if (soundThread.shouldRun())
        soundThread.run();
}
