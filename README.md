# Caching IoT-based Weather Monitoring System                                                                                                
## Overview
This project involves developing a caching IoT-based weather monitoring system with an embedded system server and a mobile application. The server measures indoor weather parameters and the mobile application displays these measurements.

## Table of Contents
- [Overview](#overview)                                                                                                                      - [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Contributing](#contributing)

## Project Structure

```
.
├── LICENSE
├── src
│   ├── .clangd
│   ├── Makefile
│   ├── main.c
│   ├── hal
│   │   ├── dht11.c
│   │   ├── dht11.h
│   │   ├── ds1307.c
│   │   ├── ds1307.h
│   │   ├── esp01.c
│   │   ├── esp01.h
│   │   ├── lcd.c
│   │   ├── lcd.h
│   ├── app
│   │   ├── server.c
│   │   ├── server.h
│   │   ├── storage.c
│   │   ├── storage.h
│   │   ├── weather.c
│   │   ├── weather.h
│   ├── mcal
│   │   ├── adc.c
│   │   ├── adc.h
│   │   ├── gpio.c
│   │   ├── gpio.h
│   │   ├── twi.c
│   │   ├── twi.h
│   │   ├── usart.c
│   │   ├── usart.h
```


## Installation

1. **Clone the repository:**
   ```bash                                                                                              git clone https://github.com/kmuali/weather-iot
   ```

2. **Build the project using the Makefile:**
   ```bash
   make flash-arduino
   ```

## Usage

1. **Upload the firmware to the embedded device.**
2. **Run the mobile application to connect to the embedded system server's WiFi access point.**   
3. **View weather information on the mobile app's graphical interface.**

## License

This project is licensed under the terms of the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

## Contributing

1. **Fork the repository.**                                                                          2. **Create a new branch for your feature or bugfix.**                                               3. **Make your changes.**
4. **Submit a pull request.**