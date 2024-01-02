# About the data
This is the data I collected from various racetracks in my ([first video](https://www.youtube.com/watch?v=PdSDhdciSpE)), where the primary focus was on enabling a single robot to drive autonomously on the racetrack. Each file represents a data collection attempt, comprising 10-30 minutes of driving on the racetrack. Most files include mixed data from both clockwise and counterclockwise driving. The data is stored in CSV format within text (TXT) files. While driving at a rate of 5 times per second, the distances from the LIDAR were recorded, along with a letter representing the current control command sent from the remote control app. Each data point was saved as a new line, and there are only five possible letters:

- F - forward
- I - forward right
- R - right
- G - forward left
- L - left

During the training phase, I excluded all samples with "R" and "L" to simplify the problem. Consequently, I utilized only samples with "F," "I," and "G."

The "racing" folder contains data collected during the creation of the ([second video](https://www.youtube.com/watch?v=KJIKexczPrU)), where I attempted to make two robots race on a track. One robot was slower, and the other was faster, with the objective of teaching the faster one to overtake without colliding with the slower robot.
File racing_all.txt is a combination of all the files from the racing stage. FIle all.txt is a combination of all the data collected in the first and the second video. This file was used to train the final model used for overtaking.
