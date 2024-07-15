/*

Decoda
Copyright (C) 2007-2013 Unknown Worlds Entertainment, Inc. 

This file is part of Decoda.

Decoda is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Decoda is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Decoda.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Config.h"
#include "Project.h"
#include "XmlUtility.h"
#include "StlUtility.h"
#include "DebugFrontend.h"
#include "Symbol.h"

#include <wx/xml/xml.h>
#include <wx/filefn.h>

#include <algorithm>

unsigned int Project::s_lastFileId = 0;

wxString Project::File::GetDisplayName() const
{

    wxString displayName = fileName.GetFullName();
    
    if (displayName.IsEmpty())
    {
        displayName = tempName;
    }

    return displayName;

}

wxString Project::File::GetFileType() const
{
    if (!type.IsEmpty())
    {
        return type;
    }
    else
    {
        return fileName.GetExt();
    }
}

#ifdef _KOOK_DECODA_
wxString Project::Path::GetDisplayName() const
{
	return pathName.AfterLast('\\');
}

void ClearPath(Project::Path* path)
{
	for (unsigned int i = 0; i < path->paths.size(); ++i)
	{
		ClearPath(path->paths[i]);
	}
	ClearVector(path->paths);
	for (unsigned int i = 0; i < path->files.size(); ++i)
	{
		ClearVector(path->files[i]->symbols);
	}
	ClearVector(path->files);
}
#endif

Project::Project()
{
    m_needsSave     = false;
    m_needsUserSave = false;
    m_tempIndex = 0;
}

Project::~Project()
{
    
    for (unsigned int i = 0; i < m_files.size(); ++i)
    {
        ClearVector(m_files[i]->symbols);
    }    
    
    ClearVector(m_files);

#ifdef _KOOK_DECODA_
	for (unsigned int i = 0; i < m_paths.size(); ++i)
	{
		ClearPath(m_paths[i]);
	}
	ClearVector(m_paths);

	m_userfiles.clear();
	m_fileMap.clear();
#endif
}

wxString Project::GetName() const
{

    wxFileName fileName = m_fileName;
    wxString name = fileName.GetName();
    
    if (name.empty())
    {
        return "Untitled";
    }
    else
    {
        return name;
    }

}

bool Project::Save(const wxString& fileName)
{

    wxFileName userFileName(fileName);
    userFileName.SetExt("deuser");
    
    if (fileName != m_fileName)
    {
        m_needsSave = true;
        m_needsUserSave = true;
    }

    bool success = true;

    if (m_needsSave)
    {
        if (SaveGeneralSettings(fileName))
        {
            m_needsSave = false;
        }
        else
        {
            success = false;
        }
    }

    if (success && m_needsUserSave)
    {
        if (SaveUserSettings(userFileName.GetFullPath()))
        {
            m_needsUserSave = false;
        }
    }

    if (success)
    {
        m_fileName = fileName;
    }

    return success;

}

bool Project::Load(const wxString& fileName)
{

    if (!LoadGeneralSettings(fileName))
    {
        return false;
    }

    wxFileName userFileName(fileName);
    userFileName.SetExt("deuser");


    // Even if we can't load the user settings, we still treat the load as
    // successful. We do this since the user file may not be present.
    LoadUserSettings(userFileName.GetFullPath());

    m_fileName  = fileName;
    m_needsSave = false;
#ifdef _KOOK_DECODA_
	m_needsUserSave = false;
#endif

    return true;

}

#ifdef _KOOK_DECODA_
bool Project::SaveUserSettings()
{
	if (!m_needsUserSave)
		return false;

	wxFileName userFileName(m_fileName);
	userFileName.SetExt("deuser");

	if (SaveUserSettings(userFileName.GetFullPath())) {
		m_needsUserSave = false;
		return true;
	}

	return false;
}
#endif

const wxString& Project::GetFileName() const
{
    return m_fileName;
}

bool Project::GetNeedsSave() const
{
    return m_needsSave || m_needsUserSave;
}

const wxString& Project::GetCommandLine() const
{
    return m_commandLine;
}

void Project::SetCommandLine(const wxString& commandLine)
{
    m_commandLine = commandLine;
    m_needsUserSave = true;
}

const wxString& Project::GetCommandArguments() const
{
    return m_commandArguments;
}

void Project::SetCommandArguments(const wxString& commandArguments)
{
    m_commandArguments  = commandArguments;
    m_needsUserSave = true;
}

const wxString& Project::GetWorkingDirectory() const
{
    return m_workingDirectory;
}

void Project::SetWorkingDirectory(const wxString& workingDirectory)
{
    m_workingDirectory = workingDirectory;
    m_needsUserSave = true;
}

const wxString& Project::GetSymbolsDirectory() const
{
    return m_symbolsDirectory;
}

void Project::SetSymbolsDirectory(const wxString& symbolsDirectory)
{
    m_symbolsDirectory = symbolsDirectory;
    m_needsUserSave = true;
}

const wxString& Project::GetSccProvider() const
{
    return m_sccProvider;
}

void Project::SetSccProvider(const wxString& sccProvider)
{
    m_sccProvider = sccProvider;
    m_needsUserSave = true;
}

const wxString& Project::GetSccUser() const
{
    return m_sccUser;
}

void Project::SetSccUser(const wxString& sccUser)
{
    m_sccUser = sccUser;
    m_needsUserSave = true;
}

const wxString& Project::GetSccProjectName() const
{
    return m_sccProjName;
}

void Project::SetSccProjectName(const wxString& sccProjName)
{
    m_sccProjName = sccProjName;
    m_needsUserSave = true;
}

const wxString& Project::GetSccLocalPath() const
{
    return m_sccLocalPath;
}

void Project::SetSccLocalPath(const wxString& sccLocalPath)
{
    m_sccLocalPath = sccLocalPath;
    m_needsUserSave = true;
}

const wxString& Project::GetSccAuxProjectPath() const
{
    return m_sccAuxProjPath;
}

void Project::SetSccAuxProjectPath(const wxString& sccAuxProjPath)
{
    m_sccAuxProjPath = sccAuxProjPath;
    m_needsUserSave = true;
}

Project::File* Project::GetFile(unsigned int fileIndex)
{
    return m_files[fileIndex];
}

const Project::File* Project::GetFile(unsigned int fileIndex) const
{
    return m_files[fileIndex];
}

Project::File* Project::GetFileById(unsigned int fileId)
{
#ifdef _KOOK_DECODA_
	for (FILEMAP::iterator it = m_fileMap.begin(); it != m_fileMap.end(); ++it) {
		if (it->second->fileId == fileId)
			return it->second;
	}
#else
    for (unsigned int fileIndex = 0; fileIndex < m_files.size(); ++fileIndex)
    {
        if (m_files[fileIndex]->fileId == fileId)
        {
            return m_files[fileIndex];
        }
    }
#endif
    return NULL;

}

unsigned int Project::GetNumFiles() const
{
    return m_files.size();
}

Project::File* Project::AddFile(const wxString& fileName)
{

    // Check if the file is already in the project.
#ifdef _KOOK_DECODA_
	FILEMAP::iterator it = m_fileMap.find(fileName.Lower());
	if (it != m_fileMap.end()) {
		if (!it->second->temporary)
			return NULL;
	}
#else
    for (unsigned int i = 0; i < m_files.size(); ++i)
    {
        if (!m_files[i]->temporary && m_files[i]->fileName.SameAs(fileName))
        {
            return NULL;
        }
    }
#endif

    File* file = new File;

    file->state         = CodeState_Normal;
    file->scriptIndex   = -1;
    file->temporary     = false;
    file->fileName      = fileName;
    file->status        = Status_None;
    file->fileId        = ++s_lastFileId;
#ifdef _KOOK_DECODA_
	file->path			= NULL;
	file->opened		= false;
	file->used			= false;
#endif

    if (fileName.IsEmpty())
    {
        file->tempName = CreateTempName();
    }

    m_files.push_back(file);
    m_needsSave = true;

#ifdef _KOOK_DECODA_
	m_fileMap.emplace(fileName.Lower(), file);
#endif
    
    return file;

}

Project::File* Project::AddTemporaryFile(unsigned int scriptIndex)
{
    
    const DebugFrontend::Script* script = DebugFrontend::Get().GetScript(scriptIndex);
    assert(script != NULL);

#ifdef _KOOK_DECODA_
	wxFileName fileName = script->filename.c_str();
	if (fileName.IsOk()) {
		if (!fileName.IsAbsolute())
			fileName.MakeAbsolute(m_workingDirectory);

		FILEMAP::iterator it = m_fileMap.find(fileName.GetFullPath().Lower());
		if (it != m_fileMap.end()) {
			File* file = it->second;
			if (file->scriptIndex == -1) {
				CheckAddUserFile(file);
				file->scriptIndex = scriptIndex;
				return file;
			}
		}
	}
#endif

    File* file = new File;

    file->state         = script->state;
    file->scriptIndex   = scriptIndex;
    file->temporary     = true;
#ifdef _KOOK_DECODA_
	file->fileName		= fileName;
#else
    file->fileName      = script->filename.c_str();
#endif
    file->status        = Status_None;
    file->fileId        = ++s_lastFileId;
#ifdef _KOOK_DECODA_
	file->path			= NULL;
	file->opened		= false;
#endif

    m_files.push_back(file);

#ifdef _KOOK_DECODA_
	if (fileName.IsOk() && fileName.IsAbsolute())
	{
		m_fileMap.emplace(fileName.GetFullPath().Lower(), file);
	}
	file->used = true;
	m_userfiles.push_back(file);
#endif

    return file;

}

Project::File* Project::AddTemporaryFile(const wxString& fileName)
{
    
    File* file = new File;

    file->state         = CodeState_Normal;
    file->scriptIndex   = -1;
    file->temporary     = true;
    file->fileName      = fileName;
    file->status        = Status_None;
    file->fileId        = ++s_lastFileId;
#ifdef _KOOK_DECODA_
	file->path			= NULL;
	file->opened		= false;
	file->used			= false;
#endif

    if (fileName.IsEmpty())
    {
        file->tempName = CreateTempName();
    }

    m_files.push_back(file);

    return file;

}

void Project::RemoveFile(File* file)
{

    std::vector<File*>::iterator iterator = m_files.begin();

    while (iterator != m_files.end())
    {
        if (file == *iterator)
        {
            m_files.erase(iterator);
            if (!file->temporary)
            {
                m_needsSave = true;
                m_needsUserSave = true;
            }
            break;
        }
        ++iterator;
    }
    
    ClearVector(file->symbols);

#ifdef _KOOK_DECODA_
	iterator = m_userfiles.begin();
	while (iterator != m_userfiles.end())
	{
		if (file == *iterator) {
			m_userfiles.erase(iterator);
			break;
		}
		++iterator;
	}
#endif

    delete file;

}

#ifdef _KOOK_DECODA_
Project::Path* Project::GetPath(unsigned int fileIndex)
{
	return m_paths[fileIndex];
}

const Project::Path* Project::GetPath(unsigned int fileIndex) const
{
	return m_paths[fileIndex];
}

unsigned int Project::GetNumPaths() const
{
	return m_paths.size();
}

void Project::AddFiles2Path(const wxString& pathName, Project::Path* path)
{
	WIN32_FIND_DATA data;
	wxString findstr = pathName + "\\*.";
	HANDLE handle = FindFirstFile(findstr, &data);
	if (handle == NULL)
		return;

	Path* newpath;
	do {
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0) {
				newpath = new Path;
				newpath->pathName = pathName + "\\" + data.cFileName;
				newpath->status = PathStatus_None;

				path->paths.push_back(newpath);

				AddFiles2Path(newpath->pathName, newpath);
			}
		}
	} while (FindNextFile(handle, &data));
	FindClose(handle);

	findstr = pathName  + "\\*.lua";
	handle = FindFirstFile(findstr, &data);
	if (handle == NULL)
		return;
	
	File* newfile;
	FILEMAP::iterator it;
	wxString fileName;
	do {
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
			fileName = pathName + "\\" + data.cFileName;

			it = m_fileMap.find(fileName.Lower());
			if (it != m_fileMap.end()) {
				newfile = it->second;
				newfile->temporary = false;

				for (std::vector<File*>::iterator it2 = m_files.begin(); it2 != m_files.end(); ++it2) {
					if (*it2 == newfile) {
						m_files.erase(it2);
						break;
					}
				}
			}
			else {
				newfile = new File;
				newfile->fileName = fileName;
				newfile->state = CodeState_Normal;
				newfile->scriptIndex = -1;
				newfile->temporary = false;
				newfile->status = Status_None;
				newfile->fileId = ++s_lastFileId;
				newfile->path = path;
				newfile->opened = false;
				newfile->used = false;

				m_fileMap.emplace(fileName.Lower(), newfile);
			}

			path->files.push_back(newfile);
		}
	} while (FindNextFile(handle, &data));
	FindClose(handle);
}

Project::Path* Project::AddPath(const wxString& pathName)
{
	for (unsigned int i = 0; i < m_paths.size(); ++i)
	{
		if (m_paths[i]->pathName == pathName)
		{
			return NULL;
		}
	}

	Path* path = new Path;
	path->pathName = pathName;
	path->status = PathStatus_None;

	m_paths.push_back(path);
	m_needsSave = true;

	AddFiles2Path(pathName, path);

	return path;
}

unsigned int Project::GetNumUserFiles() const
{
	return m_userfiles.size();
}

Project::File* Project::GetUserFile(unsigned int fileIndex)
{
	return m_userfiles[fileIndex];
}

const Project::File* Project::GetUserFile(unsigned int fileIndex) const
{
	return m_userfiles[fileIndex];
}

void Project::setFileScriptIndex(Project::File* file, unsigned scriptIndex)
{
	CheckAddUserFile(file);
	file->scriptIndex = scriptIndex;
}

void Project::OpenFile(Project::File* file)
{
	if (file->opened)
		return;
	CheckAddUserFile(file);
	file->opened = true;
	m_needsUserSave = true;
}

void Project::CloseFile(Project::File* file)
{
	if (!file->opened)
		return;
	file->opened = false;
	CheckDelUserFile(file);

	if (m_curOpenFileName == file->fileName.GetFullPath().Lower())
		m_curOpenFileName.Clear();
	m_needsUserSave = true;
}

Project::File* Project::GetCurFile()
{
	if (m_curOpenFileName.IsEmpty())
		return NULL;

	FILEMAP::iterator it = m_fileMap.find(m_curOpenFileName);
	if (it == m_fileMap.end()) {
		m_curOpenFileName.Clear();
		return NULL;
	}

	return it->second;
}

void Project::SetCurFile(Project::File* file)
{
	if (file == NULL) {
		if (m_curOpenFileName.IsEmpty())
			return;
		m_curOpenFileName.Clear();
		m_needsUserSave = true;
		return;
	}

	wxString filename = file->fileName.GetFullPath().Lower();
	if (filename == m_curOpenFileName)
		return;

	m_curOpenFileName = filename;
	m_needsUserSave = true;
}

void Project::CheckAddUserFile(File* file)
{
	if (file->path && !file->used) {
		m_userfiles.push_back(file);
		m_needsUserSave = true;
		file->used = true;
	}
}

void Project::CheckDelUserFile(File* file)
{
	if (file->path && !file->used) {
		std::remove(m_userfiles.begin(), m_userfiles.end(), file);
		m_needsUserSave = true;
		file->used = false;
	}
}

Project::File* Project::FildFile(const wxString& fileName)
{
	FILEMAP::iterator it = m_fileMap.find(fileName);
	if (it != m_fileMap.end()) {
		return it->second;
	}

	return NULL;
}
#endif

void Project::CleanUpAfterSession()
{
#ifdef _KOOK_DECODA_
	for (unsigned int i = 0; i < m_userfiles.size(); ++i)
	{
		m_userfiles[i]->scriptIndex = -1;
	}
#else
    for (unsigned int i = 0; i < m_files.size(); ++i)
    {
        m_files[i]->scriptIndex = -1;
    }
#endif
}

Project::File* Project::GetFileForScript(unsigned int scriptIndex) const
{
#ifdef _KOOK_DECODA_
	for (unsigned int i = 0; i < m_userfiles.size(); ++i)
	{
		if (m_userfiles[i]->scriptIndex == scriptIndex)
		{
			return m_userfiles[i];
		}
	}
#else
    for (unsigned int i = 0; i < m_files.size(); ++i)
    {
        if (m_files[i]->scriptIndex == scriptIndex)
        {
            return m_files[i];
        }
    }
#endif
    return NULL;

}

Project::File* Project::GetFileForFileName(const wxFileName& fileName) const
{
#ifdef _KOOK_DECODA_
	FILEMAP::const_iterator it = m_fileMap.find(fileName.GetFullPath().Lower());
	if (it != m_fileMap.end()) {
		return it->second;
	}
#else
    for (unsigned int i = 0; i < m_files.size(); ++i)
    {
        if (m_files[i]->fileName.GetFullName().CmpNoCase(fileName.GetFullName()) == 0)
        {
            return m_files[i];
        }
    }
#endif
    return NULL;

}

void Project::SetBreakpoint(unsigned int scriptIndex, unsigned int line, bool set)
{
#ifdef _KOOK_DECODA_
	File* file;
	for (unsigned int i = 0; i < m_userfiles.size(); ++i)
	{
		file = m_userfiles[i];
        if (file->scriptIndex == scriptIndex)
        {

            std::vector<unsigned int>::iterator iterator;
            iterator = std::find(file->breakpoints.begin(), file->breakpoints.end(), line);

            if (set)
            {
                if (iterator == file->breakpoints.end())
                {
					file->breakpoints.push_back(line);
                    if (!file->temporary)
                    {
                        m_needsUserSave = true;
                    }
                }
            }
            else
            {
                if (iterator != file->breakpoints.end())
                {
					file->breakpoints.erase(iterator);
                    if (!file->temporary)
                    {
                        m_needsUserSave = true;
                    }
                }
            }

        }
    }
#else
    for (unsigned int i = 0; i < m_files.size(); ++i)
    {
        if (m_files[i]->scriptIndex == scriptIndex)
        {

            std::vector<unsigned int>::iterator iterator;
            iterator = std::find(m_files[i]->breakpoints.begin(), m_files[i]->breakpoints.end(), line);

            if (set)
            {
                if (iterator == m_files[i]->breakpoints.end())
                {
                    m_files[i]->breakpoints.push_back(line);
                    if (!m_files[i]->temporary)
                    {
                        m_needsUserSave = true;
                    }
                }
            }
            else
            {
                if (iterator != m_files[i]->breakpoints.end())
                {
                    m_files[i]->breakpoints.erase(iterator);
                    if (!m_files[i]->temporary)
                    {
                        m_needsUserSave = true;
                    }
                }
            }

        }
    }
#endif
}

bool Project::ToggleBreakpoint(File* file, unsigned int line)
{
#ifdef _KOOK_DECODA_
	bool set = false;
	bool found = false;
	std::vector<unsigned int>::iterator iterator;
	for (iterator = file->breakpoints.begin(); iterator != file->breakpoints.end(); iterator++) {
		if (*iterator == line) {
			file->breakpoints.erase(iterator);
			set = false;
			CheckDelUserFile(file);
			found = true;
			break;
		}
		if (line < *iterator) {
			CheckAddUserFile(file);
			file->breakpoints.insert(iterator, line);
			set = true;
			found = true;
			break;
		}
	}
	if (!found) {
		CheckAddUserFile(file);
		file->breakpoints.push_back(line);
		set = true;
	}
#else
    std::vector<unsigned int>::iterator iterator;
    iterator = std::find(file->breakpoints.begin(), file->breakpoints.end(), line);

    bool set = false;

    if (iterator == file->breakpoints.end())
    {
        file->breakpoints.push_back(line);
        set = true;
    }
    else
    {
        file->breakpoints.erase(iterator);
        set = false;
	}
#endif

    if (!file->temporary)
    {
        m_needsUserSave = true;
    }

    return set;

}

void Project::DeleteAllBreakpoints(File* file)
{

    if (!file->breakpoints.empty())
    {
        
        file->breakpoints.clear();
    
        if (!file->temporary)
        {
            m_needsUserSave = true;
        }    

    }

}

wxXmlNode* Project::SaveFileNode(const wxString& baseDirectory, const File* file)
{

    // Temporary nodes should not be saved.
    assert(!file->temporary);

    wxFileName fileName = file->fileName;
    fileName.MakeRelativeTo(baseDirectory);

    wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE, "file");
    node->AddChild(WriteXmlNode("filename", fileName.GetFullPath()));

    return node;

}

wxXmlNode* Project::SaveUserFileNode(const wxString& baseDirectory, const File* file) const
{

    wxXmlNode* fileNode = NULL;

    if (!file->breakpoints.empty() && !file->temporary)
    {

        wxFileName fileName = file->fileName;
        fileName.MakeRelativeTo(baseDirectory);

        fileNode = new wxXmlNode(wxXML_ELEMENT_NODE, "file");
        fileNode->AddChild(WriteXmlNode("filename", fileName.GetFullPath()));

        for (unsigned int i = 0; i < file->breakpoints.size(); ++i)
        {
            wxXmlNode* breakpointNode = new wxXmlNode(wxXML_ELEMENT_NODE, "breakpoint"); 
            breakpointNode->AddChild(WriteXmlNode("line", file->breakpoints[i]));
            fileNode->AddChild(breakpointNode);
        }

    }
#ifdef _KOOK_DECODA_
	if (file->opened)
	{
		if (fileNode == NULL) {
			wxFileName fileName = file->fileName;
			fileName.MakeRelativeTo(baseDirectory);

			fileNode = new wxXmlNode(wxXML_ELEMENT_NODE, "file");
			fileNode->AddChild(WriteXmlNode("filename", fileName.GetFullPath()));
		}

		fileNode->AddChild(WriteXmlNodeBool("opened", true));
	}
#endif

    return fileNode;

}

bool Project::LoadUserFilesNode(const wxString& baseDirectory, wxXmlNode* root)
{

    if (root->GetName() != "files")
    {
        return false;
    }

    wxXmlNode* node = root->GetChildren();
    
    while (node != NULL)
    {
        LoadUserFileNode(baseDirectory, node);
        node = node->GetNext();
    }

    return true;

}

#ifdef _KOOK_DECODA_
void Project::SetPathStatus(Path* path, PathStatus status)
{
	if (path->status == status)
		return;
	path->status = status;
	m_needsUserSave = true;
}

wxXmlNode* Project::SavePathNode(const wxString& baseDirectory, const Path* path)
{
	wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE, "path");
	node->AddChild(WriteXmlNode("pathname", path->pathName));
	node->AddChild(WriteXmlNode("status", (unsigned int)path->status));

	return node;
}

bool Project::LoadPathNode(const wxString& baseDirectory, wxXmlNode* node)
{
	if (node->GetName() != "path")
	{
		return false;
	}

	Path* path = new Path;
	path->status = PathStatus_None;

	wxXmlNode* child = node->GetChildren();
	wxString pathName;

	while (child != NULL)
	{
		if (ReadXmlNode(child, "pathname", pathName))
		{
			path->pathName = pathName;
		}
		child = child->GetNext();
	}

	m_paths.push_back(path);

	AddFiles2Path(pathName, path);
	return true;

}

wxXmlNode* Project::SaveUserPathNode(const wxString& baseDirectory, const Path* path) const
{

	wxXmlNode* pathNode = NULL;

	if (path->status != PathStatus_None)
	{
		pathNode = new wxXmlNode(wxXML_ELEMENT_NODE, "path");
		pathNode->AddChild(WriteXmlNode("pathname", path->pathName));
		pathNode->AddChild(WriteXmlNode("status", (unsigned int)path->status));
	}

	for (unsigned int i = 0; i < path->paths.size(); ++i) {
		wxXmlNode* subnode = SaveUserPathNode(baseDirectory, path->paths[i]);
		if (subnode) {
			if (pathNode == NULL) {
				pathNode = new wxXmlNode(wxXML_ELEMENT_NODE, "path");
				pathNode->AddChild(WriteXmlNode("pathname", path->pathName));
			}
			pathNode->AddChild(subnode);
		}
	}

	return pathNode;

}

bool Project::LoadUserPathNode(const wxString& baseDirectory, wxXmlNode* root, Path* parent)
{
	if (root->GetName() != "path")
	{
		return false;
	}

	wxString pathName;
	unsigned int status;
	wxXmlNode* node = root->GetChildren();

	Path* path = NULL;
	while (node != NULL)
	{
		if (path == NULL) {
			if (ReadXmlNode(node, wxT("pathname"), pathName))
			{
				if (parent == NULL) {
					for (unsigned int i = 0; i < m_paths.size(); ++i) {
						if (m_paths[i]->pathName == pathName) {
							path = m_paths[i];
							break;
						}
					}
				}
				else {
					for (unsigned int i = 0; i < parent->paths.size(); ++i) {
						if (parent->paths[i]->pathName == pathName) {
							path = parent->paths[i];
							break;
						}
					}
				}
			}
			if (path)
				node = root->GetChildren();
			else
				node = node->GetNext();
			continue;
		}

		if (ReadXmlNode(node, wxT("status"), status))
		{
			path->status = (PathStatus)status;
		}
		else
			LoadUserPathNode(baseDirectory, node, path);

		node = node->GetNext();
	}

	if (path == NULL)
		return false;

	return true;

}

bool Project::LoadUserPathsNode(const wxString& baseDirectory, wxXmlNode* root)
{

	if (root->GetName() != "paths")
	{
		return false;
	}

	wxXmlNode* node = root->GetChildren();

	while (node != NULL)
	{
		LoadUserPathNode(baseDirectory, node);
		node = node->GetNext();
	}

	return true;

}
#endif

bool Project::LoadUserFileNode(const wxString& baseDirectory, wxXmlNode* root)
{
    
    if (root->GetName() != "file")
    {
        return false;
    }

    wxString fileName;
    std::vector<unsigned int> breakpoints;

    wxXmlNode* node = root->GetChildren();
#ifdef _KOOK_DECODA_
	bool opened = false;
#endif

    while (node != NULL)
    {

        ReadXmlNode(node, wxT("filename"), fileName)
#ifdef _KOOK_DECODA_
		|| ReadXmlNode(node, wxT("opened"), opened)
#endif
        || LoadBreakpointNode(node, breakpoints);

        node = node->GetNext();
    }
    
    if (!fileName.IsEmpty())
    {

        wxFileName temp = fileName;

        if (temp.IsRelative())
        {
            temp.MakeAbsolute(baseDirectory);
        }

        Project::File* file = NULL;

#ifdef _KOOK_DECODA_
		FILEMAP::iterator it = m_fileMap.find(temp.GetFullPath().Lower());
		if (it != m_fileMap.end()) {
			file = it->second;
		}
#else
        for (unsigned int fileIndex = 0; fileIndex < m_files.size(); ++fileIndex)
        {
            if (m_files[fileIndex]->fileName == temp)
            {
                file = m_files[fileIndex];
                break;
            }
        }
#endif

        if (file != NULL)
        {
            file->breakpoints = breakpoints;
#ifdef _KOOK_DECODA_
			std::sort(file->breakpoints.begin(), file->breakpoints.end());
			file->opened = opened;

			if (!file->used) {
				file->used = true;
				m_userfiles.push_back(file);
			}
#endif
        }

    }

    return true;

}

bool Project::LoadBreakpointNode(wxXmlNode* root, std::vector<unsigned int>& breakpoints)
{

    if (root->GetName() != "breakpoint")
    {
        return false;
    }

    wxXmlNode* node = root->GetChildren();

    while (node != NULL)
    {

        unsigned int line;

        if (ReadXmlNode(node, wxT("line"), line))
        {
            breakpoints.push_back(line);
        }

        node = node->GetNext();
    
    }

    return true;

}

bool Project::LoadFileNode(const wxString& baseDirectory, wxXmlNode* node)
{

    if (node->GetName() != "file")
    {
        return false;
    }

    File* file = new File;

    file->state         = CodeState_Normal;
    file->scriptIndex   = -1;
    file->temporary     = false;
    file->status        = Status_None;
    file->fileId        = ++s_lastFileId;
#ifdef _KOOK_DECODA_
	file->path			= NULL;
	file->opened		= false;
	file->used			= false;
#endif

    wxXmlNode* child = node->GetChildren();
    wxString fileName;

    while (child != NULL)
    {
        if (ReadXmlNode(child, "filename", fileName))
        {
            
            file->fileName = fileName;

            if (file->fileName.IsRelative())
            {
                file->fileName.MakeAbsolute(baseDirectory);
            }

            wxString temp = file->fileName.GetFullPath();
            int a  =0;

            
        }
        child = child->GetNext();
    }

    m_files.push_back(file);
    return true;

}

std::vector<Project::File*> Project::GetSortedFileList()
{
	struct SortByDisplayName
    {
        bool operator()(const File* file1, const File* file2) const
        {
            return file1->GetDisplayName() < file2->GetDisplayName();
        }
    };

	// Now build a copy of m_files and sort it by file name
	std::vector<Project::File*> fileList(m_files);

	std::sort(fileList.begin(), fileList.end(), SortByDisplayName());

	return fileList;
}

bool Project::SaveGeneralSettings(const wxString& fileName)
{

    // Disable logging.
    wxLogNull logNo;

    wxString baseDirectory = wxFileName(fileName).GetPath();
    
    wxXmlDocument document;
    
    wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "project");
    document.SetRoot(root);

	// Sort file names before saving, to lessen conflicts when synching to source control
	std::vector<Project::File*> sortedFileList = GetSortedFileList();
	assert(sortedFileList.size() == m_files.size());

	// Now save them out
    for (unsigned int i = 0; i < sortedFileList.size(); ++i)
    {
		File* file = sortedFileList[i];
		assert(file != NULL);

        if (!file->temporary)
        {
            wxXmlNode* node = SaveFileNode(baseDirectory, file);
            root->AddChild(node);
        }
    }

#ifdef _KOOK_DECODA_
	for (unsigned int pathIndex = 0; pathIndex < m_paths.size(); ++pathIndex)
	{
		const Path* path = m_paths[pathIndex];
		wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE, path->GetDisplayName());
		if (node != NULL)
		{
			wxXmlNode* node = SavePathNode(baseDirectory, path);
			root->AddChild(node);
		}
	}
#endif

    return document.Save(fileName);

}

bool Project::SaveUserSettings(const wxString& fileName)
{

    // Disable logging.
    wxLogNull logNo;

    wxXmlDocument document;
    
    wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "project");
    document.SetRoot(root);

#ifndef DEDICATED_PRODUCT_VERSION
    root->AddChild(WriteXmlNode("command",              m_commandLine));
    root->AddChild(WriteXmlNode("working_directory",    m_workingDirectory));
    root->AddChild(WriteXmlNode("symbols_directory",    m_symbolsDirectory));
#endif
    root->AddChild(WriteXmlNode("command_arguments",    m_commandArguments));
#ifdef _KOOK_DECODA_
	if (!m_curOpenFileName.IsEmpty())
		root->AddChild(WriteXmlNode("cur_file", m_curOpenFileName));
#endif

    // Add the source control settings.
    
    wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE, "scc");

    node->AddChild(WriteXmlNode("provider", m_sccProvider));
    node->AddChild(WriteXmlNode("user", m_sccUser));
    node->AddChild(WriteXmlNode("project_name", m_sccProjName));
    node->AddChild(WriteXmlNode("local_path", m_sccLocalPath));
    node->AddChild(WriteXmlNode("aux_project_path", m_sccAuxProjPath));

    root->AddChild(node);

    wxString baseDirectory = wxFileName(fileName).GetPath();

#ifdef _KOOK_DECODA_
	wxXmlNode* pathsNode = NULL;

	for (unsigned int pathIndex = 0; pathIndex < m_paths.size(); ++pathIndex)
	{
		const Path* path = m_paths[pathIndex];
		wxXmlNode* node = SaveUserPathNode(baseDirectory, path);
		if (node != NULL)
		{
			if (pathsNode == NULL)
			{
				pathsNode = new wxXmlNode(wxXML_ELEMENT_NODE, "paths");
			}
			pathsNode->AddChild(node);
		}
	}

	if (pathsNode != NULL)
	{
		root->AddChild(pathsNode);
	}
#endif

    wxXmlNode* filesNode = NULL;

#ifdef _KOOK_DECODA_
    for (unsigned int fileIndex = 0; fileIndex < m_userfiles.size(); ++fileIndex)
    {
        const File* file = m_userfiles[fileIndex];
#else
	for (unsigned int fileIndex = 0; fileIndex < m_files.size(); ++fileIndex)
	{
		const File* file = m_files[fileIndex];
#endif
        wxXmlNode* node = SaveUserFileNode(baseDirectory, file);
        if (node != NULL)
        {
            if (filesNode == NULL)
            {
                filesNode = new wxXmlNode(wxXML_ELEMENT_NODE, "files");
            }
            filesNode->AddChild(node);
        }
    }

    if (filesNode != NULL)
    {
        root->AddChild(filesNode);
    }

    return document.Save(fileName);

}

bool Project::LoadUserSettings(const wxString& fileName)
{

    // Disable logging.
    wxLogNull logNo;

    wxXmlDocument document;

    if (!document.Load(fileName))
    {
        return false;
    }

    wxXmlNode* root = document.GetRoot();
    
    if (root->GetName() != "project")
    {
        return false;
    }

    wxString baseDirectory = wxFileName(fileName).GetPath();

    wxXmlNode* node = root->GetChildren();
    
    while (node != NULL)
    {

           ReadXmlNode(node, "command_arguments",   m_commandArguments)
#ifndef DEDICATED_PRODUCT_VERSION
        || ReadXmlNode(node, "command",             m_commandLine)
        || ReadXmlNode(node, "working_directory",   m_workingDirectory)
        || ReadXmlNode(node, "symbols_directory",   m_symbolsDirectory)
#endif
        || LoadSccNode(node)
#ifdef _KOOK_DECODA_
        || LoadUserFilesNode(baseDirectory, node)
		|| ReadXmlNode(node, "cur_file", m_curOpenFileName)
		|| LoadUserPathsNode(baseDirectory, node);
#else
		|| LoadUserFilesNode(baseDirectory, node);
#endif
        
        node = node->GetNext();

    }

    return true;

}

bool Project::LoadGeneralSettings(const wxString& fileName)
{

    // Disable logging.
    wxLogNull logNo;

    wxString baseDirectory = wxFileName(fileName).GetPath();

    wxXmlDocument document;

    if (!document.Load(fileName))
    {
        return false;
    }

    wxXmlNode* root = document.GetRoot();
    
    if (root->GetName() != "project")
    {
        return false;
    }
    
    wxXmlNode* node = root->GetChildren();
    
    while (node != NULL)
    {
#ifdef _KOOK_DECODA_
        LoadFileNode(baseDirectory, node)
            || LoadPathNode(baseDirectory, node);
#else
        LoadFileNode(baseDirectory, node);
#endif
        node = node->GetNext();
    }

    return true;

}

wxString Project::CreateTempName()
{
    ++m_tempIndex;
    return wxString::Format("Untitled%d", m_tempIndex);
}

bool Project::LoadSccNode(wxXmlNode* root)
{

    if (root->GetName() != "scc")
    {
        return false;
    }

    wxXmlNode* node = root->GetChildren();
    
    while (node != NULL)
    {
           ReadXmlNode(node, "provider",          m_sccProvider)
        || ReadXmlNode(node, "user",              m_sccUser)
        || ReadXmlNode(node, "project_name",      m_sccProjName)
        || ReadXmlNode(node, "local_path",        m_sccLocalPath)
        || ReadXmlNode(node, "aux_project_path",  m_sccAuxProjPath);
        
        node = node->GetNext();

    }

    return true;

}