// by Mark Featherstone (C) 2015 All Rights Reserved.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sys/stat.h>
#include <io.h>

#include "File.h"
#include "Error.h"


using namespace UDEF;

bool File::s_initialised = false;
File::Names File::s_fileNames;
utf8string File::s_firstRunDirectory;	
utf16string File::s_firstRunDirectory16;	


void File::initialiseSystem()
{
	getCurrentFolder( s_firstRunDirectory );
	UDEF::convertUTF8toUTF16(s_firstRunDirectory16, s_firstRunDirectory);
	s_initialised = true;
}


File::File( const UDEF::utf8string &fileName, const int flags)
	: m_isOpen(false), 
	m_readAccess(false), 
	m_writeAccess(false), 
	m_fileName(fileName), 
	m_pFStream(NULL)
{
	MPOD_ASSERT_MSG( s_initialised, "File system was not initialised" );
	if( m_isOpen )
	{
		MPOD_VERIFY_MSG( 0, "file=%s - is already open", m_fileName.c_str() );
	}
	
	MPOD_VERIFY_MSG( openFile( m_fileName, flags, m_pFStream, &m_readAccess, &m_writeAccess ), "file=%s - cannot open", m_fileName.c_str() );
	m_isOpen = true;
}



bool File::read( void * const pBuffer, const unsigned int size, unsigned int &bytesRead )
{
	MPOD_ASSERT( m_isOpen );
	MPOD_ASSERT( m_readAccess );
	if( m_pFStream->fail() )
	{
		if(m_pFStream->eof())
			m_pFStream->clear();
		else
			MPOD_ASSERT_MSG(false, "stream failed reading %s.\n", m_fileName.c_str() );
	}
	m_pFStream->read(static_cast<char*>(pBuffer), static_cast<std::streamsize>(size));
	bytesRead = static_cast<unsigned int>(m_pFStream->gcount());
	if( bytesRead == 0 )
		return false;

	return true;
}


void File::seek( const unsigned int pos )
{
	MPOD_ASSERT( m_isOpen && pos >= 0 );
	m_pFStream->clear(); //Clear fail status in case eof was set
	m_pFStream->seekg(static_cast<std::streamoff>(pos), std::ios::beg);
}


unsigned int File::getSize()
{
	MPOD_ASSERT( m_isOpen );

	struct stat tagStat;
	int ret = stat(m_fileName.c_str(), &tagStat);
	MPOD_ASSERT_MSG(ret == 0, "Problem getting file size" );
	return tagStat.st_size;
}

bool File::close()
{
	if( m_isOpen == false )
		return false;

	MPOD_ASSERT(m_pFStream);
	m_pFStream->close();
	delete m_pFStream;
	m_isOpen = false;
	return true;
}


bool File::openFile(const utf8string &fileName, int flags, std::fstream *&pTheStream, bool * const pReadAccessFlag, bool * const pWriteAccessFlag )
{
	// Always open in binary mode
	pTheStream = new std::fstream();
	std::ios_base::open_mode mode = std::ios::binary;
	if( flags&File::MPF_APPEND )
		mode |= std::ios::app;
	if( flags&File::MPF_READ )
	{
		mode |= std::ios::in;
		if( pReadAccessFlag )
			*pReadAccessFlag = true;
	}
	else if( pReadAccessFlag )
	{
		*pReadAccessFlag = false;
	}
	if( flags&File::MPF_WRITE )
	{
		mode |= std::ios::out;
		if( pWriteAccessFlag )
			*pWriteAccessFlag = true;
	}
	else if( pWriteAccessFlag )
	{
		*pWriteAccessFlag = false;
	}
	pTheStream->open( fileName.c_str(), mode );

	// Should check ensure open succeeded, in case fail for some reason.
	if(pTheStream->fail())
	{
		delete pTheStream;
		pTheStream = NULL;
		return false;
	}
	return true;
}


bool File::remove( const utf8string &fileName )
{
	utf16string tmp;
	UDEF::convertUTF8toUTF16(tmp, fileName );
	if( _wremove( tmp.c_str() ) == 0 )
		return true;
	strerror_s(g_txt,MAX_TXT,errno);
	MPOD_VERIFY_MSG( 0, "%s - error deleting file", g_txt );
	return false;
}

bool File::createFolder( const utf8string &folderName )
{
	MPOD_VERIFY( !folderName.empty() );
	UDEF::convertUTF8toUTF16( g_wtxt, UDEF::MAX_TXT, folderName );
	if( CreateDirectoryW( g_wtxt, NULL ) )
		return true;
	DWORD dw = GetLastError();		   
	if( dw == ERROR_ALREADY_EXISTS )
		return true;
	return false;	
}

