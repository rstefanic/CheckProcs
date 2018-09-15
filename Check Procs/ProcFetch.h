#pragma once

#include <vector>

#using <System.Data.dll>
#using <System.Drawing.dll> 

using namespace System::Data::SqlClient;
using namespace System::ComponentModel;
using namespace System::Runtime::InteropServices;

ref class ProcFetch
{
public:
	ProcFetch(char* connection_string, char* database_name, 
		std::vector<std::string>* all_procs);
	std::vector<std::string>* GetProcs();

private:
	SqlConnection ^ connection_;
	SqlCommand ^ command_;
	std::vector<std::string>* all_procs_;

	void MarshalString(System::String^ s, std::string& os);
};

