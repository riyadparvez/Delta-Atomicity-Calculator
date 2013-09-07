#ifndef DEFAULT_SCORE_COMPUTER_H
#define DEFAULT_SCORE_COMPUTER_H

#include "Zone.h"

class DefaultScoreComputer : public Zone::ScoreComputer
{
public:
    DefaultScoreComputer() : ScoreComputer()
    {}
    ~DefaultScoreComputer() {}
    
    virtual Score ForwardForwardConflict(const Zone& zone_1, const Zone& zone_2)
	{
		//check whether both are forward zones and they conflict
		if (zone_1.IsForward() && zone_2.IsForward() && zone_1.ForwardForwardZoneIntersects(zone_2)) 
		{             
			if (zone_1.GetMaxStartTime() < zone_2.GetMaxStartTime()) 
			{
				return Score (zone_1.GetMaxStartTime() - zone_2.GetMinFinishTime(), 
					(zone_1.GetMaxStartTime() + zone_2.GetMinFinishTime()) / 2);
				//return sa - z.fa;
			}
			else 
			{
				return Score (zone_2.GetMaxStartTime() - zone_2.GetMinFinishTime(), 
					(zone_2.GetMaxStartTime() + zone_2.GetMinFinishTime()) / 2);
				//return z.sa - z.fa;
			}
		}
			
		return Score(0, 0);
	}
	
    virtual Score ForwardBackwardConflict(const Zone& zone_1, const Zone& zone_2)
	{
		//  we assume this is F, and z is B and check for conflict
		if (zone_1.IsForward() && zone_2.IsBackward() && zone_1.ForwardBackwardZoneIntersects(zone_2)) 
		{ 
			if (zone_2.IsValidZone() && (zone_2.GetFirstWrite() < zone_1.GetMinFinishTime())) 
			{
				if ((zone_1.GetMaxStartTime() - zone_2.GetMinFinishTime()) < 
					(zone_2.GetMaxStartTime() - zone_1.GetMinFinishTime())) 
				{
					return Score (zone_1.GetMaxStartTime() - zone_2.GetMinFinishTime(), 
							(zone_1.GetMaxStartTime() + zone_2.GetMinFinishTime()) / 2);
				}
				else 
				{
					return Score (zone_2.GetMaxStartTime() - zone_1.GetMinFinishTime(), 
						(zone_2.GetMaxStartTime() + zone_1.GetMinFinishTime()) / 2);
				}
				//return Math.min(sa - z.fa, z.sa - fa);
			}
			return Score(zone_1.GetMaxStartTime() - zone_2.GetMinFinishTime(), 
					(zone_1.GetMaxStartTime() + zone_2.GetMinFinishTime()) / 2);
		}
			
		return Score (0, 0);
	}
private:

};

#endif