#include <Wire.h>         // Thư viện I2C cho ESP32
#include <BH1750.h>       // Thư viện điều khiển cảm biến BH1750

#define I2C_SDA_PIN 21    // Chân SDA nối vào BH1750
#define I2C_SCL_PIN 22    // Chân SCL nối vào BH1750

BH1750 lightMeter;

void setup() {
  Serial.begin(115200);                            // Tốc độ Serial phù hợp (115200)
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);            // Khởi tạo I2C với chân tùy chỉnh
  
  // Bắt đầu cảm biến BH1750
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    // BH1750 đã sẵn sàng
  } else {
    // In -1 nếu không tìm thấy cảm biến, để dễ debug
    Serial.println("-1");
    while (1); // Dừng chương trình
  }
}

void loop() {
  float lux = lightMeter.readLightLevel();

  // In ra giá trị ánh sáng (lux), không có chữ hay đơn vị
  if (lux < 0) {
    Serial.println("-1");  // Giá trị lỗi
  } else {
    Serial.println(lux, 2); // In giá trị với 2 chữ số sau dấu thập phân
  }

  delay(100); // 10Hz (mỗi 100ms) – đủ để Edge Impulse nhận dạng tần số
}