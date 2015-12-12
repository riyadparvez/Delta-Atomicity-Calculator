#ifndef ZONE_H
#define ZONE_H

#include <memory>
#include <string>
#include <glog/logging.h>

#include "Score.hpp"

class Zone
{
public:
    enum class Type
    {
        Forward,
		Backward,
    };

    class ScoreComputer
    {
    public:
        virtual ~ScoreComputer() {}
        virtual Score ForwardForwardConflict(const Zone& zone_1, const Zone& zone_2) = 0;
        virtual Score ForwardBackwardConflict(const Zone& zone_1, const Zone& zone_2) = 0;
    };

    Zone(Type type, std::string key, std::string value, unsigned long long maxStartTime, 
		 unsigned long long minFinishTime, unsigned long readCount, unsigned long writeCount, 
		 unsigned long long firstWrite,
         std::shared_ptr<Zone::ScoreComputer> computer);
    Zone(std::shared_ptr<Zone::ScoreComputer> computer);
    virtual ~Zone()
	{}

    Type GetType() const 
    { return zoneType; }
    std::string GetKey() const 
    { return key; }
    std::string GetValue() const 
    { return value; }
    unsigned long long GetMaxStartTime() const 
    { return maxStartTime; }
    unsigned long long GetMinFinishTime() const 
    { return minFinishTime; }
    unsigned long long GetReadCount() const 
    { return readCount; }
    unsigned long long GetWriteCount() const 
    { return writeCount; }
    unsigned long long GetFirstWrite() const 
    { return firstWriteStart; }
    size_t HashCode() const;

    void SetType(Type value) 
	{ zoneType = value; }
    void SetKey(std::string value)
	{ key = value; }
    void SetValue(std::string value)
	{ this->value = value; }
    void SetMaxStartTime(unsigned long long value)
	{ maxStartTime = value; }
    void SetMinFinishTime(unsigned long long value)
	{ minFinishTime = value; }
    void SetReadCount(unsigned long value)
	{ readCount = value; }
    void SetWriteCount(unsigned long value)
	{ writeCount = value; }
    void SetFirstWriteStart(unsigned long long value) 
	{ firstWriteStart = value; }

    bool IsForward() const 
    { return zoneType == Type::Forward; }
    bool IsBackward() const 
    { return zoneType == Type::Backward; }
    bool IsValidZone() const;
    bool Equals(const Zone& otherZone) const;
    bool Intersects(const Zone& otherZone) const;
    bool ForwardForwardZoneIntersects(const Zone& otherZone) const;
    bool ForwardBackwardZoneIntersects(const Zone& otherZone) const;
    Score ComputeScore(const Zone& zone2) const;
    bool operator<(const Zone& other) const;
    bool operator==(const Zone& other) const;
    bool operator!=(const Zone& other) const;
	std::string ToString() const;

private:
    Type zoneType;
    std::string key;
    std::string value;
    unsigned long long maxStartTime;
    unsigned long long minFinishTime;
    unsigned long readCount;
    unsigned long writeCount;
    unsigned long long firstWriteStart;

    Score ForwardForwardConflict(const Zone& otherZone) const 
    { return computer->ForwardForwardConflict(*this, otherZone); }
    Score ForwardBackwardConflict(const Zone& otherZone) const 
    { return computer->ForwardBackwardConflict(*this, otherZone); }

    std::shared_ptr<Zone::ScoreComputer> computer;
};

#endif
