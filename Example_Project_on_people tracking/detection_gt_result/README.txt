1. Detection & Ground Truth

There are two detection files for each sequence. One is from OpenCV's HOG pedestrian detector, the other is from the link (http://www.gris.informatik.tu-darmstadt.de/~aandriye/data.html) for PETS and the link (http://www.robots.ox.ac.uk/ActiveVision/Research/Projects/2009bbenfold_headpose/project.html) for Town Center. The ground truth files are from the above links too. If you use their ground truth annotations and detection files, please cite their papers.


2. Tracking Results

There are also two versions of tracking results for each sequence corresponding to the two detections, as their names indicate. The tracking and detection xml file follow the same structure, as you can see in the files.


3. Aspect Ratio and Scale

The tracking results are directly produced by our tracking system. When compared with the ground truth, the tracking bounding boxes may suffer from the systematic unfitness in aspect ratio and scale. Therefore, we rescaled the tracking bounding boxed during evalution. The scale factors we  use in the evaluation are as follows.

a) OpenCV detector

PETS: 		width*0.9, height*1.2
Town Center	width*1.0, height*1.3

b) The other detector

PETS:		width*0.8, height*1.05
Town Center	width*1.0, height*1.0


4. Evaluation Code

The MOT_Clear.rar is the code for computing the MOT metric. You may specify a path for the sequence (e.g. PETS09S2L1) for function "calMOT()" so that the matching results will be displayed frame by frame.

You need opencv2.3+ and libxml installed and compile with code attached


5. Other Issues

The ground truth annotation for PETS09S2L1 does not cover some border areas (left side and upper right corner). However, our tracking results are for the whole area. For a fair comparison, we simply excluded trajectorys in those areas in our evaluation.

Tracking windows are condered to be matched when they overlap. In other words, their IOU (intersection over union) should be greater than 0. We used the same protocol when we did the comparison with other systems.

