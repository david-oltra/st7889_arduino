#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // Instancia del TFT

/* Callback para LVGL: Envía los datos al TFT */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1));
    tft.pushColors((uint16_t *)&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);  // Indica que el área ha sido renderizada
}

void setup() {
    tft.begin();
    tft.setRotation(1);  // Ajusta la rotación si es necesario

    lv_init();  // Inicializa LVGL

    // Configura el búfer de dibujo
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[TFT_WIDTH * 10];
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * 10);

    // Configura el driver de pantalla
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;  // Callback para el TFT
    disp_drv.draw_buf = &draw_buf;

    lv_disp_drv_register(&disp_drv);  // Registra el driver

    // Crear un widget de prueba
    lv_obj_t *label = lv_label_create(lv_scr_act());  // Crea un label en la pantalla activa
    lv_label_set_text(label, "Hello LVGL 8.3!");    // Texto del label
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);       // Centra el label
}

void loop() {
    lv_timer_handler();  // Ejecuta la tarea de LVGL periódicamente
    delay(5);            // Intervalo para evitar sobrecarga del CPU
}
