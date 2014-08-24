#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#include <string>
#include <unordered_map>
#include <vector>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_vector.h>
#include <glog/logging.h>
#include <iostream>
#include <boost/format.hpp>
#include <omp.h>
#include <tbb/parallel_for.h>

#include "Clusters.hpp"
#include "FileReader.hpp"
#include "Operation.hpp"
#include "StringComparer.hpp"

class InputProcessor
{
public:
	InputProcessor(std::vector<std::string>& filePaths,
					std::shared_ptr<Zone::ScoreComputer> scoreComputer) 
					: filePaths(filePaths), scoreComputer(scoreComputer)
	{}
	virtual ~InputProcessor()
	{}

	void Process();

private:
	std::vector<std::string>& filePaths;
	std::shared_ptr<Zone::ScoreComputer> scoreComputer;
	tbb::concurrent_hash_map<std::string, std::shared_ptr<Clusters>, StringComparer> clustersTable;
	tbb::concurrent_vector<std::shared_ptr<Clusters>> clustersList;
	
	void GetOperations(const tbb::concurrent_vector<std::string>& lines);
	void GetAllOperations();
	void ComputeScores();
	void Print();
};

#endif
