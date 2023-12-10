#include <TinyML-final-project_inferencing.h>
// Libraries for reading sensors 
#include <Arduino_LSM6DSOX.h> //Click here to get the library: http://librarymanager/All#Arduino_LSM6DSOX


#define CONVERT_G_TO_MS2    9.80665f
#define FREQUENCY_HZ        50
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

float Ax, Ay, Az;
float Gx, Gy, Gz;
String target = String("moving"); // change to target class
float target_value; 

static unsigned long last_interval_ms = 0;

void output_inference_result(ei_impulse_result_t result);

void setup()
{
  // put your setup code here, to run once:
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT); 
  
  digitalWrite(LEDB, HIGH); 
  digitalWrite(LEDG, HIGH); 
  digitalWrite(LEDR, HIGH); 

  pinMode(D3, OUTPUT); 
  digitalWrite(D3, LOW); 

  // Initialize IMU
  if (!IMU.begin()) {
        // light red LED if IMU fails to initialize
        digitalWrite(LEDR, LOW);
        while (1);
    }
}


void loop() {
  // Sampling 

  // reading into buffer
  // Allocate a buffer here for the values we'll read from the sensor
  float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };
  for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME) {
    // Determine the next tick (and then sleep later)
    int64_t next_tick = (int64_t)micros() + ((int64_t)EI_CLASSIFIER_INTERVAL_MS * 1000);

    IMU.readAcceleration(Ax, Ay, Az);
    IMU.readGyroscope(Gx, Gy, Gz);
    buffer[ix + 0] = Ax; 
    buffer[ix + 1] = Ay; 
    buffer[ix + 2] = Az; 
    buffer[ix + 3] = Gx; 
    buffer[ix + 4] = Gy; 
    buffer[ix + 5] = Gz; 

    int64_t wait_time = next_tick - (int64_t)micros();

    if(wait_time > 0) {
        delayMicroseconds(wait_time);
    }
  }

  // Turn the raw buffer into a signal which we can the classify
  signal_t features_signal;
  int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &features_signal);
  if (err != 0) {
      // if error, light red LED
      digitalWrite(LEDR, LOW); 
      return;
  }  

  // Classifying
  ei_impulse_result_t result = { 0 }; 
  // invoke the impulse
  EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false );
  if (res != EI_IMPULSE_OK) {
      // light red LED if classifier failed
      digitalWrite(LEDR, LOW); 
      return;
  }
  output_inference_result(result);
  
  delay(1000);  
}


void output_inference_result(ei_impulse_result_t result) {
  for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
      // if the target catagory name matches the set target, save value
      if (ei_classifier_inferencing_categories[i] == target){
        target_value = result.classification[i].value;
      }
  }
  
  if (target_value > 0.5){ // target activity detected
    // turn LEDs on
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);

    // tell US system to collect 5 s of data
    int starttime = micros();
    int endtime = starttime + 5000000; 
    int currtime = micros();
    // send pwm using bit banging (base library pwm functions do not work)
    while(currtime < endtime){ 
      digitalWrite(D3, HIGH);
      delayMicroseconds(6250); 
      digitalWrite(D3, LOW);
      delayMicroseconds(12500 - 6250); // Approximately 50% duty cycle @ 100Hz
      currtime = micros();
    }

    // turn LEDs off and set output pin low
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(D3, LOW); 
  }
  else{ // target activity not detected. 
    // keep LEDs off and set output pin low
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(D3, LOW); 
  }
  
  #if EI_CLASSIFIER_HAS_ANOMALY == 1
    // If anomaly, light red LED
        digitalWrite(LEDR, LOW); 
  #endif
}
