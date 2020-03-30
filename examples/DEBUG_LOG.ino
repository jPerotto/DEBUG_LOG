#include "LOG.h"

#define LOG_BLE(log, ...) LOG_print(CREATE_LOG(BLE, log), ##__VA_ARGS__)
//#define LOG_BLE(format, ...)

void setup()
{
  Serial.begin(115200);
  uint8_t x = 10;
  String st = "ola";
  LOG_BLE("TESTANDO NOVOS DEBUG %d - %s!", x, st.c_str());
  call_test();
}

void loop()
{

}

void call_test()
{
  LOG_BLE("TESTANDO NOVOS DEBUG EM OUTRAS FUNCOS!");
}
