#ifndef GDS_TEXTSPEAKER_H_INCLUDED
#define GDS_TEXTSPEAKER_H_INCLUDED


//////////////////////////////////////////////////////////////////////////
//
// FILE: TextSpeaker.h
//
// Defines a simple C++ class to speak some text.
//
// by Giovanni Dicanio <gdicanio@mvps.org>
//
// 2010, December 28th
//
//////////////////////////////////////////////////////////////////////////


#include <sapi.h>               // SAPI
#include <sphelper.h>           // SAPI Helper

#include "ComAutoInit.h"        // COM auto initializer


namespace gds {


//------------------------------------------------------------------------
// Simple class to speak some text (using SAPI).
//------------------------------------------------------------------------
class CTextSpeaker
{
public:

    //--------------------------------------------------------------------
    // Initializes the text speaker.
    //--------------------------------------------------------------------
    CTextSpeaker()
    {
        //
        // Create text to speech engine
        //
		free_flag=TRUE;
        HRESULT hr = m_tts.CoCreateInstance(CLSID_SpVoice);
        if (FAILED(hr))
        {
            ATLTRACE(TEXT("Text-to-speech creation failed.\n"));
            AtlThrow(hr);
        }


        //
        // Get token corresponding to default voice 
        //
        hr = SpGetDefaultTokenFromCategoryId(SPCAT_VOICES, &m_voiceToken, FALSE);
        if (FAILED(hr))
        {
            ATLTRACE(TEXT("Can't get default voice token.\n"));
            AtlThrow(hr);
        }


        //
        // Set default voice
        //
        hr = m_tts->SetVoice(m_voiceToken);
        if (FAILED(hr))
        {
            ATLTRACE(TEXT("Can't set default voice.\n"));
            AtlThrow(hr);
        }
    }

	static DWORD WINAPI SpeakWaiting(_In_ LPVOID lpParameter)
	{
		CTextSpeaker * ctestspeaker = (CTextSpeaker*)(lpParameter);
		ctestspeaker->m_tts->WaitUntilDone(INFINITE);
		ctestspeaker->free_flag=TRUE;
		return 0;
	}
    //--------------------------------------------------------------------
    // Speaks some text.
    // (The input text must not be empty.)
    //--------------------------------------------------------------------
    void Speak(const CString &text)
    {
        //
        // Input text must not be empty
        //
        if (text.IsEmpty())
        {
            ATLTRACE(TEXT("Empty text passed to CTextSpeaker::Speak().\n"));
            AtlThrow(E_INVALIDARG);
        }


        //
        // Speak input text
        //
        ULONG streamNumber;
		//USES_CONVERSION; 
		//LPCWSTR pwcStr = A2CW((LPCSTR)text); 
		free_flag=FALSE;
        HRESULT hr = m_tts->Speak(text.AllocSysString(), SPF_IS_NOT_XML | SPF_ASYNC | SPF_PURGEBEFORESPEAK, &streamNumber);
		if (FAILED(hr))
        {
            ATLTRACE(TEXT("Speak failed.\n"));
            AtlThrow(hr);
        }
		DWORD dwThreadId;
		CreateThread(NULL, 0, SpeakWaiting, this, 0, &dwThreadId);
    }
	BOOL GetFreeFlag()
	{
		return free_flag;
	}

    //--------------------------------------------------------------------
    // Cleanup
    //--------------------------------------------------------------------
    ~CTextSpeaker()
    {
        // Nothing to do here.
        // Automatic cleanup thanks to C++ RAII is fine :-)
    }



    //
    // Ban copy
    //
private:
    CTextSpeaker(const CTextSpeaker &);
    CTextSpeaker & operator=(const CTextSpeaker &);



    //
    // IMPLEMENTATION
    //
private:

    // COM initialization and cleanup (must precede other COM related data members)
    gds::CComAutoInit m_comInit;

    // Text to speech engine
    CComPtr<ISpVoice> m_tts;

    // Default voice token
    CComPtr<ISpObjectToken> m_voiceToken;
	BOOL free_flag;
};


} // namespace gds


#endif // GDS_TEXTSPEAKER_H_INCLUDED
