#include "stdafx.h"
#include "ProcFetch.h"
#include "ProjectInfo.h"

#define STRBUFFER 500

void GetConnectionString(char* conn_string);
void PrintUsage();
int CompareStrings(const std::string, const std::string);

void QuickSort(std::vector<std::string>& procs, int left, int right, 
	int(*comp)(const std::string, const std::string));

int main(int argc, char** argv) {
	char db_name[STRBUFFER];
	char src_dir[STRBUFFER];
	char conn_string[STRBUFFER];
	bool db_is_set, src_is_set;
	db_is_set, src_is_set = false;

	// Skip the first argument as it's the name of the program
	argv++;

	// Get arguments from CMD
	for (int i = 1; i < argc; i++) {

		if (strcmp(*argv, "-n") == 0) {
			argv++;
			i++;

			if (strlen(*argv) > 0) {
				strcpy_s(db_name, *argv);
				argv++;
			}

			db_is_set = true;
		}

		else if (strcmp(*argv, "-s") == 0) {
			argv++;
			i++;

			if (strlen(*argv) > 0) {
				strcpy_s(src_dir, *argv);
				argv++;
			}

			src_is_set = true;
		}

		else {
			std::cout << "Unknown flag passed." << "\n";
			PrintUsage();
			exit(1);
		}
	}

	// Check to see what was set
	if (!db_is_set) {
		std::cout << "The database name was not specified." << "\n";
		PrintUsage();
		exit(1);
	}
	if (!src_is_set) {
		strcpy_s(src_dir, "./");
	}

	GetConnectionString(&conn_string[0]);

	// Setup DB connection and get all procs
	std::shared_ptr<std::vector<std::string>> all_procs = 
		std::make_shared<std::vector<std::string>>();
	ProcFetch^ proc_fetch = gcnew ProcFetch(conn_string, db_name);
	proc_fetch->GetProcs(all_procs);

	// Sort all of the procs, and pass them to the ProjectInfo class
	QuickSort(*all_procs, 0, all_procs->size() - 1, CompareStrings);
	ProjectInfo* proj = new ProjectInfo(all_procs, src_dir, db_name);

	// Report the findings and cleanup
	proj->PrintSummary();
	proj->~ProjectInfo();
}

void GetConnectionString(char* conn_string)  {
	std::ifstream file;

	file.open("./connectionString");

	if (!file) {
		std::cerr << "Unable to read connectionString." << std::endl;
		exit(1);
	}

	char c;
	while (file.get(c)) {
		*conn_string = c;
		conn_string++;
	}

	*conn_string = '\0';

	file.close();
}

void PrintUsage() {
	std::cout << "\n"
		<< "Usage: CheckProcs [-n <Database Name>] [-s <Source Directory>]" 
		<< std::endl;
}

int CompareStrings(const std::string first, const std::string second) {
	return first.compare(second);
}

void QuickSort(std::vector<std::string>& procs, int left, int right, 
	int(*comp)(const std::string, const std::string)) {

	int i = left, j = right;
	std::string tmp;
	std::string pivot = procs[(left + right) / 2];

	while (i <= j) {
		while (comp(procs[i], pivot) < 0) {
			i++;
		}

		while (comp(procs[j], pivot) > 0) {
			j--;
		}

		if (i <= j) {
			tmp = procs[i];
			procs[i] = procs[j];
			procs[j] = tmp;
			i++;
			j--;
		}

	};

	if (left < j) {
		QuickSort(procs, left, j, comp);
	}

	if (i < right) {
		QuickSort(procs, i, right, comp);
	}
}