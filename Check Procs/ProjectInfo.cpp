#include "stdafx.h"
#include "ProjectInfo.h"
#include <string>
#include <iostream>
#include <sstream>

#define DBSUFFIX ".dbo."

ProjectInfo::ProjectInfo(std::vector<std::string>* procs, 
	const std::string path_to_proj, char* db_name) {

	procs_ = procs;
	original_proc_count_ = procs_->size();
	loc_ = 0;
	sloc_ = 0;
	file_count_ = 0;
	database_name_ = std::string(db_name) + DBSUFFIX;

	all_files_ = GetFilesFromDirectory(path_to_proj);

	file_count_ = all_files_.size();

	for (auto& p : all_files_) {
		CheckIfProcExistsInFile(p);
	}
}

ProjectInfo::~ProjectInfo() {
	procs_->~vector();
}

void ProjectInfo::PrintSummary() {
	std::cout << "\n";
	std::cout << "Summary: " << "\n";
	std::cout << "LOC: " << loc_ << "\n";
	std::cout << "SLOC: " << sloc_ << "\n";
	std::cout << "File Count: " << file_count_ << "\n";
	std::cout << "\n";

	std::cout << original_proc_count_ - procs_->size() << "/" <<
		original_proc_count_ << " procs used.\n\n";

	std::cout << "List of procs not used: \n\n";

	for (auto proc : *procs_)
		std::cout << proc << "\n";
}

inline void ProjectInfo::IncrementLoc() {
	loc_++;
}

inline void ProjectInfo::IncrementSlocAndLoc() {
	sloc_++;
	loc_++;
}

std::vector<fs::path> ProjectInfo::GetFilesFromDirectory(std::string path) {
	std::vector<fs::path> paths_from_directory;

	// Build a list of all files including those 
	// from sub directories
	for (auto& p : fs::directory_iterator(path)) {
		if (fs::is_directory(p)) {
			std::string sub_path = p.path().string();
			std::vector<fs::path> sub_folder_files = GetFilesFromDirectory(sub_path);

			paths_from_directory.insert(paths_from_directory.end(), 
				sub_folder_files.begin(), sub_folder_files.end());
		}
		else {
			paths_from_directory.push_back(p);
		}
	}

	return paths_from_directory;
}

void ProjectInfo::CheckIfProcExistsInFile(fs::path file) {
	std::ifstream currentFile;
	std::string line;

	currentFile.open(file.u8string());

	while (std::getline(currentFile, line)) {
		if (line.find(database_name_) != std::string::npos) {
			FindAndRemoveProcFromList(line);
			IncrementSlocAndLoc();
		}
		else {
			bool white_space_only = std::all_of(line.begin(), 
				line.end(), isspace);

			if (white_space_only) {
				IncrementLoc();
			}
			else {
				IncrementSlocAndLoc();
			}
		}
	}
}

void ProjectInfo::FindAndRemoveProcFromList(std::string line) {
	std::vector<std::string>::iterator i;

	size_t start_pos = line.find(database_name_);

	// If the proc exists in the vector of procs, remove it and continue
	for (i = procs_->begin(); i != procs_->end();) {
		std::string current_db_name = line
			.substr(start_pos, start_pos + i->length());

		if (i->compare(current_db_name)) {
			i = procs_->erase(i);
			break;
		}
		else {
			i++;
		}
	}
}