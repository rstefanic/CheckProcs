#include "stdafx.h"
#include "ProcFetch.h"

ProcFetch::ProcFetch(char* connection_string, char* database_name,
		std::vector<std::string>* all_procs) {
	System::String^ conn_string = gcnew System::String(connection_string);

	System::String^ db_name = gcnew System::String(database_name);

	System::String^ get_all_procs = gcnew System::String("SELECT * " +
		"FROM " + db_name + ".information_schema.routines " +
		"WHERE routine_type = 'PROCEDURE'");

	all_procs_ = all_procs;

	connection_ = gcnew SqlConnection(conn_string);
	command_ = gcnew SqlCommand(get_all_procs, connection_);
}

std::vector<std::string>* ProcFetch::GetProcs() {
	try {
		connection_->Open();
		SqlDataReader^ reader = command_->ExecuteReader();

		std::string reader_string;

		while (reader->Read()) {
			MarshalString(reader->GetString(2), reader_string);
			all_procs_->push_back(reader_string.c_str());
		}

		connection_->Close();
	}
	catch (SqlException^ e) {
		std::string errorMessage;
		MarshalString(e->Message, errorMessage); 

		std::cout << std::endl;
		std::cout << "An error occurred connecting to the database." << std::endl;
		std::cout << "SqlException: " << errorMessage << std::endl;
		std::cout << std::endl;
		exit(1);
	}

	return all_procs_;
}

void ProcFetch::MarshalString(System::String ^ s, std::string& os) {
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}
