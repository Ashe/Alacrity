// by Mark Featherstone (C) 2015 All Rights Reserved.
#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <vector>

#include "UtilsDefines.h"

/*
File handling can get messy and OS specific
Wrap up the most common tasks of basic reading and writing
Supply enough functionality to let the audio system do its work
*/
class File
{
public:
	enum { MPF_READ=1, MPF_WRITE=2, MPF_APPEND=4 };
	typedef std::vector<UDEF::utf8string> Names;
	File( const UDEF::utf8string &fileName, const int flags);
	~File() { close(); }
	//file size
	unsigned int getSize();
	//close it when finished if not relying on destructor
	bool close();
	//read some data into a buffer
	bool read( void * pBuffer, const unsigned int size, unsigned int &bytesRead );
	//write data into a buffer
	bool write( void const * pBuffer, const unsigned int size, unsigned int &bytesWritten );
	//change where in the file we are reading from
	void seek( const unsigned int pos );

	//look through a folder for files iwth a certain extension (*.wav or *.bin, etc)
	static const Names &findFiles( const UDEF::utf8string &path, const UDEF::utf8string &filterStr);
	//check if files and folders are actually there
	static bool fileExists( const UDEF::utf8string &fileName );
	static bool folderExists( const UDEF::utf8string &folderName);
	//if we just start writing or read a file, where will it end up?
	static void File::getCurrentFolder( UDEF::utf8string &folder );
	static bool File::setCurrentFolder( const UDEF::utf8string &folder, bool error=true );
	//remember where we were when the game started up
	static const UDEF::utf8string &getFirstRunDirectory() { return s_firstRunDirectory; }
	//do this once
	static void initialiseSystem();
	//get rid of a file
	static bool remove( const UDEF::utf8string &fileName );	
	//make a new folder on the fiel system
	static bool createFolder( const UDEF::utf16string &folderName );
	static bool createFolder( const UDEF::utf8string &folderName );

	//simplify writing/reading common data types
	static bool read( File &file, int &myInt );
	static bool write( File &file, const int myInt );



private:
	bool m_isOpen;				///<true if a file is open for reading/writing
	bool m_readAccess;			///<can read from file if true
	bool m_writeAccess;			///<can write to file if true
	UDEF::utf8string m_fileName;//keep the file name handy
	std::fstream *m_pFStream;
	
	static bool s_initialised;			//true if we initialised the file system
	static Names s_fileNames;			//if collecting file names in a folder, store them here	
	static UDEF::utf8string s_firstRunDirectory;	//our intial path
	static UDEF::utf16string s_firstRunDirectory16;	

	bool openFile(const UDEF::utf8string &fileName, int flags, std::fstream *&pTheStream, bool * const pReadAccessFlag = NULL, bool * const pWriteAccessFlag = NULL );

};


#endif