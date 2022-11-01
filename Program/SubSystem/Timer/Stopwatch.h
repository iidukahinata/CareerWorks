/**
 * @file	Stopwatch.h
 * @brief   �v���N���X
 *
 * @date	2022/09/06 2022�N�x����
 */
#pragma once

enum TimePrecision
{
	Second,
	Milli,
};

class Stopwatch
{
	COMPLETED_DEVELOPMENT()
public:

	void Start()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	void Stop()
	{
		m_stopTime = std::chrono::high_resolution_clock::now();
	}

	double GetRap(TimePrecision precision = TimePrecision::Second)
	{
		Stop();

		// rap �v��
		std::chrono::duration<double, std::milli> ms = m_stopTime - m_startTime;

		switch (precision)
		{
		case Second: return (ms.count() / 1000.0);
		case Milli:  return ms.count();
		default: break;
		}
	}

private:

	// * �ŐV�t���[���X�V����
	std::chrono::high_resolution_clock::time_point m_startTime;
	std::chrono::high_resolution_clock::time_point m_stopTime;
};