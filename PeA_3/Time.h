#pragma once
class Time
{
public:
	Time();
	~Time();

	long long int frequency;
	long long int start;
	long long int elapsed;
	float time_s;
	float time_ms;
	float time_us;

	long long int ReadQPC();
	long long int Start();
	long long int Stop();
	void Update();
	void Print();
};

