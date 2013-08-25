#include <iostream>
#include <sstream>
#include "Zone.h"

Zone::Zone(Type type, std::string key, std::string value, unsigned long long maxStartTime, unsigned long long minFinishTime,
           unsigned long readCount, unsigned long writeCount, unsigned long long firstWrite, 
           std::shared_ptr<Zone::ScoreComputer> computer)
           : zoneType(type), key(key), value(value), maxStartTime(maxStartTime), minFinishTime(minFinishTime), 
             readCount(readCount), writeCount(writeCount), firstWriteStart(firstWrite),
             computer(computer)
{
}

Zone::Zone(std::shared_ptr<Zone::ScoreComputer> computer)
           : computer(computer)
{
}

bool Zone::Equals(const Zone& otherZone) const
{
    return (zoneType == otherZone.zoneType) &&
           (key == otherZone.key) &&
           (value == otherZone.value) &&
           (maxStartTime == otherZone.maxStartTime) &&
           (minFinishTime == otherZone.minFinishTime);
}

bool Zone::Intersects(const Zone& otherZone) const
{
    if(IsForward() && otherZone.IsForward())
    {
        return ForwardForwardZoneIntersects(otherZone);
    }
    if(IsForward() && otherZone.IsBackward())
    {
        return ForwardForwardZoneIntersects(otherZone);
    }
    if(IsBackward() && otherZone.IsForward())
    {
        return otherZone.ForwardBackwardZoneIntersects(*this);
    }
    if(IsBackward() && otherZone.IsBackward())
    {
        return false;
    }
}

bool Zone::ForwardForwardZoneIntersects(const Zone& otherZone) const
{
	return (otherZone.minFinishTime < maxStartTime);
}

bool Zone::ForwardBackwardZoneIntersects(const Zone& otherZone) const
{
    return (otherZone.minFinishTime < maxStartTime && 
			otherZone.maxStartTime > minFinishTime);
}

bool Zone::IsValidZone() const
{
	return ((writeCount == 1) && (readCount > 0));
}

Score Zone::ComputeScore(const Zone& zone2) const
{
    if(zone2.IsForward())
    {
        return ForwardForwardConflict(zone2);
    }
    else
    {
        return ForwardBackwardConflict(zone2);
    }
}

bool Zone::operator<(const Zone& other) const
{
    return maxStartTime < other.maxStartTime;
}

bool Zone::operator==(const Zone& other) const 
{
    return Equals(other);
}

bool Zone::operator!=(const Zone& other) const 
{
    return !Equals(other);
}

size_t Zone::HashCode() const
{
    size_t h = 0;
    for( const char* s = this->key.c_str(); *s; ++s )
    {
        h = (h*17)^*s;
    }
    for( const char* s = this->value.c_str(); *s; ++s )
    {
        h = (h*17)^*s;
    }
    h = (h*31) + maxStartTime;
    h = (h*31) + minFinishTime;
    return h;
}

std::string Zone::ToString() const
{
	std::stringstream ss;
	std::string s= (zoneType == Type::Forward)? "Forward\n" : "Backward\n";
	ss << "Key: " << key << "\n" <<
		   "Value: " << value << "\n" <<
		   "Max Start Time: " << maxStartTime << "\n" <<
    	   "Min Finish Time: " << minFinishTime << "\n" <<
		   "Read Count: " << readCount << "\n" <<
		   "Write Count: " << writeCount << "\n" <<
		   "Type: " << s << "\n";
	return ss.str();
}
