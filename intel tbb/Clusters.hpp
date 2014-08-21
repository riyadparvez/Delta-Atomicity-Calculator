#ifndef CLUSTERS_H
#define CLUSTERS_H

#include <memory>
#include <string>
#include <glog/logging.h>
#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_vector.h"
#include "Operation.hpp"
#include "StringComparer.hpp"
#include "Score.hpp"
#include "Zone.hpp"

class Clusters
{
public:
	Clusters() {}
	Clusters(std::string key, std::shared_ptr<Zone::ScoreComputer> computer)
	: key(key), computer(computer)
	{
	}
	virtual ~Clusters()
	{}

	void AddOperation(std::shared_ptr<Operation> op)
	{
		const Operation& operation = (*op);
		tbb::concurrent_hash_map<std::string, tbb::concurrent_vector<std::shared_ptr<Operation>>, StringComparer>::accessor a;
		
		if(map.find(a, operation.GetValue()))
		{
		    a->second.push_back(op);
		}
		else
		{
		    tbb::concurrent_vector<std::shared_ptr<Operation>> ops;
		    ops.push_back(op);
		    map.insert({operation.GetValue(), ops});
		}
	}

	void ComputeZones()
	{
		const auto& localMap = map;
		for(const auto& pair : localMap)
		{
		    std::shared_ptr<Zone> z = std::make_shared<Zone>(computer);
		    z->SetKey(key);
		    z->SetValue(pair.first);

		    unsigned long long maxStartTime = 0;
		    unsigned long long minFinishTime = std::numeric_limits<unsigned long long>::max();

		    unsigned long readCount = 0;
		    unsigned long writeCount = 0;
		    unsigned long long writeStart = std::numeric_limits<unsigned long long>::max();

		    for (const auto& op: pair.second)
		    {
				const Operation& operation = (*op);
		        if (operation.GetType() == Operation::Type::Read)
		        {
		            readCount++;
		        }

		        if (operation.GetType() == Operation::Type::Write)
		        {
		            writeCount++;
		            if (operation.GetStartTime() < writeStart)
		            {
		                writeStart = operation.GetStartTime();
		            }
		        }

		        if (operation.GetStartTime() > maxStartTime)
		        {
		            maxStartTime = operation.GetStartTime();
		        }
		        if (operation.GetEndTime() < minFinishTime)
		        {
		            minFinishTime = operation.GetEndTime();
		        }
		    }
		    
		    z->SetMaxStartTime(maxStartTime);
		    z->SetMinFinishTime(minFinishTime);
		    if (minFinishTime < maxStartTime)
		    {
		        z->SetType(Zone::Type::Forward);
		    }
		    else
		    {
		        z->SetType(Zone::Type::Backward);
		    }
		    
		    z->SetReadCount(readCount);
		    z->SetWriteCount(writeCount);
		    z->SetFirstWriteStart(writeStart);
		    zones.push_back(z);
		}
		zones.shrink_to_fit();
	}

	void SortZones()
	{
		std::stable_sort(std::begin(zones), std::end(zones), 
						[](const std::shared_ptr<Zone>& zonePtr1, const std::shared_ptr<Zone>& zonePtr2)
						{
							return (*zonePtr1) < (*zonePtr2);
						});
	}

	void ComputeScores()
	{
		const auto& localZones = zones;
		int count = 0;
		for(const auto& z1 : localZones)
		{
			const Zone& zone1 = (*z1);
			count++;

			if(zone1.IsForward() && zone1.IsValidZone())
			{
				for(unsigned int i=0; i<localZones.size(); i++)
				{
          if(i < count)
          {
            continue;
          }
					const Zone& zone2 = (*localZones[i]);
				
					if(!zone1.Intersects(zone2))
					{
						continue;
					}
					Score score = zone1.ComputeScore(zone2);
				
					if(score.GetValue() > maximumScore.GetValue())
					{
						maximumScore = score;
					}
				}
			}
		}
	}

	std::string ToString() const
	{
		std::stringstream ss;
		const auto& localMap = map;
		for(const auto& pair : localMap)
		{
			ss<<"Key: "<<key<<"Value: "<<pair.first<<"\n"
			<<"Clusters Members:\n";
		    for (const auto& op: pair.second)
			{
				Operation& operation = (*op);
				std::string type = (operation.GetType()==Operation::Type::Read)? "Read" : "Write";
				ss<<"["<<operation.GetStartTime()<<", "<<
				operation.GetEndTime()<<", "<<
				type<<"]\n";
			}
		}
		return ss.str();
	}
	
	std::string GetKey() const
	{ return key; }
	
	Score GetMaximumScore() const 
	{ return maximumScore; }
	
private:
	const std::string key;
	tbb::concurrent_hash_map<std::string, tbb::concurrent_vector<std::shared_ptr<Operation>>, StringComparer> map;
	tbb::concurrent_vector<std::shared_ptr<Zone>> zones;
	std::shared_ptr<Zone::ScoreComputer> computer;
	Score maximumScore;
	long minDelta;
	long maxDelta;
	double avgDelta;
	double sdDelta;
};

#endif
