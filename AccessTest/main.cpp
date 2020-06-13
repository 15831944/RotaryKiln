#include <iostream>  
#include <map>
#include <vector>
#include <string>

#import "C:\Program Files\Common Files\System\ADO\msado15.dll"  no_namespace  rename ("EOF", "adoEOF")  
#include "../»Ø×ªÒ¤/SQLConnect.hpp"

int main()
{
	AccessConnection accessConnection;
	AccessResult res;
	accessConnection.openDatabase("RotaryKiln.accdb");
	accessConnection.executeSQL("select * from user_info where  user_number= \"chenj\" ", res);
	for (auto& field : res.fieldMap)
	{
		std::cout << field.first << " ";
	}
	std::cout << std::endl;
	for (auto& record : res)
	{
		for (auto& value : record)
			std::cout << (value.empty() ? "null" : value) << " ";
		std::cout << std::endl;
	}
	accessConnection.executeSQL("update user_info set user_name=\"ddd\" where  user_number= \"chenj\" ", res);//0x800A0E78
	//accessConnection.executeSQL("update user_info set user_name = \"ddd\" where user_number=\"zhu\")", res);

	for (auto& field : res.fieldMap)
	{
		std::cout << field.first << " ";
	}
	std::cout << std::endl;
	for (auto& record : res)
	{
		for (auto& value : record)
			std::cout << (value.empty() ? "null" : value) << " ";
		std::cout << std::endl;
	}
	std::cout << accessConnection.getLastError();

	return 0;


}