#pragma once 
#ifndef _SQL_CONNECT_HPP_
#define _SQL_CONNECT_HPP_

//#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include "../../Logger/Logger/Logger.hpp"

/// <summary>
/// SQL结果集
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
    std::map<std::string, size_t>& fieldMap;
    std::string& operator[](std::string name)
    {
        return std::vector<std::string>::operator[](fieldMap[name]);
    }
};

struct AccessResult
    : public std::vector<AccessRecord>
{
    std::map<std::string, size_t> fieldMap;

    void appendField(std::string name, bool ifRebuild = false)
    {
        fieldMap[name] = fieldMap.size();
        if (ifRebuild)
            rebuild();
    }

    void rebuild()
    {
        for (auto& record : *this)
            record.resize(fieldMap.size());
    }

    AccessRecord& appendRecord()
    {
        push_back(AccessRecord(fieldMap));
        return back();
    }

    void clear()
    {
        fieldMap.clear();
        std::vector<AccessRecord>::clear();
    }
};
/// <summary>
/// Access连接
/// </summary>
struct AccessConnection
{
protected:
    using LogLevel = thatboy::logger::LogLevel;
    _ConnectionPtr m_pConnection;
    _RecordsetPtr m_pRecordset;

public:
    AccessConnection()
    {
        m_pConnection.CreateInstance(__uuidof(Connection));
        m_pRecordset.CreateInstance(__uuidof(Recordset));
    }
    ~AccessConnection()
    {
        if (m_pConnection->GetState() != adStateClosed)
        {
            m_pConnection->Close();
        }
        m_pConnection.Release();
        m_pRecordset.Release();
    }

    /// <summary>
    /// 打开数据库
    /// </summary>
    /// <param name="dbFile">数据库文件</param>
    /// <param name="userId">用户</param>
    /// <param name="passwd">密码</param>
    /// <returns>成功>0</returns>
    HRESULT openDatabase(LPCSTR dbFile, LPCSTR userId = "", LPCSTR passwd = "")
    {
        lastError = "";
        try
        {
            m_pConnection->Provider = "Microsoft.ACE.OLEDB.12.0";
            std::string connectString = "Data Source=";
            connectString += dbFile;

            logger.log(LogLevel::Info, " Open ", dbFile, ", userId=", std::string(userId).empty() ? "null" : userId);
            return m_pConnection->Open(connectString.c_str(), "", "", adModeUnknown);
        }
        catch (_com_error& e)
        {
            lastError = std::string("[ErrorMessage:") + e.ErrorMessage()
                + std::string(", Source:") + _com_util::ConvertBSTRToString(e.Source())
                + std::string(", Description:") + _com_util::ConvertBSTRToString(e.Description()) + "]";
            logger.log(LogLevel::Error, " Open ", dbFile, ", userId=", userId, ", ", lastError);
            return e.Error();
        }
    }

    /// <summary>
    /// 执行SQL
    /// </summary>
    /// <param name="sqlCmd"></param>
    /// <returns>成功>0</returns>
    HRESULT executeSQL(LPCSTR sqlCmd, AccessResult& res)
    {
        lastError = "";
        try
        {
            res.clear();
            auto ret{ m_pRecordset->Open(sqlCmd, m_pConnection.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText) };
            if (!m_pRecordset->BOF)
            {
                // 添加字段
                auto fields{ m_pRecordset->GetFields() };
                for (size_t i = 0; i < fields->GetCount(); i++)
                    res.appendField(_com_util::ConvertBSTRToString(fields->GetItem(static_cast<short>(i))->GetName()));

                // 添加记录
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
            logger.log(LogLevel::Info, " SQL:", sqlCmd, " Success.");
            logger.log(LogLevel::Info, " Record:", res.size(), " Field:", res.fieldMap.size());
            m_pRecordset->Close();
            return ret;
        }
        catch (_com_error& e)
        {
            return handleError(sqlCmd, e);
        }
        logger.log(LogLevel::Error, " SQL:", sqlCmd, " Unknow error.");
        return -2;
    }


    HRESULT executeSQL(const std::string& sqlCmd, AccessResult& res)
    {
        return executeSQL(sqlCmd.c_str(), res);
    }

    HRESULT executeSQL(const std::string& sqlCmd)
    {
        lastError = "";
        try
        {
            //m_pRecordset->Valid();
            auto ret = m_pRecordset->Open(sqlCmd.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
            logger.log(LogLevel::Info, " SQL:", sqlCmd, " Success.");
            m_pRecordset->Close(); 
            return ret;
        }
        catch (_com_error& e)
        {
            return handleError(sqlCmd, e);
        }
    }

    HRESULT handleError(const std::string& sqlCmd, _com_error& e)
    {
        lastError = std::string("[")
            + "ErrorMessage:" + e.ErrorMessage()
            + ", ErrorCode:" + std::to_string(e.Error())
            + ", Source:" + _com_util::ConvertBSTRToString(e.Source())
            + ", Description:" + _com_util::ConvertBSTRToString(e.Description()) + "]";
        bool safeCmd{ e.Error() == 0x800A0E78 && sqlCmd.find("select") == std::string::npos };
        logger.log(safeCmd ? LogLevel::Warning : LogLevel::Error, " SQL:", sqlCmd, " Failure."
            , "ErrorCode: ", std::hex, std::setiosflags(std::ios::uppercase), e.Error()
            , ", Message:", e.ErrorMessage()
            , ", Source:", _com_util::ConvertBSTRToString(e.Source())
            , ", Description:", _com_util::ConvertBSTRToString(e.Description()));
        return safeCmd ? 0 : e.Error();
    }

    const std::string& getLastError()const
    {
        return lastError;
    }

    void setLogger(std::string file)
    {
        logger.open(file);
    }

private:
    thatboy::logger::FileLogger logger;
    std::string lastError;
    static bool ifInitialize;

    /// <summary>
    /// 自动初始化
    /// </summary>
    struct _AutoInitialize
    {
        _AutoInitialize()
        {
#ifndef __AFX_H__
            ifInitialize = SUCCEEDED(CoInitialize(nullptr));
#else
            //ifInitialize = AfxOleInit();
#endif
        }
        ~_AutoInitialize()
        {
#ifndef __AFX_H__
            CoUninitialize();
            ifInitialize = false;
#endif
        }     
    };

    static _AutoInitialize _;
};
bool AccessConnection::ifInitialize{ false };
AccessConnection::_AutoInitialize AccessConnection::_;
#endif