bool File::createFolder( const utf16string &folderName )
{
	MPOD_VERIFY( !folderName.empty() );
	if( CreateDirectoryW( folderName.c_str(), NULL ) )
		return true;
	DWORD dw = GetLastError();		   
	if( dw == ERROR_ALREADY_EXISTS )
		return true;
	return false;	
}


bool FileOrFolderExists(const utf8string &name)
{
	MPOD_VERIFY( !name.empty() );
	struct stat tagStat;
	bool ret = (stat(name.c_str(), &tagStat) == 0);
	return ret;
}

bool File::fileExists( const utf8string &fileName )
{
	return FileOrFolderExists(fileName);
}

bool File::folderExists( const utf8string &folderName )
{
	return FileOrFolderExists(folderName);
}

void File::getCurrentFolder( utf8string &folder )
{
	//find out what folder we've ended up in
	unsigned int nchars = GetCurrentDirectoryW( 0, NULL );
	static std::vector<utf16char> s_folderBuffer;
	nchars++;
	UDEF::growBuffer( s_folderBuffer, nchars, 1024 );	//make sure its big, don't want to keep changing its size
	unsigned int ret = GetCurrentDirectoryW( nchars, &s_folderBuffer[0] );
	MPOD_ASSERT( ret != 0 );
	UDEF::convertUTF16toUTF8( folder, s_folderBuffer );
}

bool File::setCurrentFolder( const utf8string &folder, bool error )
{ 
	static utf16string s_setFolderBuffer;
	UDEF::convertUTF8toUTF16( s_setFolderBuffer, folder );
	if( !SetCurrentDirectoryW(s_setFolderBuffer.c_str()) )	
	{
		if( error )
			MPOD_ASSERT(0);
		return false;
	}
	unsigned int sz = static_cast<unsigned int>(s_setFolderBuffer.length());
	return true;
}


const File::Names &File::findFiles( const UDEF::utf8string &path, const UDEF::utf8string &filterStr )
{
	//prepare to gether file names
	s_fileNames.clear();
	if( !path.empty() && !File::folderExists(path) )
		return s_fileNames;

	//remember where we were, then change the current folder to where these new files might be
	utf8string buff;
	File::getCurrentFolder( buff );
	if( !File::setCurrentFolder( path, false ) )
	{
		File::setCurrentFolder( buff );
		return s_fileNames;
	}

	//iterate over every file in the folder
	struct _wfinddata_t c_file;
	intptr_t hFile;
	utf16string fStr;
	UDEF::convertUTF8toUTF16( fStr, filterStr );
	if( (hFile = _wfindfirst( (wchar_t*)fStr.c_str(), &c_file )) == -1L )
	{
		File::setCurrentFolder( buff );
	}
	else
	{
		do
		{
			if( !(c_file.attrib&_A_SUBDIR) )
			{
				if(wcscmp(c_file.name,L".")==0 || wcscmp(c_file.name,L"..")==0)
					continue;
				//it's a real fiel so save the name
				utf8string fName;
				UDEF::convertUTF16toUTF8( fName, c_file.name );
				s_fileNames.push_back( fName );
			}
		}while(_wfindnext( hFile, &c_file ) == 0);
		_findclose( hFile );
		//set us back to where were before
		File::setCurrentFolder( buff );
	}
	return s_fileNames;
}

bool File::write( void const * pBuffer, const unsigned int size, unsigned int &bytesWritten )
{
	MPOD_ASSERT( m_isOpen );
	MPOD_ASSERT( m_writeAccess );
	MPOD_ASSERT( pBuffer );
	MPOD_ASSERT( size > 0 );

	MPOD_ASSERT( m_pFStream->good() );
	m_pFStream->write( static_cast<char const *>(pBuffer), static_cast<std::streamsize>(size) );
	if( !m_pFStream->good() )
	{
		MPOD_ASSERT(0);
		return false;
	}
	bytesWritten = size;
	return true;
}

//convenience functions
bool writeInternal( File &file, void *pData, unsigned int size )
{
	unsigned int bWritten = 0;
	if( !file.write( pData, size, bWritten ) ) 
	{
		MPOD_ASSERT(0);
		return false;
	}
	if( bWritten != size )
	{
		MPOD_ASSERT(0);	   
		return false;
	}
	return true;
}

bool File::write( File &file, const int myInt )
{
	return writeInternal( file, const_cast<int*>(&myInt), sizeof(myInt) );
} 

bool readInternal( File &file, void *pData, unsigned int size )
{
	unsigned int bRead = 0;
	if( !file.read( pData, size, bRead ) ) 
		return false;
	if( bRead != size )
		return false;
	return true;
}

bool File::read( File &file, int &myInt )
{
	return readInternal( file, &myInt, sizeof(myInt) );
}
