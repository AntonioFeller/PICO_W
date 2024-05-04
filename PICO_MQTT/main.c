#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "bsp/board.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"cma
#include "lwip/apps/mqtt.h"
#include "hardware/adc.h"

#define WIFI_SSID "Antonio"
#define WIFI_PASSWORD "17041994"
#define MQTT_SERVER "54.146.113.169"//"broker.emqx.io"
 

float read_core_temperature();

struct mqtt_connect_client_info_t mqtt_client_info=
{
  "antonio/pico_w", /*client id*/
  NULL, /* user */
  NULL, /* pass */
  0,  /* keep alive */
  NULL, /* will_topic */
  NULL, /* will_msg */
  0,    /* will_qos */
  0     /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
  , NULL
#endif
};

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    printf("data: %s\n",data); 
    char led[30];
    memset(led, "\0", 30);
    strncpy(led, data, len);

    if(strncmp(led, "on", 2) == 0) {
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    } else if (strncmp(led, "off", 3) == 0){
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
    }
}
  
static void mqtt_incoming_publish_cb(void *arg, const char *topic,u32_t tot_len) {
  printf("topic %s\n", topic);
}
 
static void mqtt_request_cb(void *arg, err_t err) { 
  printf(("MQTT client request cb: err %d\n", (int)err));
}
 
static void mqtt_connection_cb(mqtt_client_t *client, void *arg,mqtt_connection_status_t status) {
  printf(("MQTT client connection cb: status %d\n", (int)status));

  if(status == MQTT_CONNECT_ACCEPTED){
    err_t erro = mqtt_sub_unsub(client, "antonio/led", 0,&mqtt_request_cb,NULL, 1);
  
  if(erro == ERR_OK){
    printf("Inscrito com sucesso\n");
  }else{
    printf("Falha ao inscrever\n");
  }
}else{
  printf("Falha na conexão\n");
}

}

float read_core_temperature() {
    adc_init();
    adc_gpio_init(26);
    adc_select_input(4);
    sleep_ms(10);
    uint16_t result = adc_read();
    uint16_t result = 20;
    float temperature = (result * 3.3f / (1 << 12)) * 100 - 273.15f;
    return temperature;
}
 
int main()
{
    stdio_init_all();
    printf("Iniciando...\n");

    if(cyw43_arch_init()){
      printf("Falha ao iniciar\n");
      return 1;
    }


  cyw43_arch_enable_sta_mode();
  printf("Conectando ao %s\n", WIFI_SSID);

  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
  {
    printf("Falha ao conectar a rede\n");
    return 1;
  }
    printf("Conectado\n");

    ip_addr_t addr;
    if(!ip4addr_aton(MQTT_SERVER,&addr))
    {
      printf("IP error\n");
     return 1;
    }

    printf("Conectando ao MQTT\n");

    mqtt_client_t* cliente_mqtt = mqtt_client_new();

    mqtt_set_inpub_callback(cliente_mqtt,&mqtt_incoming_publish_cb,&mqtt_incoming_data_cb,NULL);

    err_t erro = mqtt_client_connect(cliente_mqtt,&addr,1883,&mqtt_connection_cb,NULL,&mqtt_client_info);

    if(erro != ERR_OK){
       printf("Erro de conexão\n");
     return 1;
    }

    printf("Conectado ao MQTT\n");

    float temperatura;


 while(true)
 {

    temperatura = read_core_temperature();
    printf("Temperatura do núcleo: %.2f°C\n", temperatura);

    char temperatura_str[30];

    sprintf(temperatura_str, "%.2f", temperatura);

    if(board_button_read()){
      mqtt_publish(cliente_mqtt, "antonio/temperature", temperatura_str, strlen(temperatura_str), 0, false, &mqtt_request_cb, NULL);
      }
      sleep_ms(200);
}
                           
}