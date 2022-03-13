#include "DatabaseAccess.h"

bool DatabaseAccess::open()
{
    int doesFileExist = _access(DBNAME.c_str(), 0);
	int res = sqlite3_open(DBNAME.c_str(), &this->db);

	if (res != SQLITE_OK)
	{
		this->db = nullptr;
		std::cerr << "Failed to open DB" << std::endl;
		return false;
	}

	if (doesFileExist != 0)
	{
		std::string sqlStatedment = "CREATE TABLE Users(ID INTEGER PRIMARY KEY not NULL,Name TEXT NOT NULL);"; //users table
		sqlStatedment += "CREATE TABLE Albums(ID INTEGER PRIMARY KEY not NULL,Name TEXT NOT NULL,Creation_date TEXT NOT NULL,User_ID INTEGER NOT NULL,FOREIGN KEY (User_ID) REFERENCES Users(ID));"; //Albums Table
		sqlStatedment += "CREATE TABLE Pictures(ID INTEGER PRIMARY KEY not NULL,Name TEXT NOT NULL,Location TEXT NOT NULL,Creation_date TEXT NOT NULL,Album_ID INTEGER NOT NULL,FOREIGN KEY (Album_ID) REFERENCES Albums(ID));"; //Pictures Table
		sqlStatedment += "CREATE TABLE Tags(ID INTEGER PRIMARY KEY not NULL,User_ID INTEGER NOT NULL,Picture_ID INTEGER NOT NULL,FOREIGN KEY (User_ID) REFERENCES Users(ID),FOREIGN KEY (Picture_ID) REFERENCES Pictures(ID));"; //Tags Table
		char* errMessage = nullptr;
		res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
			return false;
	}
	return true;
}

void DatabaseAccess::close()
{
	sqlite3_close(this->db);
	this->db = nullptr;
}

void DatabaseAccess::closeAlbum(Album& pAlbum)
{
}
