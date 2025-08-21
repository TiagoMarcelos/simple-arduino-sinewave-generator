// SinWave Generator
// This Arduino sketch generates a variable-frequency sine wave using PWM.

// --- Hardware Requirements ---
// The PWM output (Pin 6) must be connected to a **second-order passive RC low-pass filter**
// to smooth the PWM signal into an analog sine wave.
//
// Recommended Filter Components:
// - **Resistors (R):** 10 kOhm (for each stage)
// - **Capacitors (C):** 100 nF (for each stage)
//  
// With these components, the Cutoff frequency is around 159 Hz.

// --- Pin Definitions ---
const int PWM_PIN = 6;         // PWM output pin for the sine wave
const int POT_PIN = A0;        // Analog input pin for the potentiometer
// const int FILTERED_ANALOG_PIN = A5; // Analog input pin for the filtered signal (uncomment if used)

// --- Sine Wave Parameters ---
const int NUM_STEPS = 256;      // Number of steps to define one full sine wave cycle
byte sineWaveValues[NUM_STEPS];  // Array to store pre-calculated sine wave values (0-255 for PWM), save memory by using byte

// --- Frequency Control Parameters ---
// These define the range of frequencies that can be generated.
// Smaller delay = Higher frequency.
// This results in a sine wave frequency range of:
//    ~3.9 Hz (at max delay of 1000 microsec) to ~78.1 Hz (at min delay of 50 microsec).
const int MIN_DELAY_MICROSECONDS = 50;   // For higher frequency (e.g., 50 microsec)
const int MAX_DELAY_MICROSECONDS = 1000; // For lower frequency (e.g., 1000 microsec)

// --- Variables ---
int potValue = 0;              // Raw reading from the potentiometer (0-1023)
int stepDelayMicroseconds = 0; // Delay for each step, derived from potValue
// int filteredSensorValue = 0;   // Variable to store the raw analog reading from filteredAnalogPin (uncomment if used)

void setup() {
  // Initialize the PWM pin as an output
  pinMode(PWM_PIN, OUTPUT);

  // Initialize the analog input pin for the potentiometer
  pinMode(POT_PIN, INPUT);
  // pinMode(FILTERED_ANALOG_PIN, INPUT); // Uncomment if using filtered signal input

  // Pre-calculate sine wave values and store them in the array
  // Values are scaled from 0-255 for analogWrite
  for (int i = 0; i < NUM_STEPS; i++) {
    float angle = (float)i / NUM_STEPS * TWO_PI; // Use TWO_PI for clarity
    sineWaveValues[i] = (int)(sin(angle) * 127.5 + 127.5);
  }

  // Start serial communication for debugging/monitoring
  // Serial.begin(115200); // Uncomment for serial plotting/debugging
}

void loop() {
  // Read the potentiometer value (0-1023)
  potValue = analogRead(POT_PIN);

  // Map the potentiometer value (0-1023) to the desired delay range.
  // The mapping is inverted: higher potValue -> lower delay -> higher frequency.
  stepDelayMicroseconds = map(potValue, 0, 1023, MAX_DELAY_MICROSECONDS, MIN_DELAY_MICROSECONDS);

  // Loop through the pre-calculated sine wave values to generate the waveform
  for (int i = 0; i < NUM_STEPS; i++) {
    // Write the current sine value to the PWM pin
    analogWrite(PWM_PIN, sineWaveValues[i]);
    
    // Read the filtered value from A5 *immediately after* writing the PWM value
    // filteredSensorValue = analogRead(FILTERED_ANALOG_PIN);

    // --- SERIAL PLOTTING DATA (Uncomment for debugging/visualization) ---
    // For plotting BOTH the raw PWM value and the filtered A5 value:
    // Serial.print(sineWaveValues[i]);      // Original PWM value
    // Serial.print(",");                    // Separator for the Serial Plotter
    // Serial.println(filteredSensorValue);  // Filtered value from A5

    // Delay for a short period to control the frequency of the generated sine wave.
    // This delay is dynamically controlled by the potentiometer.
    delayMicroseconds(stepDelayMicroseconds);
  }
}
