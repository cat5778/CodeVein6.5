#ifndef SoundMgr_h__
#define SoundMgr_h__

// FMOD: ����Ƽ, �𸮾�� ���� ��� �������� �η� ���̴� ���� ���� ����.
// ���忡 ���õ� �������̽��� ����.
#include "fmod.h"
#pragma comment(lib, "fmodL_vc.lib")

#include <io.h>

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	CSoundMgr();
	~CSoundMgr();

public:
	enum CHANNEL_ID { BGM, PLAYER, GOD_VOICE, MONSTER, EFFECT, MAX_CHANNEL };

public:
	HRESULT Initialize();
	void UpdateSound();
	void PlaySound(const wstring& wstrSoundKey, CHANNEL_ID eID);
	void PlayBGM(const wstring& wstrSoundKey);
	void StopSound(CHANNEL_ID eID);
	void StopAll();
	void SetVolume(CHANNEL_ID eID, float fVol); // ������ ������ �ּ� 0.f ~ �ִ� 1.f����.
	void Release();

private:
	void LoadSoundFile();

private:
	// FMOD_SOUND: ���� ���Ͽ� ���� ������ ���� ����ü.
	unordered_map<wstring, FMOD_SOUND*>	m_MapSound;

	// FMOD_CHANNEL: ���带 ����ϰ� �����ϴ� ����. ex) ���� ���� ��.
	FMOD_CHANNEL*	m_pChannelArr[MAX_CHANNEL];

	// FMOD_SYSTEM: FMOD_SOUND�� FMOD_CHANNEL�� �Ѱ� �����ϴ� ��ü.
	FMOD_SYSTEM*	m_pSystem;
};

#endif //SoundMgr_h__