# Mr Ribbits

### Summary

Mr. Ribbits is an autonomous robot designed to adapt its movement based on its environment and can be configured to track human faces.

The robot is built using an Arduino Uno and an ESP32S3, paired with servos and an expansion board for motor control. It is powered by a rechargeable lithium battery and also features a passive buzzer for additional functionality.

### Demo




### What Mr Ribbits can do!
- Basic AutoPilot -> Autonomous Driving
- Face Detection configured -> can hypothetically track faces
- Deploys local webpage to show face detection.
- Plays meme song when he is switched off, (he memes on people)
- isGrounded Feature
- play Tetris theme when not grounded
- SuicidePrevention()
- hes chill

### What to implement:
- Need Level Shifter to allow serial communication between Uno and ESP32, then can employ face tracking.
- More advanced Autopilot... Autopilotv2!
- Easter Egg feature - play hidden song by movement

### Is there lost potential?
1. IS set up to **detect acceleration, current rotation and temperature**, however have disabled this due to the memory cost and lack of innovative ideas to explore these features.
2. By streaming video and a JavaScript-based HTML page over an isolated WiFi connection, it becomes possible to control Mr. Ribbits directly from a computer.
