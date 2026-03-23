# 📘 Práctica 4 – Sistemas Operativos en Tiempo Real (FreeRTOS)

## 🧪 Ejercicio 2: Sincronización de tareas con semáforos

---

## 🎯 Objetivo

Implementar dos tareas en el ESP32 utilizando FreeRTOS, donde una enciende un LED y la otra lo apaga, asegurando su correcta sincronización mediante el uso de semáforos.

---

## ⚙️ Descripción del programa

Se han creado dos tareas independientes:

* **Task LED On**: se encarga de encender el LED.
* **Task LED Off**: se encarga de apagar el LED.

Ambas tareas se sincronizan mediante dos semáforos binarios (`semOn` y `semOff`), de forma que nunca se ejecutan simultáneamente sobre el LED.

---

## 🧠 Conceptos clave

* **Multitarea (FreeRTOS)**
* **Semáforos binarios**
* **Sincronización de tareas**
* **Scheduler (planificador)**

---

## 🔌 Montaje hardware

Para la comprobación se ha utilizado:

* 1 LED
* 1 resistencia de 220Ω
* ESP32

### Conexión:

* GPIO 2 → resistencia → LED → GND

---

## 💻 Código utilizado

```cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

const int LED_PIN = 2;

SemaphoreHandle_t semOn;
SemaphoreHandle_t semOff;

void taskLEDOn(void * parameter) {
    for(;;) {
        if (xSemaphoreTake(semOn, portMAX_DELAY) == pdTRUE) {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED ENCENDIDO");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            xSemaphoreGive(semOff);
        }
    }
}

void taskLEDOff(void * parameter) {
    for(;;) {
        if (xSemaphoreTake(semOff, portMAX_DELAY) == pdTRUE) {
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED APAGADO");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            xSemaphoreGive(semOn);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    semOn = xSemaphoreCreateBinary();
    semOff = xSemaphoreCreateBinary();

    xTaskCreate(taskLEDOn, "Task LED On", 2048, NULL, 1, NULL);
    xTaskCreate(taskLEDOff, "Task LED Off", 2048, NULL, 1, NULL);

    xSemaphoreGive(semOn);
}

void loop() {}
```

---

## 🧾 Resultado obtenido

El LED conectado al ESP32 parpadea de forma periódica:

* Encendido durante 500 ms
* Apagado durante 500 ms

En el monitor serie se observa:

```
LED ENCENDIDO
LED APAGADO
LED ENCENDIDO
LED APAGADO
```

---

## ⚙️ Funcionamiento

* Se utilizan dos semáforos para controlar el acceso al LED.
* La tarea de encendido solo se ejecuta cuando recibe `semOn`.
* La tarea de apagado solo se ejecuta cuando recibe `semOff`.
* Cada tarea, al finalizar, activa la otra mediante el semáforo correspondiente.

Esto garantiza una ejecución ordenada y sincronizada.

---

## 🧠 Conclusión

El uso de semáforos en FreeRTOS permite coordinar tareas de forma eficiente, evitando conflictos y asegurando un comportamiento predecible del sistema. Esta técnica es fundamental en sistemas embebidos multitarea.

---
