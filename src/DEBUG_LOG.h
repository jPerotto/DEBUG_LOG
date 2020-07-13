#ifndef _DEBUG_LOG
#define _DEBUG_LOG

#include "rom/uart.h"
#include "soc/uart_reg.h"
#include "soc/uart_struct.h"

static int s_uart_debug_nr = 0;

struct uart_struct_t {
  uart_dev_t * dev;
#if !CONFIG_DISABLE_HAL_LOCKS
  xSemaphoreHandle lock;
#endif
  uint8_t num;
  xQueueHandle queue;
  intr_handle_t intr_handle;
};

#if CONFIG_DISABLE_HAL_LOCKS
#define UART_MUTEX_LOCK()
#define UART_MUTEX_UNLOCK()

static uart_t _uart_bus_array[3] = {
  {(volatile uart_dev_t *)(DR_REG_UART_BASE), 0, NULL, NULL},
  {(volatile uart_dev_t *)(DR_REG_UART1_BASE), 1, NULL, NULL},
  {(volatile uart_dev_t *)(DR_REG_UART2_BASE), 2, NULL, NULL}
};
#else
#define UART_MUTEX_LOCK()    do {} while (xSemaphoreTake(uart->lock, portMAX_DELAY) != pdPASS)
#define UART_MUTEX_UNLOCK()  xSemaphoreGive(uart->lock)

static uart_t _uart_bus_array[3] = {
  {(volatile uart_dev_t *)(DR_REG_UART_BASE), NULL, 0, NULL, NULL},
  {(volatile uart_dev_t *)(DR_REG_UART1_BASE), NULL, 1, NULL, NULL},
  {(volatile uart_dev_t *)(DR_REG_UART2_BASE), NULL, 2, NULL, NULL}
};
#endif

#define CREATE_LOG(letter, log)  "[" #letter "][%s:%u] %s(): " log "\r\n", pathToFileName(__FILE__), __LINE__, __FUNCTION__
#define CREATE_PRINT(letter, log)  "[" #letter "]: " log "\r\n"

//#define LOG_BLE(log, ...) LOG_print(CREATE_LOG(BLE, log), ##__VA_ARGS__)
//#define LOG_BLE(format, ...)

int LOG_print(const char *format, ...)
{
  static char loc_buf[64];
  char * temp = loc_buf;
  int len;
  va_list arg;
  va_list copy;
  va_start(arg, format);
  va_copy(copy, arg);
  len = vsnprintf(NULL, 0, format, arg);
  va_end(copy);
  if (len >= sizeof(loc_buf)) {
    temp = (char*)malloc(len + 1);
    if (temp == NULL) {
      return 0;
    }
  }
  vsnprintf(temp, len + 1, format, arg);
#if !CONFIG_DISABLE_HAL_LOCKS
  if (_uart_bus_array[s_uart_debug_nr].lock) {
    xSemaphoreTake(_uart_bus_array[s_uart_debug_nr].lock, portMAX_DELAY);
    ets_printf("%s", temp);
    xSemaphoreGive(_uart_bus_array[s_uart_debug_nr].lock);
  } else {
    ets_printf("%s", temp);
  }
#else
  ets_printf("%s", temp);
#endif
  va_end(arg);
  if (len >= sizeof(loc_buf)) {
    free(temp);
  }
  return len;
}

String LOG_string(const char *format, ...)
{
  static char loc_buf[64];
  char * temp = loc_buf;
  int len;
  va_list arg;
  va_list copy;
  va_start(arg, format);
  va_copy(copy, arg);
  len = vsnprintf(NULL, 0, format, arg);
  va_end(copy);
  if (len >= sizeof(loc_buf)) {
    temp = (char*)malloc(len + 1);
    if (temp == NULL) {
      return String("NULL LOG");
    }
  }
  vsnprintf(temp, len + 1, format, arg);
  va_end(arg);
  if (len >= sizeof(loc_buf)) {
    free(temp);
  }
  return String(temp);
}
#endif
