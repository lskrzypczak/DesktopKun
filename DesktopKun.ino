#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "person_sensor.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// How long to wait between reading the sensor. The sensor can be read as
// frequently as you like, but the results only change at about 5FPS, so
// waiting for 200ms is reasonable.
const int32_t SAMPLE_DELAY_MS = 200;

void setup() {
  // You need to make sure you call Wire.begin() in setup, or the I2C access
  // below will fail.
  Wire.begin();
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE); 
  //display.setCursor(4,4);
  //display.println("CRASH");

  display.fillRoundRect(10, 10, 40, 40, 10, WHITE);
  display.fillRoundRect(SCREEN_WIDTH - 10 - 40, 10, 40, 40, 10, WHITE);
  display.display();

}

void loop() {
  person_sensor_results_t results = {};
  // Perform a read action on the I2C address of the sensor to get the
  // current face information detected.
  if (!person_sensor_read(&results)) {
    Serial.println("No person sensor results found on the i2c bus");
    delay(SAMPLE_DELAY_MS);
    return;
  }

  if (results.num_faces > 0)
  {
    const person_sensor_face_t* face = &results.faces[0];
    int face_width = face->box_right - face->box_left;
    int face_height = face->box_bottom - face->box_top;
    int face_x = face->box_left + face_width / 2;
    int face_y = face->box_top + face_height / 2;
    double dx = (40.0/256.0) * static_cast<double>(face_x);
    double dy = (40.0/256.0) * static_cast<double>(face_y);
    Serial.print("face: ");
    Serial.print(face_x);
    Serial.print(",");
    Serial.print(face_y);
    Serial.print("; d: ");
    Serial.print(dx);
    Serial.print(",");
    Serial.println(dy);
    display.fillRoundRect(10, 10, 40, 40, 10, WHITE);
    display.fillRoundRect(SCREEN_WIDTH - 10 - 40, 10, 40, 40, 10, WHITE);
    display.fillRoundRect(10 + static_cast<int>(40 - dx), 10 + static_cast<int>(dy), 10, 10, 3, BLACK);
    display.fillRoundRect(SCREEN_WIDTH - 10 - 40 + static_cast<int>(40 - dx), 10 + static_cast<int>(dy), 10, 10, 3, BLACK);
    display.fillRect(60, 50, 10, 14, BLACK);
    if (face->is_facing)
    {
      display.setCursor(60, 50);
      display.println("U");
    }
    else
    {
      display.setCursor(60, 50);
      display.println("-");
    }
    display.display();
  }
  else
  {
    display.clearDisplay();
    display.fillRoundRect(10, 20, 40, 20, 10, WHITE);
    display.fillRoundRect(SCREEN_WIDTH - 10 - 40, 20, 40, 20, 10, WHITE);
    display.display();
  }

  // Serial.println("********");
  // Serial.print(results.num_faces);
  // Serial.println(" faces found");
  // for (int i = 0; i < results.num_faces; ++i) {
  //   const person_sensor_face_t* face = &results.faces[i];
  //   Serial.print("Face #");
  //   Serial.print(i);
  //   Serial.print(": ");
  //   Serial.print(face->box_confidence);
  //   Serial.print(" confidence, (");
  //   Serial.print(face->box_left);
  //   Serial.print(", ");
  //   Serial.print(face->box_top);
  //   Serial.print("), (");
  //   Serial.print(face->box_right);
  //   Serial.print(", ");
  //   Serial.print(face->box_bottom);
  //   Serial.print("), ");
  //   if (face->is_facing) {
  //     Serial.println("facing");
  //   } else {
  //     Serial.println("not facing");
  //   }
  // }
  delay(SAMPLE_DELAY_MS);
}
