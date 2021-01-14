/*
	This project is released under the GPL 2.0 license.
	Some parts are based on research by Bas Timmer and the OpenSteamworks project.
	Please do no evil.

	Initial author: (https://github.com/)Convery
	Started: 2014-11-24
	Notes:
		A collection of functions that make IO a little easier.
*/

#pragma once
class FileIO
{
public:
	static void WriteFile(std::string Filename, std::string Buffer, bool Append = false);
	static void WriteFile(std::string Filename, std::basic_string<uint8_t> Buffer, bool Append = false);
	static void WriteFile(std::string Filename, const void *Data, uint32_t Length, bool Append = false);
	static void WriteFile(const char *Filename, const void *Data, uint32_t Length, bool Append = false);

	static bool ListFiles(std::string Path, const char *Extension, std::vector<std::string> &Results);
	static const char *GetCurrentDir();

	static uint32_t GetFileSize(std::string Path);
	static bool ReadFileIntoBuffer(std::string Path, std::basic_string<uint8_t> &Buffer);

	static void DeleteFile(std::string Path);
	static bool FileExists(std::string Path);

	static void CreateDir(std::string Path, bool isFile = false);

	static std::string GetFileName(std::string path);
};
