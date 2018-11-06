#pragma once
#include "C:/Eigen/Eigen/Dense"
#include <algorithm>
#include <vector>
#include <random>

/******************************************************************************************
 * this class will contain the features at each node, a "feature" consists of two randomly
 * selected points (ID) from the neighborhood and a randomly selected projection operation
 * (from FeatureFactory)
 *****************************************************************************************/
struct Features
{
	Features():
		_point1(0),
		_point2(0),
		_featType(0)
	{}
	
	void operator=(Features& feat)
	{
		this->_point1 = feat._point1;
		this->_point2 = feat._point2;
	}

	int _point1;
	int _point2;
	int _featType;
};


/**************************************************
This class is used for drawing samples and features
(test node functions) from the dataset. 
***************************************************/
class Sample {
public:
	/***************************************************************************************** 
	 * construct a new Sample object using given dataset, labels, the index matrix (representing
	 * the points ID in neighborhoods), the distance matrix of corresponding dists, together  
	 * with number of classes, number of features means how many features are considered at each
	 * node.
	 *****************************************************************************************/
	Sample(Eigen::MatrixXf *dataset, Eigen::VectorXi *labels, 
		   Eigen::MatrixXi *indexMat, Eigen::MatrixXf *distMat, int numClass, int numFeature);
	
	/* to new a Sample object using a pointer to Sample, so that the dataset/labels etc. will 
	 * not be copied but referenced*/
	Sample(Sample* samples);

	/* to reference only a part of the dataset with the point ID*/
	Sample(const Sample* sample, Eigen::VectorXi &samplesId);


	// randomly select numSelectedSamples samples from dataset with replacement (bagging)
	void randomSampleDataset(Eigen::VectorXi &selectedSamplesId, int numSelectedSamples);

	/***************************************************************************************
	 * randomly sample features from each neighborhoood
	 * given a neighborhood consisting of k points, the number of possible features
	 * are k*(k-1)*n, where n is the projection operations, but only numSelectedFeatures
	 * are randomly chosen from all these features
	 ***************************************************************************************/
	void randomSampleFeatures();

	/***************************************************************************************
	 * return a matrix representing the neighborhood of the pointId-th point
	 * whose shape is (k, d), where k is the number of nearest neighbors
	 * and d is the dimention of each datapoint*
	 **************************************************************************************/
	Eigen::MatrixXf buildNeighborhood(int pointId) const;

	// keep track of the number of different classes in a Sample obejct
	inline int getNumClasses() { return _numClass; }

	// get the selected sample indices
	inline Eigen::VectorXi getSelectedSamplesId() { return _selectedSamplesId; }
	inline Eigen::VectorXi getSelectedSamplesId() const { return _selectedSamplesId; }

	// get the number of selected samples
	inline int getNumSelectedSamples() { return _numSelectedSamples; }

	// get the selected features
	inline std::vector<Features> getSelectedFeatures() { return _features; }

	// get the number of features sampled at each node
	inline int getNumFeatures() { return _numFeature; }
	inline int getNeighborhoodSize() { return _indexMat->cols(); }

	Eigen::VectorXi *_labels;
	Eigen::MatrixXf *_dataset;

private:

	// _indexMat stores the indices of nearest neighbors for each datapoint
	Eigen::MatrixXi *_indexMat;
	// _distMat stores the dists of nearest neighbors to each datapoint
	Eigen::MatrixXf *_distMat;

	// _selectedSamplesId stores the indices of selected datapoints
	Eigen::VectorXi _selectedSamplesId;
	std::vector<Features> _features;
	int _numClass;
	int _numSelectedSamples;
	int _numFeature;

};


/*****************************************************************
This class is used for aquiring samples without replacement.
 * It makes use of the shuffle method from the STL, which randomly
 * shuffles a vector using the given seed. If sampleSize samples 
 * are needed, then the first sampleSize elements of the vector
 * after being shuffled are kept. 
 *****************************************************************/
class Random
{
public:
	Random(int popSize, int sampleSize):
		_popSize(popSize),
		_sampleSize(sampleSize)
	{}

	std::vector<int> sampleWithoutReplacement()
	{
		std::vector<int> population;
		candidates(population);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(population.begin(), population.end(), gen);
		std::vector<int> samples(population.begin(), population.begin() + _sampleSize);
		return samples;
	}

private:
	void candidates(std::vector<int> &nums)
	{
		for (int i = 0; i < _popSize; ++i)
			nums.push_back(i);
	}
	int _popSize;
	int _sampleSize;
};