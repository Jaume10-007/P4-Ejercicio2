#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

const int LED_PIN = 2;

// Semáforos binarios
SemaphoreHandle_t semOn;
SemaphoreHandle_t semOff;

// Tarea que enciende el LED
void taskLEDOn(void * parameter) {
    for(;;) {
        // Espera a poder encender
        if (xSemaphoreTake(semOn, portMAX_DELAY) == pdTRUE) {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED ENCENDIDO");
            vTaskDelay(500 / portTICK_PERIOD_MS);

            // Da paso a la tarea de apagar
            xSemaphoreGive(semOff);
        }
    }
}

// Tarea que apaga el LED
void taskLEDOff(void * parameter) {
    for(;;) {
        // Espera a poder apagar
        if (xSemaphoreTake(semOff, portMAX_DELAY) == pdTRUE) {
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED APAGADO");
            vTaskDelay(500 / portTICK_PERIOD_MS);

            // Da paso a la tarea de encender
            xSemaphoreGive(semOn);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    // Crear semáforos
    semOn = xSemaphoreCreateBinary();
    semOff = xSemaphoreCreateBinary();

    // Comprobar que se han creado bien
    if (semOn == NULL || semOff == NULL) {
        Serial.println("Error creando semaforos");
        while(true);
    }

    // Crear tareas
    xTaskCreate(
        taskLEDOn,
        "Task LED On",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        taskLEDOff,
        "Task LED Off",
        2048,
        NULL,
        1,
        NULL
    );

    // Estado inicial: permitimos primero encender
    xSemaphoreGive(semOn);
}

void loop() {
    // No hacemos nada aquí
}