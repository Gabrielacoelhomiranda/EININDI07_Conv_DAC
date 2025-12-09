// This library works only with the following circuit topology
// Vcc---NTC---ADC---SERIES_RESISTOR---GND
#include <Arduino.h>
#define ADC_RESOLUTION 1023
#define TEMPERATURENOMINAL 25
#define NTC_PIN A5

double getTempTermistorNTCBeta(const float R_NTC, const uint16_t bCoefficient, const uint16_t nominalResistance)
{
  float temp = 0.0;
  temp = 1.0 / ((1.0 / (TEMPERATURENOMINAL + 273.15)) + (1.0 / bCoefficient) * log(R_NTC / nominalResistance)); // 1.0/( (1/To)+(1/B)*ln(R/Ro) )
  return (temp - 273.15);
}

double getTempTermistorNTCSteinhart(const float Log_RNTC, const float a, const float b, const float c)
{
  float temp = 0.0;
  temp = 1.0 / (a + b * Log_RNTC + c * Log_RNTC * Log_RNTC * Log_RNTC);
  return (temp - 273.15);
}

void setup()
{
  Serial.begin(115200); // INICIALIZA A SERIAL
  delay(1000);          // INTERVALO DE 1 SEGUNDO
}

#define R_SERIE 1000  // Resistor de 10k ohms em serie com o NTC
void loop()
{
  const uint64_t currentTimeMS = millis();

  static uint64_t previousTimeMS1 = 0;
  if ((currentTimeMS - previousTimeMS1) >= 1000)
  {
    previousTimeMS1 = currentTimeMS;
    uint16_t analogValue = analogRead(NTC_PIN);
    const float R_NTC = R_SERIE * ((ADC_RESOLUTION / (float)analogValue) - 1);   // convert the value to resistance

    float temperature1 = getTempTermistorNTCBeta(R_NTC,                    // Analog Value
                                                 10000,      // Nominal resistance at 25 ºC
                                                 3455); // thermistor's beta coefficient
    float temperature2 = getTempTermistorNTCSteinhart(log(R_NTC),      // Ln da Resistance do NTC
                                                      0.001129241,       // a
                                                      0.0002341077,      // b
                                                      0.00000008775468); // c
    Serial.print(">Temp Beta: ");                                        // IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(temperature1);                                        // IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
    Serial.print(">Temp Steinhart: ");                                   // IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(temperature2);                                        // IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
  }
}
