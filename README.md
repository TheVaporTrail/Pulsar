# Pulsar
Pulsar is a project to animate a single LED or other light source (e.g., a light ring or an electroluminescent wire). Animations can change brightness, color, or brightness and color. The inspiration was a single-color set of LEDs, so the initial animations all adjust only brightness.

## Inspiration
This project is inspired by two different sets of LEDs. The most recent is a string of blue LEDs with a USB connection on the end and can be either on or off. The original inspiration came many years ago when I had the idea of creating a set of spirals covered with LEDs and, I soldered over 60 LEDs in parallel between two wires. I wanted a way to have these LEDs pulse and fade and be more interesting that just on and off.

![Spiral](Misc/SpiralLEDs.jpg)

## Goals
* Time-based animations: The animation speed should not depend on how often the animation routines are called. Instead, the animations will use the current time (milliseconds) to determine the brightness.
* Light abstraction: The animations routines should not talk to the hardware directly. Instead, a `SinglePixel` class, with a `setBrightness` function, will be provided to the animation. Subclasses of the SinglePixel class will handle updating hardware. 

## Architecture

### SinglePixel class

### Animation routine

## Examples

### 01
Runs each animation for about 30 seconds and advances to the next. Build option to use either an analog pin to control the LED or a Neopixel.

### 02
Uses a motion detector to switch between a "stand-by" set of animations and an "active" set of animations.

### 03
Uses a Neopixel LED strip a "shift register", adding the latest LED setting to the start and shift all of the other LED settings towards the end. Alternative: use a Neopixel ring and shift from both ends towards the middle for a symmetrical effect.

### 04
Trinkey. Button 1: change color mode; Button 2: change??


## Next Steps
- [ ] Add setHue
- [ ] Add setBrightnessAndHue
- [ ] Called setHue from run loop to advance through colors, either with each animation change or continually
- [ ] Demonstrate different animations running independently on different pins
### Variations 
- [ ] Map brightness to hue
- [ ] Use brightness to select between two colors.
- [ ] Map brightness to distance along addressable pixel strip
