#pragma once
#ifndef _SQL_CONNECT_HPP_
#define _SQL_CONNECT_HPP_

//#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include "Logger.hpp"

/// <summary>
/// SQL��¼
/// </summary>
struct AccessRecord
	: public std::vector<std::string>
{
	AccessRecord(std::map<std::string, size_t>& map)
		: fieldMap(map)
		, std::vector<std::string>(map.size())
	{}
	AccessRecord(AccessRecord&&) = default;
	AccessRecord(const AccessRecord&) = default;
	std::map<std::string, size_t>& fieldMap;	// ���õ��������ӳ���key->ptr��
	std::string& operator[](std::string name)
	{
		return std::vector<std::string>::operator[](fieldMap[name]);
	}
};

/// <summary>
/// SQL�����
/// </summary>
struct AccessResult
	: public std::vector<AccessRecord>
{
	std::map<std::string, size_t> fieldMap;

	/// <summary>
	/// ����ֶ�
	/// </summary>
	/// <param name="name">�ֶ���</param>
	/// <param name="ifRebuild">�Ƿ��ؽ����ؿ���������</param>
	void appendField(std::string name, bool ifRebuild = false)
	{
		fieldMap[name] = fieldMap.size();
		if (ifRebuild)
			rebuild();
	}

	/// <summary>
	/// �ؽ�����
	/// </summary>
	void rebuild()
	{
		for (auto& record : *this)
			record.resize(fieldMap.size());
	}

	/// <summary>
	/// ��Ӽ�¼
	/// </summary>
	/// <returns>��¼</returns>
	AccessRecord& appendRecord()
	{
		push_back(AccessRecord(fieldMap));
		return back();
	}

	/// <summary>
	/// ��ս����
	/// </summary>
	void clear()
	{
		fieldMap.clear();
		std::vector<AccessRecord>::clear();
	}
};

/// <summary>
/// Access����
/// </summary>
struct AccessConnection
{
protected:
	using LogLevel = thatboy::logger::LogLevel;
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;

	bool isValid = false;
public:
	AccessConnection()
	{
		create();
	}
	~AccessConnection()
	{
		release();
	}

	/// <summary>
	/// ������Դ
	/// </summary>
	void create()
	{
		if (!bCoInitialized)
			;
			//throw "Not CoInitialize";
		if (!isValid)
		{
			auto a = m_pConnection.CreateInstance(__uuidof(Connection));
			auto b = m_pRecordset.CreateInstance(__uuidof(Recordset));
			isValid = true;
		}
	}

	/// <summary>
	/// �ͷ���Դ
	/// </summary
	void release()
	{
		if (!isValid)
		{
			m_pRecordset.Release();
			m_pConnection.Release();
			isValid = false;
		}
	}

	/// <summary>
	/// �ر����ݿ�
	/// </summary>
	void close()
	{
		if (!isValid)
			return;
		if (m_pRecordset->GetState() == adStateOpen)
		{
			m_pRecordset->Close();
		}
		if (m_pConnection->GetState() == adStateOpen)
		{
			m_pConnection->Close();
		}
	}

	/// <summary>
	/// �����ݿ�
	/// </summary>
	/// <param name="dbFile">���ݿ��ļ�</param>
	/// <returns>�ɹ�>0���쳣�ᱻд��lastError�����ش�����</returns>
	HRESULT open(LPCSTR dbFile)
	{
		lastError = "";
		try
		{
			// MS Access Database njsx
			m_pConnection->Provider = "Microsoft.ACE.OLEDB.12.0";
			std::string connectString = "Data Source=";
			//std::string connectString = "ODBC;DSN=MS Access Database njsx;DBQ=";
			//std::string connectString = "ODBC;DRIVER={Microsoft Access Driver (*.mdb, *.access)};DBQ=";
			connectString += dbFile;

			logger.log(LogLevel::Info, " Open ", dbFile);
			return  m_pConnection->Open(connectString.c_str(), "", "", adModeUnknown);
		}
		catch (_com_error& e)
		{
			lastError = std::string("[ErrorMessage:") + e.ErrorMessage()
				+ std::string(", Source:") + _com_util::ConvertBSTRToString(e.Source())
				+ std::string(", Description:") + _com_util::ConvertBSTRToString(e.Description()) + "]";
			logger.log(LogLevel::Error, " Open ", dbFile, lastError);
			return e.Error();
		}
	}

