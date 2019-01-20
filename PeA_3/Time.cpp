#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include "Time.h"

Time::Time()
{
	frequency = 0;
	start = 0;
	elapsed = 0;
	time_s = 0;
	time_ms = 0;
	time_us = 0;

}
Time::~Time() {}

long long int Time::ReadQPC()
{
	LARGE_INTEGER count;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&count);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return((long long int)count.QuadPart);
}

long long int Time::Start()
{
	start = ReadQPC();
	return start;
}

long long int Time::Stop()
{
	elapsed = (ReadQPC() - start);
	Update();
	return elapsed;
}

void Time::Update()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
	time_s = (float)elapsed / frequency;
	time_ms = (1000.0f * elapsed) / frequency;
	time_us = (1000000.0f * elapsed) / frequency;
}

void Time::Print()
{
	printf("czas wykonania: %lf [s], %lf [ms], %lf [us]\n", time_s, time_ms, time_us);
}
