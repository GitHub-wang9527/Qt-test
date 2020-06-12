#pragma once

#include <QString>

#include <sapi.h>
#include <sphelper.h>

class TTS
{

public:
	static void save(QString qpathname, QString text) {
		ISpVoice *pVoice = NULL;
		HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
		if (SUCCEEDED(hr))
		{
			LPCTSTR path = reinterpret_cast<LPCWSTR>(qpathname.data());

			CComPtr <ISpStream> cpWavStream;
			CComPtr <ISpStreamFormat> cpOldStream;
			CSpStreamFormat originalFmt;
			pVoice->GetOutputStream(&cpOldStream);
			originalFmt.AssignFormat(cpOldStream);
			hr = SPBindToFile(path, SPFM_CREATE_ALWAYS, &cpWavStream, &originalFmt.FormatId(), originalFmt.WaveFormatExPtr());
			if (SUCCEEDED(hr))
			{
				pVoice->SetOutput(cpWavStream, TRUE);

				LPCTSTR content = reinterpret_cast<LPCTSTR>(text.data());
				//	WCHAR WTX[] = L"凡事何必都看淡，自己理解就很好";
				pVoice->Speak(content, SPF_IS_XML, NULL);
				pVoice->Release();
				pVoice = NULL;
			}
		}
	}
	static void play(QString text) {
		ISpVoice *pVoice = NULL;
		HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
		if (SUCCEEDED(hr))
		{
			LPCTSTR content = reinterpret_cast<LPCTSTR>(text.data());
			hr = pVoice->Speak(content, 0, NULL);
			pVoice->Release();
			pVoice = NULL;
		}
	}

private:

};

#include <QAxObject>
#include <QFile>

class SapiTool 
{


public:
	static void saveHelpDoc(QString path) {
		// 保存说明文档
		QAxObject ax("SAPI.SpVoice");
		QString doc = ax.generateDocumentation();
		QFile file("spi.html");
		file.open(QIODevice::WriteOnly);
		file.write(doc.toLocal8Bit());
		file.close();
	}
	static void speak(QString text) {
		QAxObject ax("SAPI.SpVoice");
		//	ax.dynamicCall("SetRate(int)",10);
		ax.dynamicCall("Speak(QString, SpeechVoiceSpeakFlags Flags )", text);
	}
private:

};

/*
	QString txt = ui.textEdit->toPlainText();
//	SapiTool::speak(txt);

	TTS::save(QStringLiteral("D:\\你好.wav"), txt);
	TTS::play(txt);

*/


/*
#include <sapi.h>
#include <sphelper.h>

bool isCreatWavFile = false;//是否创建.wav音频文件
int main()
{
	ISpVoice *pVoice = NULL;
	if (FAILED(::CoInitialize(NULL)))
		return FALSE;
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	if (SUCCEEDED(hr))
	{
		//如果生成音频文件，在WTX[]中输入音频文本内容
		if (isCreatWavFile)
		{
			CComPtr <ISpStream> cpWavStream;
			CComPtr <ISpStreamFormat> cpOldStream;
			CSpStreamFormat originalFmt;
			pVoice->GetOutputStream(&cpOldStream);
			originalFmt.AssignFormat(cpOldStream);
			hr = SPBindToFile(L"d:\\output.wav", SPFM_CREATE_ALWAYS, &cpWavStream, &originalFmt.FormatId(), originalFmt.WaveFormatExPtr());
			if (SUCCEEDED(hr))
			{
				pVoice->SetOutput(cpWavStream, TRUE);
				WCHAR WTX[] = L"凡事何必都看淡，自己理解就很好，决定我们一生的，是我们的选择。我们时刻都在失去，我们努力的想去拥有更多的精彩，可惜只有两只手，所以必须学会选择，学会放弃。要清楚哪些是我们不需要的，如果心的欲望太大，什么都想抓，可能最后什么知足心灵。只平淡命运无奈";
				pVoice->Speak(WTX, SPF_IS_XML, NULL);
				pVoice->Release();
				pVoice = NULL;
			}
		}
		else
		{
			hr = pVoice->Speak(L"Hello world", 0, NULL);
			pVoice->Release();
			pVoice = NULL;
		}


	}

	::CoUninitialize();
	return TRUE;
}

*/