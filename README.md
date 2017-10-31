
CBMI-2017 [ACM paper](https://dl.acm.org/citation.cfm?id=3095748) - [Semi-automatic Video Assessment System](https://novasearch.org/w/images/1/11/Semiautomatic_video_assessment_system.pdf)

# Semi-automatic Video Assessment System

## Introduction

The goal is the development of a system capable of semi-automatically assess video quality, to sort and filter a large stream of user generated content (UGC) in a timely manner and in a way, that it can be manageable by a human operator.

![figure 1](/images/overview.png)
*figure 1 - System overview* 

Video quality assessment (VQA) combines existing models for human visual perception and attention with more pragmatic methods based on visual feature extraction. This assessment can have an objective or subjective nature and, until recently, was focused on professional captured media. In the perspective of VQA, UGC is different in many ways from traditional media, where fidelity is of the utmost importance and mean opinion score (MOS) is the standard for comparing quality. There is nowadays a massive volume of UGC that needs to be assessed, fidelity is less important, and the criterion that defines quality becomes more personalized, focused on the group or user. As consumers are also becoming creators of content, it makes sense to combine UGC captured on large scale events in their live conventional broadcast. This inclusion could enhance the quality of Experience (QoE) provided that at the same time high standards for content quality are maintained. 
On an initial stage, it was performed an extensive literature study to collect a set of relevant visual features in terms of human visual perception and how it discriminates visual quality, it was used the OpenCV library of computer vision algorithms as a basis to compute visual only features focused mainly on subjective criteria like interestingness and attention arouse or aesthetic perceived from video. These features were tested in terms of performance and usability using a graphical user interface (GUI). A subset of these features was also selected and used to create machine learning binary classifiers for aesthetics and interestingness. It was also developed and tested in the GUI a video indexing tool, that allows to index and sort videos by a multiple feature similarity index. It is possible to see in [figure 1]() an overview.

Afterwards, it was necessary to integrate these concepts into an extended infrastructure to offer visual quality assessment to applications. 

Finally, a stand-alone graphical application([figure 2]()) was developed that besides making the extraction of features provides tools with which is possible to quickly differentiate quality on videos, included in large collections.

We populated this release with the UGC videos from the [CERTH-ITI-VAQ700](http://mklab.iti.gr/project/certh-iti-vaq700-dataset), a dataset composed by 700 YouTube videos licensed under Creative Commons Attribution. The duration of each of these videos ranges from 1 to 6 minutes. Because of the large size of the dataset(25.3GB) we only used 1 second of each video. To have the full dataset available, one can download it from the dataset homepage to the bin/data/files folder.

![figure 2](/images/0.png)
*figure 2 - Graphical interface* 

Detailed instructions on how to use the GUI and on how to load a new video repository are available in the [instructions file](instructions.md)

## Development setup

* Windows 10
* Visual Studio community edition 2015
* openFrameworks 0.9.3
* OpenCV 3.2.0 + contrib modules

In this project github repository there is a Visual Studio 2015 Project. One can either compile it from source or just copy and paste the binaries folder. The available binaries should run on any Windows 8-10. x64 machine. To run, just copy the bin folder to local machine and run the executable video-assessment.exe

## Feature description

In this section we enumerate the visual features, for each one we explain its relevancy, from our point of view.

![figure 3](/images/diag.png)
*figure 3 - Feature extraction process*

### Attention features

Measures related to attention can be used to improve user QoE in many applications. In the remainder of this section are enumerated visual features in some way related to attention, and for each one explained its relevancy, from our point of view. It was used a top-down approach for feature extraction. It is possible to see an overview in [figure 3](), resulting in a small, off the shelf, set of visual features able to capture, at some extent, important aspects of the interestingness, attention and visual aesthetic concepts. Interestingness is an important visual property, as the human vision system (HVS) searches actively for regions of interest to improve the efficiency of subsequent object detection and recognition tasks and is also a precursor to attention. Interestingness literally means something that arouses curiosity.

#### Focus

Focus is very related to sharpness, in an image that is completely in-focus we can easily perceive fully its content, on the other hand it is harder for our perception system to process a blurry image thus, possibly, hampering overall attention arouse. Based on a survey implementation of many focus measurement algorithms in [[1]](https://drive.google.com/file/d/0B6UHr3GQEkQwYnlDY2dKNTdudjg/view) we selected the OpenCV port of 'LAPV' Variance of Laplacian algorithm [[2]](http://decsai.ugr.es/vip/files/conferences/Autofocusing2000.pdf). These types of algorithms are used in camera auto-focus, and rely on edge detection since we can detect more edges in a focused image than in a blurred version of it. It returns the maximum sharpness detected, which is a good indicator of if a camera is in focus or not.

#### Faces

Faces capture attention, to the detriment of other visual stimuli. There are [evidences](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC1857737/)  from neuropsychology and physiology that there are specialized areas for face perception in the human brain. In a [visual cueing paradigm](http://jov.arvojournals.org/article.aspx?articleid=2192946), observers responded faster to a target appearing in the location of a face cue than of an object cue. The capacity to detect and recognize faces in milliseconds is very important, as it played a preponderant role in the survival of our ancestors. It was computed the average number of video frames where faces were detected.

#### Face area

It is the ratio of face area (area of face bounding box) in relation to the full image area. If faces are found we measure their areas, we considered that the preponderancy of attention faces can arouse is directly related to their visible area size.

#### Smiles

When someone smiles he/she is normally seen as a person with positive intentions. Generally, people tend to be attracted by smiling subjects. We do smile detection with a cascade classifier to compute a smile measure. It represents average frames where smiles were found. Depending on the Boolean parameter "inside Face" smiles can be detected inside or outside faces.

#### static saliency

Algorithms belonging to this category, exploit different image features that allow to detect salient objects in a non-dynamic scenario. We experiment with both approaches for static saliency offered by the [OpenCV 3.2 Saliency API](https://docs.opencv.org/3.2.0/d8/d65/group__saliency.html).
- Spectral Residual [[3]](https://www.researchgate.net/publication/221364530_Saliency_Detection_A_Spectral_Residual_Approach) - Starting from the principle of natural image statistics, this method simulates the behavior of pre-attentive visual search. The algorithm analyzes the log spectrum of each image and obtain the spectral residual. Then transform the spectral residual to the spatial domain to obtain the saliency map, which suggests the positions of proto-objects.
- Fine Grained [[4]](http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=4CC3F597B5F4F9EC4E4D71817098C3E2?doi=10.1.1.700.4706&rep=rep1&type=pdf) - This method calculates saliency based on center-surround differences. High resolution saliency maps are generated in real time by using integral images. 
After computing a saliency map with one of the above methods, we calculate the mean pixel value of this saliency map to give a rough measure of its strength, afterwards the value is normalized.

#### Focus difference

From the background subtraction category of algorithms, it represents the difference of focus between foreground and background. We try to detect low depth of field, where the region of interest is in focus and the background blurred. This is an attribute that can boost aesthetic perceived from media or attention arouse and in cinema and photography is commonly used to direct attention from the audience or emphasize visual depth.

#### Motion magnitude

Motion is an important cue for attention deployment. Motion magnitude is the computed length of the average motion vector, and we can use it as a global measure of change introduced by motion between two frames of video. 

#### Shakiness

It is based on the comparison of the angle between two subsequent motion vectors (computed from three consecutive video frames). If the angle is above a certain threshold (45º) the transition between frames is marked as "shaky", then is computed the ratio between these "shaky" transitions and the total frames of the video.

#### Objective index

The frame rate and resolution of video were used to compute a simple objective quality measure.

#### Binary classification for interestingness

It is based on the Video Interestingness Database (VID), two benchmark [datasets](http://www.yugangjiang.info/research/interestingness/index.html)  with ground-truth interestingness labels. The first one (V.I.D. dataset A) comprises 1200 videos collected from Flickr which have a rank based on interestingness. The second (V.I.D. dataset B) comprises 420 advertisement videos from YouTube. YouTube does not have an interestingness rank so to collect the interestingness scores, this dataset was subject to an experimental annotation procedure. It was extracted an extended set of features, including the before mentioned related to attention and others related to aesthetics, from the (V.I.D. dataset A). These features together with the experimental binary values were used to train SVM classifiers for interestingness on video.

### Aesthetic features

Aesthetics is not only a cue for interestingness but also closely related to visual attention and the way images or videos appeal to humans.

#### Color moments

The basis of color moments lays in the assumption that the distribution of color in an image  can be interpreted as a probability distribution. Probability distributions are characterized by a number of unique moments (e.g. Normal distributions are differentiated by their mean and variance). It therefore follows that if the color in an image follows a certain probability distribution, the moments of that distribution can then be used as features to identify that image based on color. Color moments are scaling and rotation invariant, they are also a good feature to use under changing lighting conditions. We use the first two central moments of color distribution. They are the mean and standard deviation computed as explained in ["M. Stricker and M. Orengo Similarity of color images"](http://spie.org/Publications/Proceedings/Paper/10.1117/12.205308). We compute moments for each RGB channel.

* Mean - The first color moment can be interpreted as the average color in the image.

* Standard deviation - The second color moment which is obtained by computing the square root of the variance of the color distribution.

#### Color ratio

Color moments are great as a measure to judge similarity between images or videos, but it's a concept hard to be directly interpreted by a human user, instead we devised a better feature based on the color average given by the first moment, the color ratio. For a human is much easier to understand and use this concept.


#### Luminance

Luminance is arguable the most important component of color, humans are so much more sensitive to light than color information. We compute the first luminance moment.

We also compute the luminance standard deviation. In terms of the human visual perception system the variance of luminance may be more informative than the mean or median.

#### Edge orientation

Edges may indicate the boundaries of objects in the scene. Detecting sharp changes in brigthness between each pixel and its surroundings allow us to detect edges.

We implented a simplified version of the method presented in ["Efficient Use of Local Edge Histogram Descriptor"](https://www.dcc.fc.up.pt/~mcoimbra/lectures/VC_1415/VC_1415_P8_LEH.pdf) where edge histograms MPEG-7 compliant features are extracted. We use a global and a local representation for edges, for the local one a support tool was used to split each frame by a 4x4 grid, resulting in 16 sub-images. Each one of this sub-images was convolved with 5 different oriented kernels (0-zero, 1-vertical, 2-horizontal, 3-45ª, 4-135ª, 5-non-directional) this process results in 16 local features representing the orientation of each sub-image. A global edge value is also computed.

#### Edge strenght

The amount of edges detected.

#### Color diversity

Color diversity is a property related to visual aesthetics. We count different groups of hues. On the hue histogram (computed from the H channel of HSV) we count any variation of hue bigger than a certain threshold.

#### Rule of thirds

The rule of thirds is one of the usual guidelines used to assess quality of visual compositions, the image is divided in nine equal parts by four equal spaced lines (the power lines), two oriented horizontally and the other two vertically, important compositional elements should be placed along these lines or their intersections. Like in ["Evaluating Visual Aesthetics in Photographic Portraiture"](https://ai2-s2-pdfs.s3.amazonaws.com/ab62/68d290b23fee36c3faf1ae77f0fd900a225f.pd) we use a Spatial composition template to score images according to a variation of the rule of thirds where more power points were added, the template is in grayscale format, afterwards we compare face centroids from the previous step of the pipeline with this template and assign a score ranging from 0-255, this value is afterward normalized.

#### Foreground area

The relative size of the area of the foreground detected.

#### Shadow area

When we know the location of a light source and see objects casting shadows on other objects, we learn that the object shadowing the other is closer to the light source. As most illumination comes downward we tend to resolve ambiguities using this information, [See](http://www.hitl.washington.edu/research/knowledge_base/virtual-worlds/EVE/III.A.1.c.DepthCues.html).

#### Camera move

Percentage of frames where foreground area is bigger then 80% of total area available, this indicates high probability of camera movement occurance.

#### Entropy

Entropy is a quantity normally used to describe the business of an image, in other words, the volume of data that needs to be encoded by a compression algorithm. A completly flat image, without any proeminent pixel have zero entropy and can be compressed to a very small size. On the other side an image with high entropy, full of details and pixel contrast will result in a very large encoded file size.

#### Binary classification for aesthetics

  We used the CERTH-ITI-VAQ700 dataset, a comprehensive [video dataset](http://mklab.iti.gr/project/certh-iti-vaq700-dataset) for the problem of aesthetic quality assessment with annotated scores for 700 (UGC)videos from YouTube, 350 videos are rated as being of high aesthetic quality and another 350 as being of low aesthetic quality. We extracted our set of features from the dataset and using the ground truth experimental results generated a classifier to predict aesthetic of any new sample.


## Algorithm categories

### Object detection

We apply object Detection using Haar feature-based cascade classifiers. This method was originally proposed by Paul Viola and Michael J. Jones. We instantiate two cascade classifiers loading data from existing .xml classifier files, the first one is used for face detection, and the second can be used to load any existing classifiers. There are around 30 different human features classifiers in the folder (/bin/data/haar), and it is possible to find classifiers for features like eyes, mouth, nose and body among others.

### Background Subtraction

Background subtraction or Foreground Detection is a technique where an image's foreground is extracted for further processing. It is widely used for detecting moving objects in videos or cameras. The rationale in the approach is that of detecting the moving objects from the difference between the current frame and a reference frame, often called “background image”, or “background model”. Background subtraction is generally based on a static background hypothesis which is often not applicable in real environments. With indoor scenes, reflections or animated images on screens lead to background changes. In a same way, due to wind, rain or illumination changes brought by weather, static backgrounds methods have difficulties with outdoor scenes.
From the [video analysis](http://docs.opencv.org/3.0-beta/modules/video/doc/video.html) section of OpenCV, we use the class BackgroundSubtractor, included in the Motion Analysis subcategory. The class is only used to define the common interface for the whole family of background/foreground segmentation algorithms from which we use two specific algorithms that take shadows in account [[5]](http://escholarship.org/uc/item/2kj2g2f7), a shadow is detected if a pixel is a darker version of the background.

* Background Subtractor KNN - The class implements the K-nearest neigbours background subtraction described in [[6]](http://www.zoranz.net/Publications/zivkovicPRL2006.pdf). Very efficient if number of foreground pixels is low.
* Background Subtractor MOG2 - The class implements the Gaussian mixture model background subtraction method described in [[7]](http://www.zoranz.net/Publications/zivkovic2004ICPR.pdf) , and [[6]](http://www.zoranz.net/Publications/zivkovicPRL2006.pdf).

### Optical flow

Optical flow is the pattern of apparent motion of image objects between two consecutive frames caused by the movement of an object or camera. It is a 2D vector field where each vector is a displacement vector showing the movement of points from first frame to second. Optical flow works on two main assumptions: the pixel intensities of an object do not change between consecutive frames and neighboring pixels have similar motion. OpenCV provides an algorithm to find dense optical flow. It computes the optical flow for every frame. It is based on Gunner Farneback's algorithm which is explained in [[8]](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.102.2455&rep=rep1&type=pdf). 

### Saliency

Topics related to saliency were adapted from the related OpenCV documentation. Many computer vision applications may benefit from understanding where humans focus given a scene. Other than cognitively understanding the way human perceive images and scenes, finding salient regions and objects in the images helps various tasks such as speeding up object detection, object recognition, object tracking and content-aware image editing.

### Machine learning classification

It was trained SVM binary classifiers, using our extracted features, to predict aesthetic and interestingness from video. The classifiers are based on two publicly available video datasets with ground truth data. One for aesthetic  concepts where the dataset was used with a scheme that exploits visual properties and movement to assess the aesthetic quality of video. Other for interestingness , used in the development of a computational system to compare the interestingness levels of videos. For more details please refer to the corresponding papers [[9]](https://www.researchgate.net/profile/Christos_Tzelepis/publication/307516039_Video_aesthetic_quality_assessment_using_kernel_Support_Vector_Machine_with_isotropic_Gaussian_sample_uncertainty_KSVM-IGSU/links/57d9074a08ae601b39b04749/Video-aesthetic-quality-assessment-using-kernel-Support-Vector-Machine-with-isotropic-Gaussian-sample-uncertainty-KSVM-IGSU.pdf) and [[10]](http://www.yugangjiang.info/publication/aaai13-interestingness.pdf).

## References

- [1] 	D. P. A. G. Said Pertuz, "Analysis of focus measure operators for shape-from-focus," Pattern Recognition, vol. 46, no. 5, pp. 1415-1432, 2013. 
- [2] 	G. C. C. F. V. J.L. Pech Pacheco, "Diatom autofocusing in brightfield microscopy: a comparative study," in 15th International Conference on Pattern Recognition, 2000. 
- [3] 	X. Z. Hou, "Saliency Detection: A Spectral Residual Approach," in IEEE Conference on Computer Vision and Pattern Recognition , 2007. 
- [4] 	A. S. A. Sebastian Montabone A, "Human Detection Using a Mobile Platform and Novel Features Derived From a Visual Saliency Mechanism," Image and Vision Computing, vol. 28, no. 3, pp. 391-402, 2010. 
- [5] 	A. Prati, I. Mikic, M. M. Trivedi and R. Cucchiara, "Detecting moving shadows: algorithms and evaluation," IEEE Transactions on Pattern Analysis and Machine Intelligence , vol. 25, no. 7, pp. 918 - 923 , 2003. 
- [6] 	F. H. Z. Zivkovic, "Efficient adaptive density estimation per image pixel for the task of background subtraction," Pattern Recognition Letters, vol. 27, no. 7, pp. 773-780, 2006. 
- [7] 	Z. Zivkovic, "Improved adaptive Gaussian mixture model for background subtraction," in Proceedings of the 17th International Conference on Pattern Recognition, ICPR, 2004. 
- [8] 	G. Farnebäck, "Two-Frame Motion Estimation Based on Polynomial Expansion," in 13th Scandinavian Conference, SCIA 2003 , Halmstad, 2003. 
- [9] 	E. M. V. M. I. P. C. Tzelepis, "Video Aesthetic Quality Assessment using Kernel Support Vector Machine with Isotropic Gaussian Sample Uncertainty (KSVM-iGSU)," in IEEE International Conference on Image Processing(ICIP), Phoenix, Arizona, USA, 2016. 
- [10] 	Y. W. ,. R. F. ,. X. X. ,. Y. Z. ,. H. Y. Y.Jiang, "Understanding and predicting interestingness of videos," in AAAI, Conference on Artificial Intelligence, Washington, 2013. 