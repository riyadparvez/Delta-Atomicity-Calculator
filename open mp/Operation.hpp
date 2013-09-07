#ifndef OPERATION_H
#define OPERATION_H

#include <sstream>
#include <string>

class Operation
{
public:
	enum class Type
	{
		Read,
		Write,
	};

	Operation(std::string key, std::string value, unsigned long long startTime, unsigned long long endTime, Type type)
	: key(key), value(value), startTime(startTime), endTime(endTime), operationType(type)
	{
	}

	std::string GetKey() const { return key; }
	std::string GetValue() const { return value; }
	unsigned long long GetStartTime() const  { return startTime; }
	unsigned long long GetEndTime() const { return endTime; }
	Type GetType() const { return operationType; }

	struct OperationComparer 
	{
		static size_t hash(const Operation& x ) 
		{
			std::size_t h = 0;
			for (auto& s : x.key)
			{
				h = (h*17)^s;
			}
			for (auto& s : x.value)
			{
				h = (h*17)^s;
			}
			h = (std::size_t) ((h*17)^x.startTime);
			h = (std::size_t) ((h*17)^x.endTime);

			return h;
		}
		//! True if operations are equal
		static bool equal(const Operation& x, const Operation& y ) 
		{
			return (x.key == y.key) &&
				   (x.value == y.value) &&
				   (x.startTime == y.startTime) && 
		           (x.endTime == y.endTime) &&
				   (x.operationType == y.operationType);;
		}
	};

	bool Equals(const Operation& y) const
	{
		return (key == y.key) && 
			(value == y.value) &&
			(startTime == y.startTime) && 
			(endTime == y.endTime) &&
			(operationType == y.operationType);
	}
	
	std::string ToString() const
	{
		std::stringstream ss;
		std::string s = (operationType == Type::Read)? "Read" : "Write";
		ss << "Key: " << key << "\n" << 
				"Value: " << value << "\n" <<
				"Start Time " << startTime << "\n" <<
				"End Time: " << endTime << "\n" <<
				"Type: " << s << "\n";
		return ss.str();
	}
	
private:
	std::string key;
	std::string value;
	unsigned long long startTime;
	unsigned long long endTime;
	Type operationType;

	Operation() { } // private default constructor
};

#endif