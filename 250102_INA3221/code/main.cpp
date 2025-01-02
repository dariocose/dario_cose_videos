#include <Arduino.h>
#include <Wire.h>
#include <INA3221.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3f,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define SERIAL_SPEED     115200  // serial baud rate

// Set I2C address to 0x41 (A0 pin -> VCC)
INA3221 ina_0(INA3221_ADDR40_GND);

void current_measure_init() {
    ina_0.begin(&Wire);
    ina_0.reset();
    ina_0.setShuntRes(100, 100, 100);
    ina_0.setFilterRes(0.1, 0.1, 0.1);
}

void setup() {
    Serial.begin(SERIAL_SPEED);
    current_measure_init();

    lcd.init();
    lcd.backlight();

    while (!Serial) {
        delay(1);
    }
}

String format_curr(float f){
  int a = int(f);
  String sp = "-";

  if(a >= 0 && a < 10)
    sp = "   ";
  else if(a >= 10 && a < 100)
    sp = "  ";
  else if(a >= 100 && a < 1000)
    sp = " ";
  else if(a >= 1000)
    sp = "";

  return sp + String(a) ;
}

void loop() {
  float CH1_v = ina_0.getVoltage(INA3221_CH1);
  float CH1_c = ina_0.getCurrentCompensated(INA3221_CH1) * 1000;
  float CH2_v = ina_0.getVoltage(INA3221_CH2);
  float CH2_c = ina_0.getCurrentCompensated(INA3221_CH2) * 1000;
  float CH3_v = ina_0.getVoltage(INA3221_CH3);
  float CH3_c = ina_0.getCurrentCompensated(INA3221_CH3) * 1000;

  Serial.print("CH1 ");
  Serial.print(CH1_c, 0); // Corrente in mA, senza decimali
  Serial.print("ma ");
  Serial.print(CH1_v, 2); // Tensione con 1 decimale

  Serial.print("V CH2 ");
  Serial.print(CH2_c, 0); // Corrente in mA, senza decimali
  Serial.print("ma ");
  Serial.print(CH2_v, 2); // Tensione con 1 decimale
  Serial.print("V\r\n");

  // Stampa seconda riga
  Serial.print("CH3 ");
  Serial.print(CH3_c, 0); // Corrente in mA, senza decimali
  Serial.print("ma ");
  Serial.print(CH3_v, 2); // Tensione con 1 decimale
  Serial.print("V ");

  lcd.setCursor(0, 0);
  lcd.print(  String(CH1_v, 2)    +"|"+   String(CH2_v, 2)    +"|"+   String(CH3_v, 2)+" V"   );
  lcd.setCursor(0, 1);
  lcd.print(  format_curr(CH1_c)  +"|"+   format_curr(CH2_c)  +"|"+   format_curr(CH3_c)+"mA" );
  delay (300);
}