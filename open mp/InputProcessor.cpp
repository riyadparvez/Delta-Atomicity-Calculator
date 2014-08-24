#include "InputProcessor.hpp"

void InputProcessor::GetOperations(const tbb::concurrent_vector<std::string>& lines)
{
	auto& localClustersList = clustersList;
	for (unsigned i=0; (i+4) < lines.size(); i+=5)
	{
		unsigned long long startTime = std::stoull(lines[i+2]);
		unsigned long long endTime = std::stoull(lines[i+3]);
		Operation::Type type;

		if(lines[i+4]=="0")
		{
			type = Operation::Type::Read;
		}
		else if(lines[i+4]=="2" || lines[i+4]=="3")
		{
			type = Operation::Type::Write;
		}
		else
		{
			//there is an input format error for current record
			continue;
		}
		std::shared_ptr<Operation> op = std::make_shared<Operation>(lines[i], lines[i+1], startTime, endTime, type);

		Operation& operation = (*op);
		tbb::concurrent_hash_map<std::string, std::shared_ptr<Clusters>, StringComparer>::accessor a;
		if(clustersTable.find(a, operation.GetKey()))
		{
			a->second->AddOperation(op);
		}
		else
		{
			std::shared_ptr<Clusters> clusters = std::make_shared<Clusters>(operation.GetKey(), scoreComputer);
			clusters->AddOperation(op);
			localClustersList.push_back(clusters);
			clustersTable.insert({operation.GetKey(), clusters});
		}
		localClustersList.shrink_to_fit();
	}
}
    
void InputProcessor::GetAllOperations()
{
	tbb::concurrent_vector<tbb::concurrent_vector<std::string>> fileLines;
	
	tbb::parallel_for(tbb::blocked_range<size_t>(0, filePaths.size()), 
		[&](const tbb::blocked_range<size_t>& r) 
		{
			for(size_t i = std::begin(r); i != std::end(r); ++i)
			{
				const std::string& filePath = filePaths[i];
				FileReader reader(filePath);			
				tbb::concurrent_vector<std::string>& lines = reader.ReadAll();
				fileLines.push_back(lines);
			}
		}
	);
	fileLines.shrink_to_fit();
	
	LOG(INFO)<<"Read all files";
	
	tbb::parallel_for(tbb::blocked_range<size_t>(0, fileLines.size()), 
		[&](const tbb::blocked_range<size_t>& r) 
		{
			for(size_t i = std::begin(r); i != std::end(r); ++i)
			{
				const tbb::concurrent_vector<std::string>& lines = fileLines[i];			
				GetOperations(lines);
			}
		}
	);
}

void InputProcessor::ComputeScores()
{
	tbb::parallel_for(tbb::blocked_range<size_t>(0, clustersList.size()),
		[&](const tbb::blocked_range<size_t>& r)
		{
			for(size_t i = std::begin(r); i != std::end(r); ++i)
			{
				auto& clusters = (*clustersList[i]);
				clusters.ComputeZones();
 				clusters.SortZones();	
				clusters.ComputeScores();
			}		
		}
	);
}

void InputProcessor::Print()
{
	auto& localClusterTable = clustersTable;
 	for(const auto& pair: localClusterTable)
 	{
 		std::cout<<"Key: "<<pair.first<<std::endl;
		std::cout<<"Delta: "<<pair.second->GetMaximumScore().GetValue()<<std::endl;
 	}
}

void InputProcessor::Process()
{
	LOG(INFO)<<"Started process";
	GetAllOperations();	
	LOG(INFO)<<"Got all operations";	
	ComputeScores();
		
	Print();	
}