	/// <summary>
	/// ��ѯSQL
	/// </summary>
	/// <param name="sqlCmd"></param>
	/// <returns>�ɹ�>0���쳣�ᱻд��lastError�����ش�����</returns>
	HRESULT select(const std::string& sqlCmd, AccessResult& res)
	{
		lastError = "";
		try
		{
			res.clear();
			auto ret{ m_pRecordset->Open(_bstr_t(sqlCmd.c_str()), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText) };
			if (!m_pRecordset->BOF)
			{
				// ����ֶ�
				auto fields{ m_pRecordset->GetFields() };
				for (size_t i = 0; i < fields->GetCount(); i++)
					res.appendField(_com_util::ConvertBSTRToString(fields->GetItem(static_cast<short>(i))->GetName()));

				// ��Ӽ�¼
				m_pRecordset->MoveFirst();
				while (!m_pRecordset->adoEOF)
				{
					auto& record = res.appendRecord();
					for (auto& field : res.fieldMap)
					{
						auto& fieldName = field.first;
						auto var{ m_pRecordset->GetCollect(fieldName.c_str()) };
						record[fieldName] = var.vt != VT_NULL ? _com_util::ConvertBSTRToString(static_cast<_bstr_t>(var)) : "";
					}
					m_pRecordset->MoveNext();
				}
			}
			//logger.log(LogLevel::Info, " SQL:", sqlCmd, " Success.");
			//logger.log(LogLevel::Info, " Record:", res.size(), " Field:", res.fieldMap.size());
			m_pRecordset->Close();
			return ret;
		}
		catch (_com_error& e)
		{
			return handleError(sqlCmd, e);
		}
		catch (...)
		{
			logger.log(LogLevel::Fatal, " SQL:", sqlCmd, " Unknow error.");
		}
		return -1;
	}

	/// <summary>
	/// ִ��SQL
	/// </summary>
	/// <param name="sqlCmd"></param>
	/// <returns>�ɹ�>0���쳣�ᱻд��lastError�����ش�����</returns>
	HRESULT execute(const std::string& sqlCmd)
	{
		lastError = "";
		try
		{
			auto ret = m_pConnection->Execute(sqlCmd.c_str(), NULL, adCmdText);
			//logger.log(LogLevel::Info, " SQL:", sqlCmd, " Success.");
			return ret;
		}
		catch (_com_error& e)
		{
			return handleError(sqlCmd, e);
		}
		catch (...)
		{
			logger.log(LogLevel::Fatal, " SQL:", sqlCmd, " Unknow error.");
		}
		return -1;
	}

	/// <summary>
	/// ����SQL�쳣
	/// </summary>
	/// <param name="sqlCmd">SQL���</param>
	/// <param name="e">�쳣</param>
	/// <returns>�ɹ�>0���쳣�ᱻд��lastError�����ش�����</returns>
	HRESULT handleError(const std::string& sqlCmd, _com_error& e)
	{
		try
		{
			lastError = std::string("[")
				+ "ErrorMessage:" + e.ErrorMessage()
				+ ", ErrorCode:" + std::to_string(e.Error())
				+ ", Source:" + _com_util::ConvertBSTRToString(e.Source())
				+ ", Description:" + _com_util::ConvertBSTRToString(e.Description()) + "]";
			logger.log(LogLevel::Error, " SQL:", sqlCmd, " Failure."
				, "ErrorCode: ", std::hex, std::setiosflags(std::ios::uppercase), e.Error(), std::dec
				, ", Message:", e.ErrorMessage()
				, ", Source:", _com_util::ConvertBSTRToString(e.Source())
				, ", Description:", _com_util::ConvertBSTRToString(e.Description()));
			return e.Error();
		}
		catch (...)
		{
			logger.log(LogLevel::Fatal, " SQL:", sqlCmd, " Unknow error.");
		}
		return -1;
	}

	/// <summary>
	/// ��ȡ��һ�δ�������
	/// </summary>
	const std::string& getLastError()const
	{
		return lastError;
	}

	/// <summary>
	/// ������־�ļ�
	/// </summary>
	/// <param name="file">��־·��</param>
	void setLogger(std::string file)
	{
		logger.open(file);
	}

private:
	thatboy::logger::FileLogger logger;
	std::string lastError;

	static bool bCoInitialized;
	/// <summary>
	/// ȫ���Զ���ʼ��
	/// </summary>
	struct _AutoInitialize
	{
		_AutoInitialize()
		{
#ifndef __AFX_H__
			bCoInitialized = (CoInitialize(nullptr) == S_OK);
#else
			// MFC �ڴ˳�ʼ�������ֶ���ʼ��
			// AfxOleInit();
#endif
		}
		~_AutoInitialize()
		{
#ifndef __AFX_H__
			CoUninitialize();
			bCoInitialized = false;
#endif
		}
		};
	static _AutoInitialize _;
};
bool AccessConnection::bCoInitialized{ false };
AccessConnection::_AutoInitialize AccessConnection::_;
#endif