# Base application for ESP32 based Artnet projects

This base application does the heavy lifting and offers scaffolding for:
- Basic Ethernet communication with the W5500 Ethernet chip
- Standard implementation of Artnet protocol on the Ethernet connection for receiving pixel data
- Webinterface for setting up 
    - connected pixel system, 
    - ethernet settings, 
    - artnet settings,
    - Static colour settings and brightness
- Debugging and performance monitoring
- Sending pixel data to multiple pins