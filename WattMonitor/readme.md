# Watt Monitor ⚡

This project is a Watt Monitor that reads data from a shunt connected to Pin 35 and GND of the microcontroller. It features a CYD3.5in 640x320 display without touch functionality to visualize power consumption in real time.

## Features ✨
- **Power Measurement:** Displays real-time wattage (Watts).
- **High-Resolution Display:** Uses a CYD3.5in 640x320 screen for clear and detailed visualization.
- **Real-Time Updates:** Provides live feedback on power consumption.

## Hardware Requirements 🛠️
- **Microcontroller:** Compatible with the shunt and display.
- **Shunt Resistor:** Connect to Pin 35 and GND.
- **CYD3.5in 640x320 Display:** No touch drivers are required.
- **Power Supply:** Ensure adequate power for the microcontroller and display.

## Wiring Diagram 🖼️
```
+---------------------+
| Microcontroller     |
|                     |
| Pin 35  <-----------+---------+ Shunt Resistor
| GND    <-----------+---------+
|                     |
+---------------------+
     |
     +-------> CYD3.5in Display (640x320, no touch)
```

## Software Requirements 💻
- **PlatformIO/Arduino IDE:** Development environment.
- **Libraries:**
  - `TFT_eSPI`: Graphics support for the CYD3.5in display.
  - `math.h`: For mathematical calculations.

## Installation 🚀
1. Clone this repository
2. Install the necessary libraries in your development environment.
3. Connect the hardware as per the wiring diagram.
4. Upload the firmware to your microcontroller.

## Usage 📊
1. Power on the device.
2. The display will show:
   - Power (W)
3. Monitor the real-time data for your connected load.

## Customization 🛠️
- **Adjust Pin Configuration:** Update `#define SHUNT_PIN 35` in the code if using a different pin.
- **Calibrate Measurements:** Modify the calibration constants to match your shunt specifications.

## Troubleshooting 🐛
- **No Display Output:**
  - Ensure the CYD3.5in display is properly connected.
  - Verify the display library is correctly installed.
- **Incorrect Readings:**
  - Check the wiring to the shunt resistor.
  - Validate calibration constants.

## Contributing 🤝
Feel free to submit issues or pull requests to improve this project.

## License 📜
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments 🙌
Special thanks to the open-source community for the libraries and tools that make this project possible.
