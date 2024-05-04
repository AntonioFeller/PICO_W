#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "bsp/board.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "lwip/apps/mqtt.h"

#define WIFI_SSID "Antonio"
#define WIFI_PASSWORD "17041994"
#define MQTT_SERVER "54.146.113.169"//"broker.emqx.io"
 
#define EN_A 18
#define A1 16
#define A2 17

uint16_t duty_cycle = 0;
int M_sentido = 0;
int Salve_sentido = 0;

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

static void rampaDutyCycle(uint16_t duty_cycle_alvo, uint16_t duty_cycle_inicial, uint16_t incremento) {
    
    uint16_t duty_cycle_atual = duty_cycle_inicial;

    if (duty_cycle_alvo > duty_cycle_inicial) {
        // Subida
        while (duty_cycle_atual < duty_cycle_alvo) {
            if (duty_cycle_atual + incremento <= duty_cycle_alvo) {
                duty_cycle_atual += incremento;
            } else {
                duty_cycle_atual = duty_cycle_alvo;
            }
            pwm_set_gpio_level(EN_A, duty_cycle_atual);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
            sleep_ms(25);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
            sleep_ms(25);
        }
    } else {
        // Descida
        while (duty_cycle_atual > duty_cycle_alvo) {
            if (duty_cycle_atual - incremento >= duty_cycle_alvo) {
                duty_cycle_atual -= incremento;
            } else {
                duty_cycle_atual = duty_cycle_alvo;
            }
            pwm_set_gpio_level(EN_A, duty_cycle_atual);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
            sleep_ms(25);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
            sleep_ms(25);
        }
    }
}

int parseJSON(char *json, char *key, char *value) {
    char *start = strstr(json, key);
    if (start == NULL) {
        return -1;
    }
    start = strchr(start, ':');
    if (start == NULL) {
        return -2;
    }
    start++;
    while (*start == ' ' || *start == '"') {
        start++;
    }
    char *end = start;
    while (*end != ',' && *end != '}' && *end != '\0') {
        end++;
    }
    while (*(end - 1) == '"' || *(end - 1) == ' ') {
        end--;
    }
    strncpy(value, start, end - start);
    value[end - start] = '\0';
    return 0;
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    printf("data: %s\n",data); 

    char motor[30];
    memset(motor, "\0", 30);
    strncpy(motor, data, len);

    char sentido[10];
    if (parseJSON(data, "\"sentido\"", sentido) == 0) {
        printf("Sentido: %s\n", sentido);

      if((strncmp(sentido, "SH", 2) == 0) &&(Salve_sentido != 0)){
      rampaDutyCycle(0, duty_cycle, 200);
      gpio_put(A1, true);
      gpio_put(A2, false);
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
      sleep_ms(200);
      Salve_sentido = 0;
      rampaDutyCycle(duty_cycle, 0, 200);
      } else if ((strncmp(sentido, "AT", 2) == 0)&&(Salve_sentido != 1)){
      rampaDutyCycle(0, duty_cycle, 500);
      gpio_put(A1, false);
      gpio_put(A2, true);
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
      sleep_ms(200);
      Salve_sentido = 1;
      rampaDutyCycle(duty_cycle, 0, 200);
      }else {
      printf("Valor de sentido fora do intervalo permitido, deve ser SH ou AT.\n");
      }
    } else {
        printf("Chave 'sentido' não encontrada ou formato inválido.\n");
    }

    char velocidade_str[30];
    if (parseJSON(data, "\"velocidade\"", velocidade_str) == 0) {
        int velocidade = atoi(velocidade_str);
        if (velocidade >= 0 && velocidade <= 65000) {
            printf("Velocidade: %d\n", velocidade);
            uint16_t duty_cycle_alvo = (uint16_t)velocidade;
            rampaDutyCycle(duty_cycle_alvo, duty_cycle, 200);
            duty_cycle = duty_cycle_alvo;
        } else {
            printf("Valor de velocidade fora do intervalo permitido, deve ser de 0 até 65000.\n");
        }
    } else {
        printf("Chave 'velocidade' não encontrada ou formato inválido.\n");
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
  err_t erro = mqtt_sub_unsub(client, "antonio/motor", 0,&mqtt_request_cb,NULL, 1);
  
  if(erro == ERR_OK){
    printf("Inscrito com sucesso\n");
  }else{
    printf("Falha ao inscrever\n");
  }

}else{
  printf("Falha na conexão\n");
}

}
 
int main() {

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

    //para utilizar o EN_A como um I/O simples descomente a linha de baixo
    //gpio_init(EN_A);
    gpio_set_function(EN_A, GPIO_FUNC_PWM);
    int fatia_pwm = pwm_gpio_to_slice_num(EN_A);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(fatia_pwm, &config, true);
   
    gpio_init(A1);
    gpio_init(A2);
 
    //para utilizar o EN_A como um I/O simples descomente a linha de baixo
    //gpio_set_dir(EN_A, GPIO_OUT);
 
    gpio_set_dir(A1, GPIO_OUT);
    gpio_set_dir(A2, GPIO_OUT);
 
    gpio_put(A1, true);
    gpio_put(A2, false);
    pwm_set_gpio_level(EN_A, duty_cycle);
 
    while(1) {
        
    sleep_ms(200);
            
    }
    
}