#include <DEBUG_LOG.h>

#define LOG_LOG(log, ...) LOG_print(CREATE_LOG(LOG, log), ##__VA_ARGS__)
#define LOG_STRING_FULL(log, ...) LOG_string(CREATE_LOG(STR, log), ##__VA_ARGS__)
#define LOG_STRING_LOG(log, ...) LOG_string(CREATE_LOG(STR, log), ##__VA_ARGS__)

//#define LOG_LOG(log, ...) 
//#define LOG_STRING_FULL(log, ...) 
//#define LOG_STRING_LOG(log, ...) 

void setup()
{
  String stringLogFull = LOG_STRING_FULL("salva na string o log, file, line e function");
  String stringLogLog = LOG_STRING_LOG("recebe apenas o nome do log na string");
  uint8_t x = 10;
  String st = "ola";
  LOG_LOG("TESTANDO NOVOS DEBUG %d - %s!", x, st.c_str());
  call_test();
}

void loop()
{

}

void call_test()
{
  LOG_LOG("TESTANDO NOVOS DEBUG EM OUTRAS FUNCOES!");
}
