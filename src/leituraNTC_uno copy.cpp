// This library works only with the following circuit topology
// Vcc---NTC---ADC---SERIES_RESISTOR---GND
#include <Arduino.h>

#define R_SERIE 10000  // Resistor de 10k ohms em serie com o NTC
#define ADC_RESOLUTION 1023.0
#define TEMP_REFERENCE 273.15
#define NTC_PIN A5

double getTempTermistorNTCBeta(const float R_NTC, const uint16_t bCoefficient, const uint16_t nominalResistance)
{
  const float temp = 1.0 / ((1.0 / (TEMP_REFERENCE + 25.0)) + (1.0 / bCoefficient) * log(R_NTC / nominalResistance)); // 1.0/( (1/To)+(1/B)*ln(R/Ro) )
  return (temp - TEMP_REFERENCE);
}

double getTempTermistorNTCSteinhart(const float Log_RNTC, const float a, const float b, const float c)
{
  const float temp = 1.0 / (a + b * Log_RNTC + c * Log_RNTC * Log_RNTC * Log_RNTC);
  return (temp - TEMP_REFERENCE);
}

void setup()
{
  Serial.begin(115200); // INICIALIZA A SERIAL
  delay(1000);            // INTERVALO DE 1 SEGUNDO
}

void loop()
{
  const uint64_t currentTimeMS = millis();

  static uint64_t previousTimeMS1 = 0;
  if ((currentTimeMS - previousTimeMS1) >= 1000)
  {
    previousTimeMS1 = currentTimeMS;
    uint16_t V_BITS = analogRead(NTC_PIN);
    const float R_NTC = R_SERIE * ((ADC_RESOLUTION / (float)V_BITS) - 1);   // convert the value to resistance
    float temperature1 = getTempTermistorNTCBeta(R_NTC,                    // Analog Value
                                                 3455,        // thermistor's beta coefficient
                                                 10000); // Nominal resistance at 25 ºC
    float temperature2 = getTempTermistorNTCSteinhart(log(R_NTC),      // Ln da Resistance do NTC
                                                      0.001129241,         // a
                                                      0.0002341077,     // b
                                                      0.00000008775468);// c
    Serial.print(">Temp Beta: ");                                        // IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(temperature1);                                        // IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
    Serial.print(">Temp Steinhart: ");                                   // IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(temperature2);                                        // IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
  }
}
