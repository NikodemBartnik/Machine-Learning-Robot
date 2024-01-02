# Machine learning robot with Arduino and LIDAR
The idea for this project was born as I was learning about clasifiers at my university. Classes are very boring and usually that leads to poor understading of the topic so I decided to learn more on my own by implementing classifiers on Arduino UNO to build a robot that can autonomously drive in a race track based on LIDAR data. If you think that it sounds like an ambitious project. That's right. It definitely wasn't an easy one but at the same time it was so rewarding once it worked in the end.

Beofre diving into the code and all the files I recommend watching these two videos to understand how it works.

[![Machine Learning on Arduino Uno was a Good Idea](https://img.youtube.com/vi/PdSDhdciSpE/0.jpg)](https://www.youtube.com/watch?v=PdSDhdciSpE)

[![Machine Learning on Arduino Uno was a Good Idea](https://img.youtube.com/vi/KJIKexczPrU/0.jpg)](https://www.youtube.com/watch?v=KJIKexczPrU)

## How to build it

Project can be divided into a few steps:
- Building the robot - chassis was build accorind to [ORP design rules](https://openroboticplatform.com/designrules). All the files for the chassis can be found in the [ORP library](https://openroboticplatform.com/library).
- Collecting the data - the data I collected is included in the data folder but if you modify the project, want to use it ina  different application or you are using completely different sensors you will have to collect your own data. There are scripts for data collection for two different LIDARs in the arduino folder. To control the robots I used an app on Google Play called "Bluetooth RC controller" it looks like at the point of writing this it's not available anymore. So you would have to use something else and adjust the code to get it to work. I saved the data on the SD card as a txt file and later moved to the computer where I processed it with Python.
- Data processing and training the model - that is done with Python, there are plenty of libraries available so it's quite easy to do. In ML folder you can find Jupyter Notebook file with explonation of all the code. There is also simple main.py with just Python code if that's what you prefer.
- Uploading to Arduino - after going through the previous step you can will have an exported classifier as a header file. You need to move it to the folder with .ino file that you want to use and adjust the name of the header file in Arduino IDE. Remember to also copy the indices of selected LIDAR features.
- Testing - try to run your robot with the code uploaded. Be careful as it might not work the best right from the beginning, watch out for your robot and if it is still not able to drive on it's own it might require some more learning, data or better cleaning. 

There are additional readmes in each subfolder that explain everything so please take a look at that.

## Parts
To build the project you will need:
| Name | Amount | Link |
| --- | --- | ---|
| LIDAR | 1 | https://amzn.to/3H53CvK |
| Arduino UNO | 1 | https://amzn.to/3vne7YJ |
| TT motor | 2 | https://amzn.to/3tJCO0M |
| Motor driver | 1 | https://amzn.to/3THvldr |
| SD card module | 1 | https://amzn.to/3NNny9X |
| Breadboard | 1 | https://amzn.to/3NLGyWk |
| Bluetooth module | 1 | https://amzn.to/3RBvNXK |

All the parts required to build the chassis can be 3D printed and files can be found on [Open Robotic Platform website](https://openroboticplatform.com/). You can also DIY it or use any other chassis.

## Arduino code
Arduino programs were divided into 4 folders. There are two different versions of the code for data collection and classification and I used two different LIDARs so there is each version for both LIDARs. Delta 2A LIDAR is not available anymore so might be hard to buy. RPLidar is very popular and can be still bought in most robotics stores. 

## How to support my work
Makign all this projects, documentation and videos takes a lot of time and money. If you would like to support what I am doing you can do it on [Patreon](https://www.patreon.com/nikodembartnik) or through my [Indystry store](https://indystry.cc/store/). Even simply watching my videos helps a lot so subscribe to [my channel](https://www.youtube.com/@nikodembartnik) to don't miss any of them.