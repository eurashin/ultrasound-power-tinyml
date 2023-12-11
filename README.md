## Directory guide
* `data/` is a folder that contains our 210 activity instances. 
    * `original/` is the originally collected dataset, which contains labels for all 7 classes (walking, running, stairs, walking-disabled, other, sitting, pushing). The data is already split into train and test sets via the `original/info.labels` file. DO NOT change this file. 
    * `relabel.py` is code that takes in the original data and outputs a new directory which contains relabled data. For example, `stationary-or-moving/` is an output from running `python relabel.py`
    * `stationary-or-moving/` is the dataset that contains the 210 activity instances, labeled as either `stationary` or as `moving`
* `sketch_model_light_battery/` is a directory that contains the Arduino code. 
    * `sketch_model_light_battery.ino` is the Arduino code that loads the activity recognition model onto the Nicla Vision. It is written so that the Nicla can predict activity even when connected to an external power source and output. 
    * `TinyML-final-project_inferencing.h` contains the activity recognition model
* `plot-hyperparameters.py` is plotting code used to generate Fig. 3 of our report. 
* `results_data.py` is the results used to generate Fig. 3 of our report. 


## Instructions for running
**To run ML inference on the Nicla Vision**:
1. Make sure your nicla vision is connected
2. Open sketch_model_light_battery/sketch_model_light_battery.ino with our arduino ide
3. Compile and run
4. Unplug the Nicla Vision. Connect it to the external battery
5. Your model should still be running (the blue LED will light when motion is detected)

**To relabel the data into the classes of interest**: 
In `relabel.py`, change the `mapping` variable to map from the original label to your new labels of interest. For example, to create binary labels of `stationary` vs `moving`, you may specify the following mapping: 

`mapping = {'sitting': 'stationary', 'walking': 'moving', 'running':'moving', 'stairs':'moving', 'pushing': 'moving', 'walkingdisabled':'moving', 'other':'stationary'}`

Change the `dirname` parameter to the name of the directory you want the new data with labels to write to. 

Finally, run `relabel.py` to make your new dataset. 

In the Edge Impulse interface, import a new dataset by selecting a directory; this should be the directory that you generated.

**To reproduce fig 3**:
Please run `python plot-hyperparameters.py -d <dataset>`. There `-d` flag can either be `stairs`, `sitting`, or `all`. 

## Hardware setup
A diagram of the recommended hardware setup is below. The system should be made up of 1) a device capable of running Windows with the ultrasound engine software on it; 2) the ultrasound engine with compatible transducer; 3) the Arduino Nicla Vision; 4) a battery pack powering these devices. Nicla pins 5 (D3) and 6 (GND) should be wired to ultrasound system Mini HDMI port pin 15 (SYNC IN) and 20 (GND), respectively. 

![hardware_setup](us_overview.png)
