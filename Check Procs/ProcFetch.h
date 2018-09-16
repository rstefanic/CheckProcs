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
	ProcFetch(char* connection_string, char* database_name);
	void GetProcs(std::shared_ptr<std::vector<std::string>> all_procs);

private:
	SqlConnection ^ connection_;
	SqlCommand ^ command_;

	void MarshalString(System::String^ s, std::string& os);
};

