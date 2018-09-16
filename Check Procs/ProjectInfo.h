#pragma once

#include <vector>
#include <experimental\filesystem>
#include <filesystem>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN) 
namespace fs = std::experimental::filesystem::v1;
#else
namespace fs = std::filesystem;
#endif

class ProjectInfo
{
public:
	ProjectInfo(std::shared_ptr<std::vector<std::string>> procs, 
		const std::string path_to_proj, char* db_name);
	~ProjectInfo();

	void PrintSummary();

private:
	std::shared_ptr<std::vector<std::string>> procs_;
	std::string database_name_;
	std::vector<fs::path> all_files_;
	size_t original_proc_count_;
	size_t file_count_;
	size_t loc_;
	size_t sloc_;

	std::vector<fs::path> GetFilesFromDirectory(std::string path);
	void CheckIfProcExistsInFile(fs::path file);
	void FindAndRemoveProcFromList(std::string line);
	inline void IncrementLoc();
	inline void IncrementSlocAndLoc();
};

