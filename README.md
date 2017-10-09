
CBMI-2017 [ACM paper](https://dl.acm.org/citation.cfm?id=3095748) - [Semi-automatic Video Assessment System](https://novasearch.org/w/images/1/11/Semiautomatic_video_assessment_system.pdf)

# Video Quality Assessment System

The availability of high-speed internet connections and the increasing rate of mobile phone usage all together combined with the advent of Social Media, created a growing stream of user generated content (UGC). 
The goal was to develop a simple system capable of semi-automatically assess quality from user generated  video. It uses image and video processing combined with a model that is in accordance with human visual attention and perception. The main purpose of the system is the sorting and filtering of a large stream of UGC video in a timely manner and in a way, that it can be manageable by a human operator.

Our aproach combines in a greaphical user interface, a balanced set of tools for video retrieval, to allow eficient video descrimination. It is available filtering and sorting operations based on a broad variety of visual properties and concepts that are very easy to be undestood by humans. there is a multiple feature indexing tool that allows to organize the videos by similarity to a reference. We also used Support Vector Machines to generate binary classifiers for video aesthetics and interestingness.

We populated this release with the UGC videos from the [CERTH-ITI-VAQ700](http://mklab.iti.gr/project/certh-iti-vaq700-dataset), a dataset composed by 700 YouTube videos licensed under Creative Commons Attribution. The duration of each of these videos ranges from 1 to 6 minutes. Because of the large size of the dataset(25.3GB) we only used 1 second of each video. To have the full dataset available, one can download it from the dataset homepage to the bin/data/files folder.

Detailed instructions on how to use the GUI and on how to load a new video repository are available in the included instructions file instructions.pdf

## Development setup

Windows
Visual Studio community edition 2015
openFrameworks 0.9.3
OpenCV 3.2.0 + contrib modules

The available binaries should run on any Windows 8-10. x64 machine. To run, just copy the bin folder to local machine and run the executable video-assessment.exe
The fastest way to solve missing video codecs error is to install [K-lite codec pack 12.3.0.](https://k-lite-mega-codec-pack.en.uptodown.com/windows/download/791979)

## Feature description

In this section we enumerate the visual features, for each one we explain its relevancy, from our point of view, and show the range for output values.

#### Focus

* focus - [0.0-1.0]

Based on a survey implementation of many focus measurement algorithms in ["Analysis of focus measure operators for shape from focus"](https://drive.google.com/file/d/0B6UHr3GQEkQwYnlDY2dKNTdudjg/view). We selected the OpenCV port of 'LAPV' Variance of Laplacian algorithm [Pech2000](http://decsai.ugr.es/vip/files/conferences/Autofocusing2000.pdf). These types of algorithms is used in camera auto-focus, and rely on edge detection since we can detect more edges in a focused image than in a blurred version of it. Returns  the maximum sharpness detected, which is a pretty good indicator of if a camera is in focus or not.

### Color moments

The basis of color moments lays in the assumption that the distribution of color in an image  can be interpreted as a probability distribution. Probability distributions are characterized by a number of unique moments (e.g. Normal distributions are differentiated by their mean and variance). It therefore follows that if the color in an image follows a certain probability distribution, the moments of that distribution can then be used as features to identify that image based on color. Color moments are scaling and rotation invariant, they are also a good feature to use under changing lighting conditions. We use the first two central moments of color distribution. They are the mean and standard deviation computed as explained in ["M. Stricker and M. Orengo Similarity of color images"](http://spie.org/Publications/Proceedings/Paper/10.1117/12.205308). We compute moments for each RGB channel.

Mean - The first color moment can be interpreted as the average color in the image.

* red_moment_1 - [0-255]
* green_moment_1 - [0-255]
* blue_moment_1 - [0-255]

Standard deviation - The second color moment which is obtained by computing the square root of the variance of the color distribution.

* red_moment_2 - [0-255]
* green_moment_2 - [0-255]
* blue_moment_2 - [0-255]

### Color ratio

Color moments are great as a measure to judge similarity between images or videos, but it's a concept hard to be directly interpreted by a human user, instead we devised a better feature based on the color average given by the first moment, the color ratio. For a human is much easier to understand and use this concept.

* red_ratio - [0.0-1.0]
* blue_ratio - [0.0-1.0]
* green_ratio - [0.0-1.0]

### Luminance

Luminance is arguable the most important component of color, humans are so much more sensitive to light than color information. We compute the first luminance moment.

* luminance - [0.0-1.0]

We also compute the luminance standard deviation. In terms of the human visual perception system the variance of luminance may be more informative than the mean or median.

* luminance_std - [0.0-1.0]

### Edges

Edges may indicate the boundaries of objects in the scene. Detecting sharp changes in brigthness between each pixel and its surroundings allow us to detect edges.

* EH_0 - [0,1,2,3,4,5]

...to...
 
* EH_15 - [0,1,2,3,4,5]

We implented a simplified version of the method presented in ["Efficient Use of Local Edge Histogram Descriptor"](https://www.dcc.fc.up.pt/~mcoimbra/lectures/VC_1415/VC_1415_P8_LEH.pdf) where edge histograms MPEG-7 compliant features are extracted. We use a global and a local representation for edges, for the local one a support tool was used to split each frame by a 4x4 grid, resulting in 16 sub-images. Each one of this sub-images was convolved with 5 different oriented kernels (0-zero, 1-vertical, 2-horizontal, 3-45ª, 4-135ª, 5-non-directional) this process results in 16 local features representing the orientation of each sub-image.

* EH_16 - [0,1,2,3,4,5]

The global edge histogram feature is computed using the full image.

#### Color diversity

Color diversity is a property related to visual aesthetics. We count different groups of hues. On the hue histogram (computed from the H channel of HSV) we count any variation of hue bigger than a certain threshold.

* dif_hues - [0-360]


### Object detection

We apply object Detection using Haar feature-based cascade classifiers, this method was originally proposed by Paul Viola and Michael J. Jones. We instantiate two cascade classifiers loading data from existing .xml classifier files, the first one is used for face detection, and the second can be used to load any existing classifiers, there are around 30 different human features classifiers in the folder (/bin/data/haar), we can find classifiers for features like eyes, mouth, nose and body among others.

* faces - images[0,1] , videos[0.0-1.0]

Faces capture attention, to the detriment of other visual stimuli, in a visual cueing paradigm, observers responded faster to a target probe appearing in the location of a face cue than of a competing object cue, [see](http://jov.arvojournals.org/article.aspx?articleid=2192946). This measure have a boolean value for images or a double value for videos, in the first case it signs if it was found or not faces in the image, in the second it represents the average number of video frames where faces were detected.

* faces_area - [0.0-1.0]

Ratio of face area(face bounding box) to full image area. If faces are found we measure their areas, we consider that preponderancy of attention faces can arouse is directly related to their visible area.

* smiles - [0.0-1.0]

When someone smiles he's normally seen as a person with positive intentions. Generally people tend to be attracted by smiling subjects. We do smile detection with cascade classifier to compute a smile measure.It represents average frames where smiles were found. Depending on the boolean parameter "insideFace" smiles can be detected inside or outside faces.

* rule_of_thirds - [0.0-1.0]

The rule of thirds is one of the usual guidelines used to assess quality of visual compositions, The image is divided in nine equal parts by four equal spaced lines(the power lines), two oriented horizontally and the other two vertically, important compositional elements should be placed along these lines or their intersections.
Like in [“Evaluating Visual Aesthetics in Photographic Portraiture”](https://ai2-s2-pdfs.s3.amazonaws.com/ab62/68d290b23fee36c3faf1ae77f0fd900a225f.pdf) we use a Spatial composition template to score images according to a variation of the rule of thirds where more power points were added, the template is in grayscale format, afterwards we compare face centroids from the previous step of the pipeline with this template and assign a score ranging from 0-255, this value is afterward normalized to the [0.1-1.0] range.

### Saliency

Topics related to saliency were adapted from the related OpenCV [documentation](http://docs.opencv.org/3.0-beta/modules/saliency/doc/saliency.html). Many computer vision applications may benefit from understanding where humans focus given a scene. Other than cognitively understanding the way human perceive images and scenes, finding salient regions and objects in the images helps various tasks such as speeding up object detection, object recognition, object tracking and content-aware image editing.

* static_saliency[0.0-1.0]

Algorithms belonging to this category, exploit different image features that allow to detect salient objects in a non dynamic scenario. We experiment with both approaches for static saliency offered by the OpenCV 3.2 Saliency API:

** SpectralResidual - Starting from the principle of natural image statistics, this method simulate the behavior of pre-attentive visual search. The algorithm analyzes the log spectrum of each image and obtain the spectral residual. Then transform the spectral residual to spatial domain to obtain the saliency map, which suggests the positions of proto-objects.
based on [“Saliency detection: A spectral residual approach.”](https://www.researchgate.net/publication/221364530_Saliency_Detection_A_Spectral_Residual_Approach).

** FineGrained - This method calculates saliency based on center-surround differences. High resolution saliency maps are generated in real time by using integral images. Based on [Human detection using a mobile platform and novel features derived from a visual saliency mechanism](http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=4CC3F597B5F4F9EC4E4D71817098C3E2?doi=10.1.1.700.4706&rep=rep1&type=pdf).

After computing a saliency map with one of the above methods, we calculate the mean pixel value of this saliency map to give a rough measure of its strength, the value is normalized dividing it by 255.

### Optical flow

Optical flow is the pattern of apparent motion of image objects between two consecutive frames caused by the movement of an object or camera. It is a 2D vector field where each vector is a displacement vector showing the movement of points from first frame to second.
Optical flow works on two main assumptions: the pixel intensities of an object do not change between consecutive frames and neighboring pixels have similar motion.
OpenCV provides an algorithm to find dense optical flow. It computes the optical flow for all the points in the frame. It is based on Gunner Farneback's algorithm which is explained in ["Two-Frame Motion Estimation Based on Polynomial Expansion"](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.102.2455&rep=rep1&type=pdf) by Gunner Farneback in 2003.
For the sake of performance, we compute motion vectors in a 5 by 5 grid, this way we only have to deal with a fraction  of around 1/25 of total pixels present in the image.
After we compute the absolute total flow and signed total flow per frame used to compute the total video flow and average flow per frame.
We also measure the optical flow only on a border of the frame so the main subject movement don't interfere with our angle measures to increase the accuracy of shake detection.

* shackiness [0.0-1.0]

We compare the angle between two subsequent motion vectors(computed from three consecutive video frames), if the angle is above a certain threshold(90º) we mark the transition between frames as "shaky", then compute the ratio between this "shaky" transitions and the total frames of the video.

* motion_mag [0.0-1.0]

Motion magnitude is the length of the average motion vector, we can use it as a global measure of change introduced by motion  between two frames of video.

### Background subtraction

Background subtraction or Foreground Detection  is a technique where an image's foreground is extracted for further processing. It is widely used for detecting moving objects in videos or cameras. The rationale in the approach is that of detecting the moving objects from the difference between the current frame and a reference frame, often called “background image”, or “background model”. Background subtraction is generally based on a static background hypothesis which is often not applicable in real environments. With indoor scenes, reflections or animated images on screens lead to background changes. In a same way, due to wind, rain or illumination changes brought by weather, static backgrounds methods have difficulties with outdoor scenes.

From the video analysis [section](http://docs.opencv.org/3.0-beta/modules/video/doc/video.html) of OpenCV, we use the class BackgroundSubtractor, included in the Motion Analysis and Object Tracking algorithms. The class is only used to define the common interface for the whole family of background/foreground segmentation algorithms from wich we use two specific algorithms that take shadows in account, a shadow is detected if a pixel is a darker version of the background. See ["Detecting Moving Shadows: Algorithms and Evaluation"](http://escholarship.org/uc/item/2kj2g2f7).

** BackgroundSubtractorKNN - The class implements the K-nearest neigbours background subtraction described in [“Efficient Adaptive Density Estimation per Image Pixel for the Task of Background Subtraction”](http://www.zoranz.net/Publications/zivkovicPRL2006.pdf). Very efficient if number of foreground pixels is low.

** BackgroundSubtractorMOG2 - The class implements the Gaussian mixture model background subtraction described in [“Improved adaptive Gausian mixture model for background subtraction”](http://www.zoranz.net/Publications/zivkovic2004ICPR.pdf) , and [“Efficient Adaptive Density Estimation per Image Pixel for the Task of Background Subtraction”](http://www.zoranz.net/Publications/zivkovicPRL2006.pdf).

* fg_area [0.0 – 1.0]

Foreground area 

* shadow_area [0.0 – 1.0]

When we know the location of a light source and see objects casting shadows on other objects, we learn that the object shadowing the other is closer to the light source. As most illumination comes downward we tend to resolve ambiguities using this information [See](http://www.hitl.washington.edu/research/knowledge_base/virtual-worlds/EVE/III.A.1.c.DepthCues.html).

* bg_area [0.0 – 1.0]

background area 

* camera_move [0.0 – 1.0]

Percentage of frames where foreground area is bigger then 80% of total area available, this indicates high probability of camera movement occurance.

* focus_diff [0.0 – 1.0]

Difference of focus between foreground and background. We try to detect low depth of field, the region of interest should be in sharp focus and the background blurred, this is a attribute that can boost aesthetic perceived from media or attention arouse.

#### Entropy

* entropy [0.0 – 1.0]

Entropy is a quantity normally used to describe the business of an image, in other words, the volume of data that needs to be encoded by a compression algorithm. A completly flat image, without any proeminent pixel have zero entropy and can be compressed to a very small size. On the other side an image with high entropy, full of details and pixel contrast will result in a very large encoded file size.


### Machine learning classification

We use SVM classifiers to predict aesthetic and interestingness from video, the classifiers were based on two video datasets:

#### CERTH-ITI-VAQ700
A comprehensive  [video dataset](http://mklab.iti.gr/project/certh-iti-vaq700-dataset) for the problem of aesthetic quality assessment with annotated scores for 700 (UGC)videos from YouTube, 350 videos are rated as being of high aesthetic quality and another 350 as being of low aesthetic quality.We extracted our set of features from the dataset and using the ground truth experimental results [paper](https://www.researchgate.net/profile/Christos_Tzelepis/publication/307516039_Video_aesthetic_quality_assessment_using_kernel_Support_Vector_Machine_with_isotropic_Gaussian_sample_uncertainty_KSVM-IGSU/links/57d9074a08ae601b39b04749/Video-aesthetic-quality-assessment-using-kernel-Support-Vector-Machine-with-isotropic-Gaussian-sample-uncertainty-KSVM-IGSU.pdf) generated a classifier to predict aesthetic of any new sample.

#### Video Interestingness Database (VID)
Two benchmark datasets with ground-truth interestingness labels [more info](http://www.yugangjiang.info/research/interestingness/index.html). The first one (V.I.D. dataset A) consists in 1200 videos collected from Flickr which has a rank based on interestingness. The second (V.I.D. dataset B) consisted of 420 advertisement videos from YouTube. YouTube does not have an interestingness rank so to collect the interestingness scores, this dataset was subject to an experimental annotation procedure. We extracted our set of features from the (V.I.D. dataset A) together with the experimental binary values we generate a SVM classifier for interestingness on video.