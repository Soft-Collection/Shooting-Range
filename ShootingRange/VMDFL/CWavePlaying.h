#if !defined(__CWAVEPLAYING_H__)
#define __CWAVEPLAYING_H__

#include <queue>
#include "CWaveBase.h"

typedef struct 
{
	HWAVEOUT hwo;
	UINT uMsg;
	DWORD dwInstance;
	DWORD dwParam1;
	DWORD dwParam2;
} WAVEOUTPROCDATA;

void CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

class CWavePlaying : public CWaveBase 
{
public:
	enum ePlayingState
	{
		Initialized = 0,
		Opened = 1,
		PlayingStarted = 2,
		PlayingPaused = 3
	};
	CWavePlaying(DWORD buffersCount, DWORD bufferLength, DWORD nSamplesPerSec, WORD wBitsPerSample, WORD nChannels);
	virtual ~CWavePlaying();
	BOOL Open();
	BOOL Close();
	BOOL GetPlayingState(DWORD* playingState);
	BOOL Play(char* pData, ULONG offset, ULONG size);
	BOOL Stop();
	BOOL Pause();
	BOOL Resume();
	BOOL GetPitch(LPDWORD pdwPitch);
	BOOL SetPitch(DWORD dwPitch);
	BOOL GetPlaybackRate(LPDWORD pdwRate);
	BOOL SetPlaybackRate(DWORD dwRate);
	BOOL GetVolume(LPWORD pwLeftVolume, LPWORD pwRightVolume);
	BOOL SetVolume(WORD wLeftVolume, WORD wRightVolume);
	BOOL GetPosition(LPMMTIME pmmt);
	static BOOL GetDevCaps(LPWAVEOUTCAPS pwoc);
private:
	BOOL PrepareBuffers();
	BOOL DestroyBuffers();
	BOOL PrepareHeaders();
	BOOL UnprepareHeaders();
public:
	HANDLE m_hevBufferArrived;
	std::queue<WAVEOUTPROCDATA>* m_queue;
	CRITICAL_SECTION m_QueueCriticalSection;
private:
	HWAVEOUT mHWaveOut;
	ePlayingState mPlayingState;
	BOOL mBuffersAreReady;
	BOOL mHeadersAreReady;
	DWORD mBuffersCount;
	DWORD mBufferLength;
	char** mBuffers;
	WAVEHDR* mWaveHeaders;
	WAVEFORMATEX mPCMfmt;
	CRITICAL_SECTION m_HeadersCriticalSection;
};

#endif // !defined(__CWAVEPLAYING_H__)
