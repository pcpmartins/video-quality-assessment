
CBMI-2017 [ACM paper](https://dl.acm.org/citation.cfm?id=3095748) - [Semi-automatic Video Assessment System](https://novasearch.org/w/images/1/11/Semiautomatic_video_assessment_system.pdf)

# Semi-automatic Video Assessment System

**Table of Contents**

- [Introduction](#introduction)
- [Development setup](#development-setup)
- [Visual features extraction](#visual-features-extraction)
	- [Aesthetic related features](#aesthetic-related-features)
		- [Colour moments](#colour-moments)
		- [Colour ratio](#colour-ratio)
		- [Luminance](#luminance)
		- [Edge orientation and strength](#edge-orientation-and-strength)
		- [Colour diversity](#colour-diversity)
		- [Colourfulness](#colourfulness)
		- [Dominat colours](#dominant-colours)
		- [Rule of thirds](#rule-of-thirds)
		- [Foreground area](#foreground-area)
		- [Shadow area](#shadow-area)
		- [Camera moves](#camera-moves)
		- [Entropy](#entropy)
	- [Attention related features](#attention-related-features)
		- [Focus](#focus)
		- [Faces](#faces)
		- [Face area](#face-area)
		- [Smiles](#smiles)
		- [static saliency](#static-saliency)
		- [Focus difference](#focus-difference)
		- [Motion magnitude](#motion-magnitude)
		- [Shakiness](#shakiness)
		- [Objective index](#objective-index)
- [Semantic features extraction](#semantic-features-extraction)
- [Audio features extraction](#audio-features-extraction)
	- [Low level](#lowlevel)
	- [Rythm](#rythm)
	- [Tonal](#tonal)
- [Algorithm categories](#algorithm-categories)
	- [Object detection](#object-detection)
	- [Background Subtraction](#background-subtraction)
	- [Optical flow](#optical-flow)
	- [Saliency](#saliency)
	- [Semantic analysis](#semantic-analysis)
	- [Audio analysis](#audio-analysis)
- [Machine learning classification](#machine-learning-classification)
	- [SVM setup and parameters](#svm-setup-and-parameters)
	- [Feature scaling](#feature-scaling)
	- [Statistical measures](#statistical-measures)
	- [Feature selection](#feature-selection)
	- [Binary classification of aesthetics](#binary-classification-of-aesthetics)
	- [Binary classification of interestingness](#binary-classification-of-interestingness)
		- [Basketball classifier](#basketball-classifier)
		- [Parade classifier](#parade-classifier)
		- [Music performance classifier](#music-performance-classifier)
		- [Beach classifier](#beach-classifier)
- [References](#references)


## Introduction

The goal is the development of a system capable of semi-automatically assess video quality, to sort and filter a large stream of user generated content (UGC) in a timely manner and in a way, that it can be manageable by a human operator.

![figure 1](/images/overview.png)
*figure 1 - System overview* 

Video quality assessment (VQA) combines existing models for human visual perception and attention with more pragmatic methods based on visual feature extraction. This assessment can have an objective or subjective nature and, until recently, was focused on professional captured media. In the perspective of VQA, UGC is different in many ways from traditional media, where fidelity is of the utmost importance and mean opinion score (MOS) is the standard for comparing quality. There is nowadays a massive volume of UGC that needs to be assessed, fidelity is less important, and the criterion that defines quality becomes more personalized, focused on the group or user. As consumers are also becoming creators of content, it makes sense to combine UGC captured on large scale events in their live conventional broadcast. This inclusion could enhance the quality of Experience (QoE) provided that at the same time high standards for content quality are maintained. 

The OpenCV library of algorithms was used to compute visual features focused mainly on subjective criteria like interestingness, attention or aesthetics. These features were tested in terms of performance and usability using a graphical user interface (GUI). A subset of these features was also selected and used to create machine learning classifiers for aesthetics and interestingness. It was also developed and tested in the GUI a video indexing tool, that allows to index and sort videos by a multiple feature similarity index. It is possible to see in [figure 1]() an overview.

In a nutshell, we perform an intuition based feature engineering process to select an initial feature extraction setup, relevant according to different concepts, followed by automatic supervisioned feature selection, where a optimal attribute set is selected for each concept classification. Together, the combined result of classification with feature tresholding over visual, semantic and audio data are used to create specific event models.

We populated this release with the UGC videos from the [CERTH-ITI-VAQ700](http://mklab.iti.gr/project/certh-iti-vaq700-dataset), a dataset composed by 700 YouTube videos licensed under Creative Commons Attribution. The duration of each of these videos ranges from 1 to 6 minutes. Because of the large size of the dataset(25.3GB) we only used 1 second of each video. To have the full dataset available, one can download it from the dataset homepage to the bin/data/files folder.

![figure 2](/images/0.png)
*figure 2 - Graphical interface* 

Detailed instructions on how to use the GUI and on how to load a new video repository are available in the [instructions file](instructions.md)

## Development setup

* Windows 10
* Visual Studio community edition 2015
* openFrameworks 0.9.8
* OpenCV 3.2.0 + contrib modules
* ffmpeg

In this project github repository there is a Visual Studio 2015 Project. One can either compile it from source (make sure to read [this](install_requirements.md) before) or just copy and paste the binaries folder. The available binaries should run on any Windows 8-10. x64 machine.


## Visual features extraction

We start from an initial basic set of basic measurements, from video data (metadata, raw 3 channel frame pixel values and their change along time), and some derived values. In this section we enumerate the main features, for each one we explain its relevancy, from our point of view. We can see in [figure 3](/images/feature_groups.png) a snapshot of the current visual features taxonomy. The features can be computed by groups. For each one it is possible to compute several statistical measures including the mean, variance, standard deviation, skewness and kurtosis.

![figure 3](/images/feature_groups.png)
*figure 3 - Feature extraction groups*

### Aesthetic related features

Aesthetics is not only a cue for interestingness but also closely related to visual attention and the way images or videos appeal to humans.

#### Colour moments

The basis of colour moments lays in the assumption that the distribution of colour in an image  can be interpreted as a probability distribution. Probability distributions are characterized by a number of unique moments (e.g. Normal distributions are differentiated by their mean and variance). It therefore follows that if the colour in an image follows a certain probability distribution, the moments of that distribution can then be used as features to identify that image based on colour. Colour moments are scaling and rotation invariant, they are also a good feature to use under changing lighting conditions. We use the four central moments of colour distribution. They are the mean,  standard deviation, skewness and kurtosis computed as explained in ["M. Stricker and M. Orengo Similarity of colour in images"](http://spie.org/Publications/Proceedings/Paper/10.1117/12.205308). We compute moments for each RGB channel.

#### Colour ratio

Colour moments are great as a measure to judge similarity between images or videos, but it's a concept hard to be directly interpreted by a human, instead we devised a better feature for human interaction on a GUI based on the colour average, the colour ratio. For a human is much easier to understand and use this concept closer to mixing colors for painting.

#### Luminance

Luminance is arguable the most important visual components, humans are so much more sensitive to light than colour information. We compute the first luminance moment. In terms of the human visual perception system the variance of luminance may even be more informative than the mean or median.

#### Edge orientation and strength

Edges may indicate the boundaries of objects in the scene. Detecting sharp changes in brightness between each pixel and its surroundings allow us to detect edges.

It was implemented a simplified version of the method presented in ["Efficient Use of Local Edge Histogram Descriptor"](https://www.dcc.fc.up.pt/~mcoimbra/lectures/VC_1415/VC_1415_P8_LEH.pdf) where edge histograms MPEG-7 compliant features are extracted. We use a global and a local representation for edges, for the local one a support tool was used to split each frame by a 4x4 grid, resulting in 16 sub-images. Each one of this sub-image was convolved with 5 different oriented kernels (0-zero, 1-vertical, 2-horizontal, 3-45º, 4-135º, 5-non-directional) this process results in 16 local features representing the orientation of each sub-image. A global edge value is also computed alongside with the detected edges strength.


#### Colour diversity

Colour diversity is a property related to visual aesthetics. We count different groups of hues. On the hue histogram (computed from the H channel of HSV) we count any variation of hue bigger than a certain threshold.

#### Colourfulness

From "Measuring colourfulness in natural images" - (Hasler and Susstrunk, 2003).
Using a simple opponent colour space and computing the mean and standard deviation of the pixel cloud along each direction [figure 4](/images/formula_colourfullness.png).

![figure 4](/images/formula_colourfullness.png)
*figure 4 – Colourfulness metric* 

#### Dominant colours

A straight forward method of computing dominant colors is to use kmean clustering. It is available on most machine learning packages. However the result may not be deterministic, a standard practice is to use known initializations(not guaranteed best colors) or to run kmeans several times (runtime increase). To overcome this, we use instead a precise and fast to compute color quantisation method based on Eigenvectors.[Dominant colors in an image](https://github.com/aishack/dominant-colors).


#### Rule of thirds

The rule of thirds is one of the usual guidelines used to assess quality of visual compositions, the image is divided in nine equal parts by four equal spaced lines (the power lines), two oriented horizontally and the other two vertically, important compositional elements should be placed along these lines or their intersections. Like in ["Evaluating Visual Aesthetics in Photographic Portraiture"](https://ai2-s2-pdfs.s3.amazonaws.com/ab62/68d290b23fee36c3faf1ae77f0fd900a225f.pd) we use a Spatial composition template to score images according to a variation of the rule of thirds where more power points were added, the template is in grayscale format, afterwards we compare face centroids from the previous step of the pipeline with this template and assign a score ranging from 0-255, this value is afterward normalized.

#### Foreground area

The relative size of the area of the foreground detected.

#### Shadow area

When we know the location of a light source and see objects casting shadows on other objects, we learn that the object shadowing the other is closer to the light source. As most illumination comes downward we tend to resolve ambiguities using this information, [See](http://www.hitl.washington.edu/research/knowledge_base/virtual-worlds/EVE/III.A.1.c.DepthCues.html).

#### Camera moves

Percentage of frames where foreground area is bigger than 80% of total area available, this indicates high probability of camera movement occurrence.

#### Entropy

Entropy is a quantity normally used to describe the volume of data that needs to be encoded by a compression algorithm. A completely flat image, without any prominent pixel have zero entropy and can be compressed to a very small size. On the other hand, an image with high entropy, full of details and pixel contrast will result in a very large encoded file size.

### Attention related features

Measures related to attention can be used to improve user QoE in many applications. In the remainder of this section are enumerated visual features in some way related to attention, and for each one explained its relevancy, from our point of view. It was used a top-down approach for feature extraction. It is possible to see an overview in [figure 3]( /images/feature_groups.png), resulting in a small, off the shelf, set of visual features able to capture, at some extent, important aspects of the interestingness, attention and visual aesthetic concepts. Interestingness is an important visual property, as the human vision system (HVS) searches actively for regions of interest to improve the efficiency of subsequent object detection and recognition tasks and is also a precursor to attention. Interestingness literally means something that arouses curiosity.

#### Focus

Focus is very related to sharpness, in an image that is completely in-focus we can easily perceive fully its content, on the other hand it is harder for our perception system to process a blurry image thus, possibly, hampering overall attention arouse. Based on a survey implementation of many focus measurement algorithms in [[1]](https://drive.google.com/file/d/0B6UHr3GQEkQwYnlDY2dKNTdudjg/view) we selected the OpenCV port of 'LAPV' Variance of Laplacian algorithm [[2]](http://decsai.ugr.es/vip/files/conferences/Autofocusing2000.pdf). These types of algorithms are used in camera auto-focus, and rely on edge detection since we can detect more edges in a focused image than in a blurred version of it. It returns the maximum sharpness detected, which is a good indicator of if a camera is in focus or not.

#### Faces

Faces capture attention, to the detriment of other visual stimuli. There are [evidences](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC1857737/)  from neuropsychology and physiology that there are specialized areas for face perception in the human brain. In a [visual cueing paradigm](http://jov.arvojournals.org/article.aspx?articleid=2192946), observers responded faster to a target appearing in the location of a face cue than of an object cue. The capacity to detect and recognize faces in milliseconds is very important, as it played a preponderant role in the survival of our ancestors. It was computed the average number of video frames where faces were detected.

#### Face area

It is the ratio of face area (area of face bounding box) in relation to the full image area. If faces are found we measure their areas, we considered that the preponderance of attention faces can arouse is directly related to their visible area size.

#### Smiles

When someone smiles he/she is normally seen as a person with positive intentions. Generally, people tend to be attracted by smiling subjects. We do smile detection with a cascade classifier to compute a smile measure. It represents average frames where smiles were found. Depending on the Boolean parameter "inside Face" smiles can be detected inside or outside faces.

#### static saliency

Algorithms belonging to this category, exploit different image features that allow to detect salient objects in a non-dynamic scenario. We experiment with both approaches for static saliency offered by the [OpenCV 3.2 Saliency API](https://docs.opencv.org/3.2.0/d8/d65/group__saliency.html).
- Spectral Residual [[3]](https://www.researchgate.net/publication/221364530_Saliency_Detection_A_Spectral_Residual_Approach) - Starting from the principle of natural image statistics, this method simulates the behaviour of pre-attentive visual search. The algorithm analyses the log spectrum of each image and obtain the spectral residual. Then transform the spectral residual to the spatial domain to obtain the saliency map, which suggests the positions of proto-objects.
- Fine Grained [[4]](http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=4CC3F597B5F4F9EC4E4D71817098C3E2?doi=10.1.1.700.4706&rep=rep1&type=pdf) - This method calculates saliency based on centre-surround differences. High resolution saliency maps are generated in real time by using integral images. 
After computing a saliency map with one of the above methods, we calculate the mean pixel value of this saliency map to give a rough measure of its strength, afterwards the value is normalized.

#### Focus difference

From the background subtraction category of algorithms, it represents the difference of focus between foreground and background. We try to detect low depth of field, where the region of interest is in focus and the background blurred. This is an attribute that can boost aesthetic perceived from media or attention arouse and in cinema and photography is commonly used to direct attention from the audience or emphasize visual depth.

#### Motion magnitude

Motion is an important cue for attention deployment. Motion magnitude is the computed length of the average motion vector, and we can use it as a global measure of change introduced by motion between two frames of video. 

#### Shakiness

It is based on the comparison of the angle between two subsequent motion vectors (computed from three consecutive video frames). If the angle is above a certain threshold (45º) the transition between frames is marked as "shaky", then is computed the ratio between these "shaky" transitions and the total frames of the video.

#### Objective index

The frame rate and resolution of video were used to compute a simple objective quality measure.

## Semantic features extraction

We extract the top-5 semantic concepts present on each video. Further details in the [Semantic analysis section](#semantic-analysis)

## Audio features extraction

We extracted several audio descriptors using the [Essentia](http://essentia.upf.edu/documentation/) open-source C++ library for audio description and music analysis. More details are available in the [Audio analysis section](#audio-analysis).

### LOWLEVEL

* Average_loudness -  Dynamic range descriptor. It rescales average loudness, computed on 2sec windows with 1 sec overlap, into the [0,1] interval. The value of 0 corresponds to signals with large dynamic range, 1 corresponds to signal with little dynamic range. This algorithm computes the loudness of an audio signal defined by Steven's power law. It computes loudness as the energy of the signal raised to the power of 0.67. Algorithms: [Loudness](http://essentia.upf.edu/documentation/reference/streaming_Loudness.html).
	
* Dynamic_complexity - Dynamic complexity computed on 2sec windows with 1sec overlap. This algorithm computes the dynamic complexity defined as the average absolute deviation from the global loudness level estimate on the dB scale. It is related to the dynamic range and to the amount of fluctuation in loudness present in a recording. Silence at the beginning and at the end of a track are ignored in the computation in order not to deteriorate the results. Algorithms: [DynamicComplexity](http://essentia.upf.edu/documentation/reference/streaming_DynamicComplexity.html)
	
* MFCC - The first 13 melodic frequency cepstrum coefficients. As there is no standard implementation, the MFCC-FB40 is used by default. See algorithm: [MFCC](http://essentia.upf.edu/documentation/reference/streaming_MFCC.html)

### RHYTHM

* Beats_count - Number of detected beats

* Bpm - BPM value according to detected beats

* Danceability - The algorithm is derived from Detrended Fluctuation Analysis (DFA). The output is the danceability of the audio signal. These values usually range from 0 to 3 (higher values meaning more danceable).. Algorithms: [Danceability](http://essentia.upf.edu/documentation/reference/streaming_Danceability.html)

* Onset_rate - This algorithm computes the number of onsets per second and their position in time for an audio signal. Onset detection functions are computed using both high frequency content and complex-domain methods. Algorithms: [OnsetRate](http://essentia.upf.edu/documentation/reference/streaming_OnsetRate.html)

### TONAL

Algorithms: [ChordsDetection](http://essentia.upf.edu/documentation/reference/streaming_ChordsDetection.html), [ChordsDescriptors](http://essentia.upf.edu/documentation/reference/streaming_ChordsDescriptors.html).

* Chords_changes_rate - Chords change rate in the progression.

* Chords_number_rate -  Ratio of different chords from the total number of chords in the progression.

* Key_strength -  key of the progression.

* Tuning_diatonic_strength key - Strength estimated from high-resolution HPCP (120 dimensions) using diatonic profile. Algorithms: [Key](http://essentia.upf.edu/documentation/reference/streaming_Key.html)

* Tuning_equal_tempered_deviation -  Equal-temperament deviation estimated from high-resolution HPCP (120 dimensions). Algorithms: [HighResolutionFeatures](http://essentia.upf.edu/documentation/reference/streaming_HighResolutionFeatures.html)
	
* Tuning_nontempered_energy_ratio - Non-tempered energy ratio estimated from high-resolution HPCP (120 dimensions). Algorithms: [HighResolutionFeatures](http://essentia.upf.edu/documentation/reference/streaming_HighResolutionFeatures.html)


## Algorithm categories

### Object detection

We apply object Detection using Haar feature-based cascade classifiers. This method was originally proposed by Paul Viola and Michael J. Jones. We instantiate two cascade classifiers loading data from existing .xml classifier files, the first one is used for face detection, and the second can be used to load any existing classifiers. There are around 30 different human features classifiers in the folder (/bin/data/haar), and it is possible to find classifiers for features like eyes, mouth, nose and body among others.

### Background Subtraction

Background subtraction or Foreground Detection is a technique where an image's foreground is extracted for further processing. It is widely used for detecting moving objects in videos or cameras. The rationale in the approach is that of detecting the moving objects from the difference between the current frame and a reference frame, often called “background image”, or “background model”. Background subtraction is generally based on a static background hypothesis which is often not applicable in real environments. With indoor scenes, reflections or animated images on screens lead to background changes. In a same way, due to wind, rain or illumination changes brought by weather, static backgrounds methods have difficulties with outdoor scenes.
From the [video analysis](http://docs.opencv.org/3.0-beta/modules/video/doc/video.html) section of OpenCV, we use the class BackgroundSubtractor, included in the Motion Analysis subcategory. The class is only used to define the common interface for the whole family of background/foreground segmentation algorithms from which we use two specific algorithms that take shadows in account [[5]](http://escholarship.org/uc/item/2kj2g2f7), a shadow is detected if a pixel is a darker version of the background.

* Background Subtractor KNN - The class implements the K-nearest neighbours background subtraction described in [[6]](http://www.zoranz.net/Publications/zivkovicPRL2006.pdf). Very efficient if number of foreground pixels is low.
* Background Subtractor MOG2 - The class implements the Gaussian mixture model background subtraction method described in [[7]](http://www.zoranz.net/Publications/zivkovic2004ICPR.pdf) , and [[6]](http://www.zoranz.net/Publications/zivkovicPRL2006.pdf).

### Optical flow

Optical flow is the pattern of apparent motion of image objects between two consecutive frames caused by the movement of an object or camera. It is a 2D vector field where each vector is a displacement vector showing the movement of points from first frame to second. Optical flow works on two main assumptions: the pixel intensities of an object do not change between consecutive frames and neighbouring pixels have similar motion. OpenCV provides an algorithm to find dense optical flow. It computes the optical flow for every frame. It is based on Gunner Farneback's algorithm which is explained in [[8]](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.102.2455&rep=rep1&type=pdf). 

### Saliency

Topics related to saliency were adapted from the related OpenCV documentation. Many computer vision applications may benefit from understanding where humans focus given a scene. Other than cognitively understanding the way human perceive images and scenes, finding salient regions and objects in the images helps various tasks such as speeding up object detection, object recognition, object tracking and content-aware image editing.

### Semantic analysis

Since OpenCV 3.3.0 was officially released, a [new deep neural networks](https://docs.opencv.org/3.3.0/d2/d58/tutorial_table_of_content_dnn.html) module was made available supporting several deep learning frameworks, including Caffe, TensorFlow, and Torch/PyTorch.
Furthermore, this API for using pre-trained deep learning models is compatible with the C++ API, making it very simple to:

    - Load a model from disk.
    - Pre-process an input video frame.
    - Pass the video frame through the network and obtain the output classifications.
    - Compute pooled top 5 concepts for each video file.

We use the popular GoogleLeNet network architecture (Inception), it was introduced by Szegedy et al. in their 2014 paper, [Going deeper with convolutions](https://arxiv.org/abs/1409.4842). Caffe is a deep learning framework made with expression, speed, and modularity in mind. It was developed by Berkeley AI Research (BAIR)/The Berkeley Vision and Learning Center (BVLC) and community contributors. Next we link the caffe BVLC model and the prototxt file together with the list of the 1000  concepts.

- [caffemodel file](video-assessment/bin/data/dnn/bvlc_googlenet.caffemodel).
- [prototxt file](video-assessment/bin/data/dnn/bvlc_googlenet.prototxt).
- [concept list file](video-assessment/bin/data/dnn/synset_words.txt).

Further information:

* [Brewing ImageNet](http://caffe.berkeleyvision.org/gathered/examples/imagenet.html)
* [OpenCV sample: caffe_googlenet.cpp](https://github.com/opencv/opencv/blob/master/samples/dnn/caffe_googlenet.cpp)
* [OpenCV dnn/googleNet tutorial](https://docs.opencv.org/3.4.0/d5/de7/tutorial_dnn_googlenet.html)
* [Classification: Instant Recognition with Caffe](http://nbviewer.jupyter.org/github/BVLC/caffe/blob/master/examples/00-classification.ipynb)
* [Model zoo](https://github.com/BVLC/caffe/wiki/Model-Zoo#berkeley-trained-models)
* [ImageNet index](http://image-net.org/challenges/LSVRC/2012/index)
* [ImageNet 1000 categories](http://image-net.org/challenges/LSVRC/2012/browse-synsets)

### Audio analysis

[Essentia](http://essentia.upf.edu/documentation/) is an open-source C++ library with Python bindings for audio analysis and audio-based music information retrieval. The library contains an extensive collection of reusable algorithms which implement audio input/output functionality, standard digital signal processing blocks, statistical characterization of data, and a large set of spectral, temporal, tonal and high-level music descriptors. In addition, Essentia can be complemented with [Gaia](https://github.com/MTG/gaia), a C++ library with python bindings which implement similarity measures and classification on the results of audio analysis, and generate classification models that Essentia can use to compute high-level description of music.

 We used the Static binaries for an extractor specifically designed for the [AcousticBrainz project](http://acousticbrainz.org/). It is a configurable command-line feature extractor that computes a large set of spectral, time-domain, rhythm, tonal and high-level descriptors suited for batch computations on large music collections.

All descriptors are analyzed on a signal resampled to 44kHz sample rate, summed to mono and normalized using replay gain value. The frame-wise descriptors are summarized by their statistical distribution. We only selected 13 features from the [large list](http://essentia.upf.edu/documentation/streaming_extractor_music.html#music-descriptors) of available features.

## Machine learning classification

 SVM binary classifiers were trained, using our extracted features, to predict aesthetic and interestingness from video. The classifiers are based on two publicly available video datasets with ground truth data. One for aesthetic  concepts where the dataset was used with a scheme that exploits visual properties and movement to assess the aesthetic quality of video. Other for interestingness , used in the development of a computational system to compare the interestingness levels of videos. For more details please refer to the corresponding papers [[9]](https://www.researchgate.net/profile/Christos_Tzelepis/publication/307516039_Video_aesthetic_quality_assessment_using_kernel_Support_Vector_Machine_with_isotropic_Gaussian_sample_uncertainty_KSVM-IGSU/links/57d9074a08ae601b39b04749/Video-aesthetic-quality-assessment-using-kernel-Support-Vector-Machine-with-isotropic-Gaussian-sample-uncertainty-KSVM-IGSU.pdf) and [[10]](http://www.yugangjiang.info/publication/aaai13-interestingness.pdf).

### SVM setup and parameters

SVM Type (C_SVC) - C-Support Vector Classification. n-class classification, this type of formulation allows imperfect separation of classes with penalty multiplier C for outliers.

Kernel Type (RBF) - Radial basis function, a good choice in most cases.

C value – Parameter C of a SVM optimization problem. The C parameter trades off misclassification of training examples against simplicity of the decision surface. A low C makes the decision surface smooth, while a high C aims at classifying all training examples correctly by giving the model freedom to select more samples as support vectors.

gamma – Parameter gamma of a kernel function. Intuitively, the gamma parameter defines how far the influence of a single training example reaches, with low values meaning "far" and high values meaning "close". The gamma parameters can be seen as the inverse of the radius of influence of samples selected by the model as support vectors.

### Feature scaling

In a pre-processing step data normalisation is performed. Since the range of values of raw data varies widely, in some machine learning algorithms, namely SVM, objective functions will not work properly without normalization.

![figure 6](/images/tool-norm-formulae.png)
*figure 6 - Feature normalisation options* 

### Statistical measures

In a classification task, the precision for a class is the number of true positives (i.e. the number of items correctly labelled as belonging to the positive class) divided by the total number of elements labelled as belonging to the positive class (i.e. the sum of true positives and false positives, which are items incorrectly labelled as belonging to the class). Recall in this context is defined as the number of true positives divided by the total number of elements that actually belong to the positive class (i.e. the sum of true positives and false negatives, which are items which were not labelled as belonging to the positive class but should have been).

A precision score of 1.0 for a class C means that every item labelled as belonging to class C does indeed belong to class C (but says nothing about the number of items from class C that were not labelled correctly) whereas a recall of 1.0 means that every item from class C was labelled as belonging to class C (but says nothing about how many other items were incorrectly also labelled as belonging to class C).

Often, there is an inverse relationship between precision and recall, where it is possible to increase one at the cost of reducing the other. Usually, precision and recall scores are not discussed in isolation. Instead, either values for one measure are compared for a fixed level at the other measure (e.g. precision at a recall level of 0.75) or both are combined into a single measure. Examples of measures that are a combination of precision and recall are the F-measure (the weighted harmonic mean of precision and recall).

![figure 7](/images/statistical_measures.png)
*figure 7 - Statistical measures* 

### Feature selection

In machine learning and statistics, feature selection, also known as variable selection, attribute selection or variable subset selection, is the process of selecting a subset of relevant features (variables, predictors) for use in model construction. Feature selection techniques are used for four reasons:

        - simplification of models to make them easier to interpret by researchers/users,
        - shorter training times,
        - to avoid the curse of dimensionality,
        - enhanced generalization by reducing overfitting (formally, reduction of variance)

The central premise when using a feature selection technique is that the data contains many features that are either redundant or irrelevant, and can thus be removed without incurring much loss of information. Redundant or irrelevant features are two distinct notions, since one relevant feature may be redundant in the presence of another relevant feature with which it is strongly correlated.

For this we use the feature selection algorithm [minimum-Redundancy-Maximum-Relevance (mRMR)](http://home.penglab.com/proj/mRMR/), to reorganize our variables taking into account not only the dependency and redundancy between variables but also their relevancy. More details in [[11]](http://home.penglab.com/papersall/docpdf/2004_JBCB_feasel-04-06-15.pdf).

### Binary classification of aesthetics

  We used the CERTH-ITI-VAQ700 dataset, a comprehensive [video dataset](http://mklab.iti.gr/project/certh-iti-vaq700-dataset) for the problem of aesthetic quality assessment with annotated scores for 700 (UGC)videos from YouTube, 350 videos are rated as being of high aesthetic quality and another 350 as being of low aesthetic quality. We extracted our set of features from the dataset and using the ground truth experimental results generated a classifier to predict aesthetic of any new sample.

* At start a rough pre-selection was made according to the concept to be classified, discarding redundant or not applicable groups of features. The overall extracted features were reduced by this process from the initial 98 columns to 48 as shown in [figure 8](/images/a_48_features.png).

![figure 8](/images/a_48_features.png)
*figure 8 - Feature pre-selection* 

* We fed these 48 columns [initial feature vector](/docs/A2018C_49_cut_mrmr.csv), representing our features/attributes to the feature selection algorithm [minimum-Redundancy-Maximum-Relevance (mRMR)](http://home.penglab.com/proj/mRMR/), during this step some variables were discarded, only the top 40 features were preserved. It is possible to see the resulting algorithm output in [figure 9](/images/a_mrmr40_result.png). The 40 variables are shown sorted by relevancy only, at the left side, and by mRMR at the right, the first column for both sort methods refer to the ranked variable position, the second to its original column position, the third to its denomination and finally, the fourth to the algorithm rank score.

![figure 9](/images/a_mrmr40_result.png)
*figure 9 - Feature selection result* 

* The resulting 700x40 feature vector from the previous step was rescaled to [0,1] with minmax normalisation. Other normalisation methods were also pondered as shown in [figure 6](/images/tool-norm-formulae.png). The resulting min. and max. values are also saved in a CSV file.

* Next, we trained SVM classifiers incrementing progressively the number of features used and evaluating, at the same time, the classification performance using several statistical measures listed in [figure 7](/images/statistical_measures.png). The registered classification performance using cross-validation can be observed in [figure 10](/images/a_classification_stats.png).

![figure 10](/images/a_classification_stats.png)
*figure 10 - Aesthetic classification performance with increasing number of feature* 

* The selection of the best number of features(22 or 30) was made after pondering on the statistical measures of accuracy and F1 score and also on the SVM  parameters C and Gamma obtained from the classification results with incremental number of features [figure 11](/images/a_classification_C.png) and [figure 12](/images/a_classification_G.png).

![figure 11](/images/a_classification_C.png)
*figure 11 - Aesthetic Classification C parameter* 

![figure 12](/images/a_classification_G.png)
*figure 12 - Aesthetic Classification Gamma parameter* 

### Binary classification of specific event interestingness

It is based on the Video Interestingness Database (VID), two benchmarks [datasets](http://www.yugangjiang.info/research/interestingness/index.html)  with ground-truth interestingness labels. The first one (V.I.D. dataset A) comprises 1200 videos collected from Flickr which have a rank based on interestingness. The second (V.I.D. dataset B) comprises 420 advertisement videos from YouTube. YouTube does not have an interestingness rank so to collect the interestingness scores, this dataset was subject to an experimental annotation procedure. It was extracted an extended set of features, including the before mentioned attention and aesthetics related features, from the (V.I.D. dataset A). These features together with the experimental binary values were used to train SVM classifiers for interestingness on video.

The interestingness classification focused on specific events. Each specific event dataset was gathered from the event categories of the beforementioned Flickr interestingness video dataset. We can see a breakdown and the event categories selected in [figure 13](/images/i_categories.png).
Each event dataset comprises 40 positive and 40 negative samples, this samples were compiled from the top 10% (positives) and bottom 10% (negatives) from a total of 400 videos retrieved from Flickr keyword search for each event and sorted by interestingness. The methodology used was the same used for aesthetic classification explained in the previous [Binary classification of aesthetics](#binary-classification-of-aesthetics) section.

![figure 13](/images/i_categories.png)
*figure 13 - Interestingness event categories breakdown* 

#### Basketball classifier

![figure 14](/images/basket_graph.png)
*figure 14 - Interestingness: Basketball event classifier evaluation*

In [figure 14](/images/basket_graph.png) it is possible to  see the performance comparison between classifiers trained to predict interestingness on basketball events with progressively higher number of features. The associated C and Gamma parameters evolution can also be seen at [figure 15](/images/basket_c.png) and [figure 16](/images/basket_g.png) respectively. The top 28 features were selected to train the final classifier. The accuracy registered was 88% and the F-score 0.8.

![figure 15](/images/basket_c.png)
*figure 15 - Interestingness: Basketball event classifier C optimal parameter*

![figure 16](/images/basket_g.png)
*figure 16 - Interestingness: Basketball event classifier Gamma optimal parameter*

#### Parade classifier

In [figure 17](/images/parade_graph.png) it is possible to see the performance comparison between classifiers trained to predict interestingness of parade events with progressively higher number of features.

![figure 17](/images/parade_graph.png)
*figure 17 - Interestingness: Parade event classifier evaluation*

#### Music performance classifier

In [figure 18](/images/music_graph.png) it is possible to see the performance comparison between classifiers trained to predict interestingness of music events with progressively higher number of features. as the audio information is very relevant on this event, the overall performance of the classification was worst comparing with the other events.

![figure 18](/images/music_graph.png)
*figure 18 - Interestingness: Music performance event classifier evaluation*

#### Beach classifier

In [figure 19](/images/beach_graph.png) it is possible to see the performance comparison between classifiers trained to predict interestingness from beach videos with progressively higher number of features.

![figure 19](/images/beach_graph.png)
*figure 19 - Interestingness: Beach classifier evaluation*

## References

- [1] 	D. P. A. G. Said Pertuz, "Analysis of focus measure operators for shape-from-focus," Pattern Recognition, vol. 46, no. 5, pp. 1415-1432, 2013. 
- [2] 	G. C. C. F. V. J.L. Pech Pacheco, "Diatom autofocusing in bright field microscopy: a comparative study," in 15th International Conference on Pattern Recognition, 2000. 
- [3] 	X. Z. Hou, "Saliency Detection: A Spectral Residual Approach," in IEEE Conference on Computer Vision and Pattern Recognition , 2007. 
- [4] 	A. S. A. Sebastian Montabone A, "Human Detection Using a Mobile Platform and Novel Features Derived from a Visual Saliency Mechanism," Image and Vision Computing, vol. 28, no. 3, pp. 391-402, 2010. 
- [5] 	A. Prati, I. Mikic, M. M. Trivedi and R. Cucchiara, "Detecting moving shadows: algorithms and evaluation," IEEE Transactions on Pattern Analysis and Machine Intelligence , vol. 25, no. 7, pp. 918 - 923 , 2003. 
- [6] 	F. H. Z. Zivkovic, "Efficient adaptive density estimation per image pixel for the task of background subtraction," Pattern Recognition Letters, vol. 27, no. 7, pp. 773-780, 2006. 
- [7] 	Z. Zivkovic, "Improved adaptive Gaussian mixture model for background subtraction," in Proceedings of the 17th International Conference on Pattern Recognition, ICPR, 2004. 
- [8] 	G. Farnebäck, "Two-Frame Motion Estimation Based on Polynomial Expansion," in 13th Scandinavian Conference, SCIA 2003 , Halmstad, 2003. 
- [9] 	E. M. V. M. I. P. C. Tzelepis, "Video Aesthetic Quality Assessment using Kernel Support Vector Machine with Isotropic Gaussian Sample Uncertainty (KSVM-iGSU)," in IEEE International Conference on Image Processing(ICIP), Phoenix, Arizona, USA, 2016. 
- [10] 	Y. W. ,. R. F. ,. X. X. ,. Y. Z. ,. H. Y. Y.Jiang, "Understanding and predicting interestingness of videos," in AAAI, Conference on Artificial Intelligence, Washington, 2013.
- [11] H. Peng, F. Long, C. Ding, "Feature selection based on mutual information: criteria of max-dependency, max-relevance, and min-redundancy," in IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 27, No. 8, pp.1226-1238, 2005.
