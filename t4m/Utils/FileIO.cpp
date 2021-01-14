/*
	This project is released under the GPL 2.0 license.
	Some parts are based on research by Bas Timmer and the OpenSteamworks project.
	Please do no evil.

	Initial author: (https://github.com/)Convery
	Started: 2014-11-24
	Notes:
		A collection of functions that make IO a little easier.
*/

#include "..\stdinc.h"

void FileIO::WriteFile(std::string Filename, std::string Buffer, bool Append)
{
	return FileIO::WriteFile(Filename, Buffer.data(), Buffer.size(), Append);
}
void FileIO::WriteFile(std::string Filename, std::basic_string<uint8_t> Buffer, bool Append)
{
	return FileIO::WriteFile(Filename, Buffer.data(), Buffer.size(), Append);
}
void FileIO::WriteFile(std::string Filename, const void *Data, uint32_t Length, bool Append)
{
	return FileIO::WriteFile(Filename.c_str(), Data, Length, Append);
}
void FileIO::WriteFile(const char *Filename, const void *Data, uint32_t Length, bool Append)
{
	FILE *File;
	char Path[255];

	// Workaround for portability.
	while (Filename[0] == ',' || Filename[0] == '.' || Filename[0] == ' ')
	{
		Filename++;
	}

	// Make the path Win32 compatible.
	strcpy(Path, Filename);
	for (int i = strlen(Filename); i > 0; i--)
	{
		if (Path[i] == '/')
		{
			Path[i] = '\\';
		}
	}

	// Terminate the path.
	for (int i = strlen(Filename); i > 0; i--)
	{
		if (Path[i] == '\\')
		{
			memset(Path + i, 0, 255 - i);

			// Create the path if it doesn't exist.
			FileIO::CreateDir(Path);
			break;
		}
	}

	// Create or open the file.
	File = fopen(Filename, (Append ? "ab" : "wb"));

	// If opened, write.
	if (File != nullptr)
	{
		fwrite(Data, 1, Length, File);
		fclose(File);
	}
}

bool FileIO::ListFiles(std::string Path, const char *Extension, std::vector<std::string> &Results)
{
	// ### WARNING ###
	// Win32 only.

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	bool Result = false;

	// Any filename.
	Path.append("\\*");
	Path.append(Extension);

	hFind = FindFirstFileA(Path.c_str(), &FindFileData);

	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				Results.push_back(FindFileData.cFileName);
				Result = true;
			}
		}
		while(FindNextFileA(hFind, &FindFileData));
		FindClose(hFind);
	}

	return Result;
}
const char *FileIO::GetCurrentDir()
{
	std::string Result;
	static char CWD[255];

	ZeroMemory(CWD, sizeof(CWD));
	_getcwd(CWD, 225);
	Result.append(CWD);

	return CWD;
}

uint32_t FileIO::GetFileSize(std::string Path)
{
	std::ifstream File(Path, std::ios::binary);
	std::streamsize Size = 0;

	File.seekg(0, std::ios::end);
	Size = File.tellg();
	File.seekg(0, std::ios::beg);

	return (uint32_t)Size;
}

void FileIO::DeleteFile(std::string Path)
{
	remove(Path.c_str());
}

bool FileIO::FileExists(std::string Path)
{
	FILE* fp;
	fopen_s(&fp, Path.c_str(), "r");

	if (fp)
	{
		fclose(fp);
		return true;
	}

	return false;
}

void FileIO::CreateDir(std::string Path, bool isFile)
{
	char opath[MAX_PATH];
	char *p;
	size_t len;

	if (isFile)
	{
		if (Path.find_last_of("/\\") != std::string::npos) Path = Path.substr(0, Path.find_last_of("/\\"));
		else return;
	}

	strncpy_s(opath, Path.c_str(), sizeof(opath));
	len = strlen(opath);

	if (opath[len - 1] == L'/')
	{
		opath[len - 1] = L'\0';
	}

	for (p = opath; *p; p++)
	{
		if (*p == L'/' || *p == L'\\')
		{
			*p = L'\0';

			if (_access(opath, 0))
			{
				_mkdir(opath);
			}

			*p = L'\\';
		}
	}

	if (_access(opath, 0))
	{
		_mkdir(opath);
	}
}

bool FileIO::ReadFileIntoBuffer(std::string Path, std::basic_string<uint8_t> &Buffer)
{
	std::ifstream File(Path, std::ios::binary);
	std::streamsize Size = 0;

	File.seekg(0, std::ios::end);
	Size = File.tellg();
	File.seekg(0, std::ios::beg);

	if(Size == -1)
	{
		File.close();
		return false;
	}

	Buffer.clear();
	Buffer.resize((uint32_t)Size);
	
	if(!File.read((char *)Buffer.data(), Size))
	{
		File.close();
		return false;
	}
	File.close();
	return true;
}

std::string FileIO::GetFileName(std::string path)
{
	if (path.find_last_of("/\\") != std::string::npos)
	{
		path = path.substr(path.find_last_of("/\\") + 1);
	}

	return path;
}
