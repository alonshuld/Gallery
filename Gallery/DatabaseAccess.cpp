#include "DatabaseAccess.h"

bool DatabaseAccess::open()
{
	std::string sqlStatedment;
	char* errMessage;
    int doesFileExist = _access(DBNAME.c_str(), 0);
	int res = sqlite3_open(DBNAME.c_str(), &this->db);

	if (res != SQLITE_OK)
	{
		this->db = nullptr;
		std::cerr << "Failed to open DB" << std::endl;
		return false;
	}

	if (doesFileExist == -1)
	{
		sqlStatedment = "CREATE TABLE Users(ID INTEGER PRIMARY KEY NOT NULL, Name TEXT NOT NULL);"; //users table
		sqlStatedment += "CREATE TABLE Albums(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Name TEXT NOT NULL, Creation_date TEXT NOT NULL, User_ID INTEGER NOT NULL, FOREIGN KEY (User_ID) REFERENCES Users(ID));"; //Albums Table
		sqlStatedment += "CREATE TABLE Pictures(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,Name TEXT NOT NULL, Location TEXT NOT NULL, Creation_date TEXT NOT NULL, Album_ID INTEGER NOT NULL, FOREIGN KEY (Album_ID) REFERENCES Albums(ID));"; //Pictures Table
		sqlStatedment += "CREATE TABLE Tags(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, User_ID INTEGER NOT NULL, Picture_ID INTEGER NOT NULL, FOREIGN KEY (User_ID) REFERENCES Users(ID), FOREIGN KEY (Picture_ID) REFERENCES Pictures(ID));"; //Tags Table
		errMessage = nullptr;
		res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
		{
			std::cerr << "CREATION OF TABLES FAILED!" << std::endl;
			return false;
		}
	}

	return true;
}

void DatabaseAccess::close()
{
	sqlite3_close(this->db);
	this->db = nullptr;
}

void DatabaseAccess::clear()
{
}

void DatabaseAccess::closeAlbum(Album& pAlbum)
{
}

void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	int res;
	std::string sqlStatedment;
	char* errMessage;
	sqlStatedment = "DELETE FROM Albums WHERE Name = " + albumName + ";";
	errMessage = nullptr;
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "DELETE ALBUM FAILED!" << std::endl;
}

void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{

}

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
}

void DatabaseAccess::createUser(User& user)
{
	int res;
	std::string sqlStatedment;
	char* errMessage;
	sqlStatedment = "INSERT INTO Users VALUES (" + std::to_string(user.getId()) + ", '" + user.getName() + "');";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "CREATE USER FAILED!" << std::endl;
}

void DatabaseAccess::deleteUser(const User& user)
{
	int res;
	std::string sqlStatedment;
	char* errMessage;
	sqlStatedment = "DELETE FROM Users WHERE ID = " + std::to_string(user.getId()) + "AND Name = '" + user.getName() + "';";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "CREATE USER FAILED!" << std::endl;
}
