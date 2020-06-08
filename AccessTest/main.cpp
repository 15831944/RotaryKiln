#include <iostream>  
#include <map>
#include <vector>
#include <string>

#import "C:\Program Files\Common Files\System\ADO\msado15.dll"  no_namespace  rename ("EOF", "adoEOF")  

using SQLResult = std::map<std::string, std::vector<std::string>>;

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
    HRESULT OpenDB(LPCSTR dbFile, LPCSTR userId = "", LPCSTR passwd = "")
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
            return -1;// e.Error();
        }
        return -2;
    }

    /// <summary>
    /// 执行SQL
    /// </summary>
    /// <param name="sqlCmd"></param>
    /// <returns>成功>0</returns>
    HRESULT ExecuteSQL(LPCSTR sqlCmd, SQLResult& res)
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
                for (auto& [field, list] : res)
                {
                    auto var{ m_pRecordset->GetCollect(field.c_str()) };
                    if (var.vt != VT_NULL)
                        list.push_back(_com_util::ConvertBSTRToString(static_cast<_bstr_t>(var)));
                }

                m_pRecordset->MoveNext();
            }
            m_pRecordset->Close();
            return ret;
        }
        catch (_com_error& e)
        {
            return -1;
        }
        return -2;
    }

private:
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

void ExecuteSQL()
{
   AccessConnection conn;
   SQLResult res;
   conn.OpenDB("RotaryKiln.accdb", "", "");
   auto ret=conn.ExecuteSQL(R"(select * from user_info where user_number = 'admin')", res);

   if (res.empty())
       return;

   for (auto& [field, l] : res)
       std::cout << field << "\t";
   std::cout << std::endl;
   for (size_t i = 0; i < res.begin()->second.size(); i++)
   {
       for (auto& [f, list] : res)
       {
           std::cout << list[i] << "\t";;
       }
       std::cout << std::endl;
   }
}


void ExecuteSQL_()
{
    SQLResult res;
    _ConnectionPtr m_pConnection;
    _RecordsetPtr m_pRecordset;
    m_pConnection.CreateInstance(__uuidof(Connection));
    m_pRecordset.CreateInstance(__uuidof(Recordset));
    m_pConnection->Provider = "Microsoft.ACE.OLEDB.12.0";
    m_pConnection->Open("Data Source=RotaryKiln.accdb;"/*connectString.c_str()*/, "", "", adModeUnknown);
    int ret = m_pRecordset->Open("SELECT * FROM region_info", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

    /// 字段
    auto fields{ m_pRecordset->GetFields() };
    for (int i = fields->GetCount() - 1; i >= 0; i--)
    {
        
        auto name{ fields->GetItem(static_cast<short>(i))->GetName() };
        std::cout << _com_util::ConvertBSTRToString(name) << std::endl;
        //auto fieldName = std::to_string(i);// _com_util::ConvertBSTRToString(/*fields->Item[i]->GetName()*/);
       // res[fieldName] = std::vector<std::string>();
    }
    m_pRecordset->MoveFirst();
    auto var{ m_pRecordset->GetCollect("stu_name") };

    std::cout << _com_util::ConvertBSTRToString(static_cast<_bstr_t>(var));

    m_pRecordset->Close();



    //m_pConnection->OpenSchema()
    for (int i = fields->GetCount() - 1; i >= 0; i--)
    {
        auto fieldName = std::to_string(i);// _com_util::ConvertBSTRToString(/*fields->Item[i]->GetName()*/);
        res[fieldName] = std::vector<std::string>();
    }

    // 结果
    m_pRecordset->MoveFirst();
    while (!m_pRecordset->adoEOF)
    {
        for (auto& [field, list] : res)
        {
            auto var{ m_pRecordset->GetCollect(field.c_str()) };
            if (var.vt != VT_NULL)
                list.push_back(_com_util::ConvertBSTRToString(static_cast<_bstr_t>(var)));
        }

        m_pRecordset->MoveNext();
    }
    m_pRecordset->Close();
}
int main()
{
    CoInitialize(NULL);         //初始化OLE/COM库环境  

    ExecuteSQL();

    CoUninitialize();
}