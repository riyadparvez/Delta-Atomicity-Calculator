#ifndef SCORE_H
#define SCORE_H

class Score
{
public:
	Score()
	{
		this->value = 0;
		this->time = 0;
	}
	Score(unsigned long value, unsigned long time)
	{
		this->value = value;
		this->time = time;
	}
	
	unsigned long GetValue() const { return value; }
	unsigned long GetTime() const { return time; }
private:
	unsigned long value;
	unsigned long time;
};

#endif