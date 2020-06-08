#pragma once 
#include "stdafx.h"
#include <map>
#include <vector>
#include <string>


/// <summary>
/// SQL结果集
/// </summary>
using SQLResult = std::map<std::string, std::vector<std::string>>;

/// <summary>
/// Access连接
/// </summary>
struct AccessConnection
{
    _ConnectionPtr m_pConnection;
    _RecordsetPtr m_pRecordset;

    AccessConnection()
    {
        m_pConnection.CreateInstance(__uuidof(Connection));
        m_pRecordset.CreateInstance(__uuidof(Recordset));
    }
    ~AccessConnection()
    {
        if (m_pConnection->State)
        {
            m_pConnection->Close();
        }
        m_pConnection = nullptr;
        m_pRecordset = nullptr;
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
        try
        {
            m_pConnection->Provider = "Microsoft.ACE.OLEDB.12.0";
            std::string connectString = "Data Source=";
            connectString += dbFile;
            return m_pConnection->Open(connectString.c_str(), "", "", adModeUnknown);
        }
        catch (_com_error& e)
        {
            lastError = e.ErrorMessage();
            return -1;// e.Error();
        }
        return -2;
    }

    /// <summary>
    /// 执行SQL
    /// </summary>
    /// <param name="sqlCmd"></param>
    /// <returns>成功>0</returns>
    HRESULT executeSQL(LPCSTR sqlCmd, SQLResult& res)
    {
        try
        {
            auto ret{ m_pRecordset->Open(sqlCmd, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText) };


            res.clear();
            if (m_pRecordset->BOF)
                return -1;

            /// 字段
            auto fields{ m_pRecordset->GetFields() };
            for (int i = fields->GetCount() - 1; i >= 0; i--)
            {
                auto fieldName{ _com_util::ConvertBSTRToString(fields->GetItem(static_cast<short>(i))->GetName()) };

                res[fieldName] = std::vector<std::string>();
            }

            // 结果
            m_pRecordset->MoveFirst();
            while (!m_pRecordset->adoEOF)
            {
                for(auto&field:res)
                {
                    auto& fieldName = field.first;
                    auto& fieldCont = field.second;
                    auto var{ m_pRecordset->GetCollect(fieldName.c_str()) };
                    if (var.vt != VT_NULL)
                        fieldCont.push_back(_com_util::ConvertBSTRToString(static_cast<_bstr_t>(var)));
                }

                m_pRecordset->MoveNext();
            }
            m_pRecordset->Close();
            return ret;
        }
        catch (_com_error& e)
        {
            lastError = e.ErrorMessage();
            return -1;
        }
        return -2;
    }


    HRESULT executeSQL(const std::string& sqlCmd, SQLResult& res)
    {
        return executeSQL(sqlCmd.c_str(), res);
    }

    HRESULT executeSQL(const std::string& sqlCmd)
    {
        try
        {
            return { m_pRecordset->Open(sqlCmd.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText) };
        }
        catch (_com_error& e)
        {
            lastError = e.ErrorMessage();
            return -1;
        }
    }

    const std::string& getLastError()const
    {
        return lastError;
    }
private:
    std::string lastError;
    /// <summary>
    /// 自动初始化
    /// </summary>
    static struct _AutoInitialize
    {
        _AutoInitialize()
        {
            CoInitialize(nullptr);
        }
        ~_AutoInitialize()
        {
            CoUninitialize();
        }

    } _;
};