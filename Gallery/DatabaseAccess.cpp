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
		sqlStatedment += "CREATE TABLE Pictures(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Name TEXT NOT NULL, Location TEXT NOT NULL, Creation_date TEXT NOT NULL, Album_ID INTEGER NOT NULL, FOREIGN KEY (Album_ID) REFERENCES Albums(ID));"; //Pictures Table
		sqlStatedment += "CREATE TABLE Tags(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, User_ID INTEGER NOT NULL, Picture_Name TEXT NOT NULL, FOREIGN KEY (User_ID) REFERENCES Users(ID), FOREIGN KEY (Picture_Name) REFERENCES Pictures(Name));"; //Tags Table
		errMessage = nullptr;
		res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
		{
			std::cerr << "---The creation of the table failed!---" << std::endl << errMessage << std::endl;
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

User DatabaseAccess::getTopTaggedUser()
{
	User user(1, "temp");
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Users WHERE ID = (SELECT User_ID FROM Tags GROUP BY User_ID LIMIT 1);";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackUser, &user, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Get top tagged user failed!---" << std::endl << errMessage << std::endl;
	return user;
}

Picture DatabaseAccess::getTopTaggedPicture()
{
	Picture pic(1, "temp");
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Pictures WHERE Name = (SELECT Picture_Name FROM Tags GROUP BY Picture_Name LIMIT 1);";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackPicture, &pic, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Get top tagged picture failed!---" << std::endl << errMessage << std::endl;
	return pic;
}

std::list<Picture> DatabaseAccess::getTaggedPicturesOfUser(const User& user)
{
	std::list<Picture> picList;
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Tags WHERE User_ID = " + std::to_string(user.getId()) + ";";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackPictures, &picList, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Get tagged pictures of user failed!---" << std::endl << errMessage << std::endl;
	return picList;
}

const std::list<Album> DatabaseAccess::getAlbums()
{
	std::list<Album> allAlbums;
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Albums;";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackToAlbums, &allAlbums, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Get all albums failed!---" << std::endl << errMessage << std::endl;
	return allAlbums;
}

const std::list<Album> DatabaseAccess::getAlbumsOfUser(const User& user)
{
	std::list<Album> album;
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Albums WHERE User_ID = " + std::to_string(user.getId()) + ";";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackToAlbums, &album, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Get all albums of a user failed!---" << std::endl << errMessage << std::endl;
	return album;
}

int callbackToAlbums(void* data, int argc, char** argv, char** azColName)
{
	Album album;
	std::list<Album>* tempList = (std::list<Album>*)data;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "Name")
			album.setName(argv[i]);
		else if (std::string(azColName[i]) == "Creation_date")
			album.setCreationDate(argv[i]);
		else if (std::string(azColName[i]) == "User_ID")
			album.setOwner(atoi(argv[i]));
	}
	tempList->push_back(album);
	return 0;
}

int callbackToAlbum(void* data, int argc, char** argv, char** azColName)
{
	Album* album = (Album*)data;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "Name")
			album->setName(argv[i]);
		else if (std::string(azColName[i]) == "Creation_date")
			album->setCreationDate(argv[i]);
		else if (std::string(azColName[i]) == "User_ID")
			album->setOwner(atoi(argv[i]));
	}
	return 0;
}

int callbackDoesExist(void* data, int argc, char** argv, char** azColName)
{
	*(bool*)data = true;
	return 0;
}

int callbackUser(void* data, int argc, char** argv, char** azColName)
{
	User* user = (User*)data;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "Name")
			user->setName(argv[i]);
		else if (std::string(azColName[i]) == "ID")
			user->setId(atoi(argv[i]));
	}
	return 0;
}

int callbackPicture(void* data, int argc, char** argv, char** azColName)
{
	Picture* pic = (Picture*)data;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "Name")
			pic->setName(argv[i]);
		else if (std::string(azColName[i]) == "ID")
			pic->setId(atoi(argv[i]));
		else if (std::string(azColName[i]) == "Location")
			pic->setPath(argv[i]);
		else if (std::string(azColName[i]) == "Creation_date")
			pic->setCreationDate(argv[i]);
	}
	return 0;
}

int callbackPictures(void* data, int argc, char** argv, char** azColName)
{
	Picture pic(1, "temp");
	std::list<Picture>* tempList = (std::list<Picture>*)data;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "Name")
			pic.setName(argv[i]);
		else if (std::string(azColName[i]) == "ID")
			pic.setId(atoi(argv[i]));
		else if (std::string(azColName[i]) == "Location")
			pic.setPath(argv[i]);
		else if (std::string(azColName[i]) == "Creation_date")
			pic.setCreationDate(argv[i]);
	}
	tempList->push_back(pic);
	return 0;
}

int callbackPrintUsers(void* data, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "ID")
			std::cout << argv[i] << "@";
		else if (std::string(azColName[i]) == "Name")
			std::cout << argv[i] << std::endl;
	}
	std::cout << std::endl;
	return 0;
}

int callbackReturnArgc(void* data, int argc, char** argv, char** azColName)
{
	int* num = (int*)data;
	*num = argc;
	return 0;
}

void DatabaseAccess::closeAlbum(Album& pAlbum)
{
}

void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "DELETE FROM Albums WHERE Name = " + albumName + ";";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Delete album failed!---" << std::endl << errMessage << std::endl;
}

void DatabaseAccess::createAlbum(const Album& album)
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "INSERT INTO Albums (Name, Creation_date, User_ID) VALUES ('" + album.getName() + "', '" + album.getCreationDate() + "', " + std::to_string(album.getOwnerId()) + ");";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Create an album failed!---" << std::endl << errMessage << std::endl;
}

bool DatabaseAccess::doesAlbumExists(const std::string& albumName, int userId)
{
	bool flag = false;
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Albums WHERE User_ID = " + std::to_string(userId) + " AND Name = '" + albumName + "';";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackDoesExist, &flag, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Does album exist failed!---" << std::endl << errMessage << std::endl;
	return flag;
}

Album DatabaseAccess::openAlbum(const std::string& albumName)
{
	Album album;
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Albums WHERE Name = '" + albumName + "';";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackToAlbum, &album, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Open album failed!---" << std::endl << errMessage << std::endl;
	return album;
}

void DatabaseAccess::printAlbums()
{
	std::list<Album> allAlbums = getAlbums();
	std::cout << "Album list:" << std::endl;
	std::cout << "-----------" << std::endl;
	for (auto it = allAlbums.begin(); it != allAlbums.end(); it++)
	{
		std::cout << "Name: " + it->getName() << std::endl << "Creation date: " + it->getCreationDate() << std::endl << "Owner ID: " << it->getOwnerId() << std::endl << std::endl;
	}
}

void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "INSERT INTO Tags (User_ID, Picture_Name) VALUES (" + std::to_string(userId) + ", '" + pictureName + "');";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Tag user in picture failed!---" << std::endl << errMessage << std::endl;
}

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "DELETE FROM Tags WHERE User_ID = " + std::to_string(userId) + " AND Picture_Name = '" + pictureName + "');";
	errMessage = nullptr;
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Untag user in picture failed!---" << std::endl << errMessage << std::endl;
}

void DatabaseAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture)
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "INSERT INTO Pictures (Name, Location, Creation_date, Album_ID) VALUES ('" + picture.getName() + "', '" + picture.getPath() + "', '" + picture.getCreationDate() + "', (SELECT ID FROM Albums WHERE Name = '" + albumName + "'));";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Add picture to an album by name failed!---" << errMessage << std::endl << errMessage << std::endl;
}

void DatabaseAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName)
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "DELETE FROM Pictures WHERE Album_ID = (SELECT ID FROM Albums WHERE Name = '" + albumName + "') AND Name = '" + pictureName + "';";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Remove picture from an album by name failed!---" << std::endl << errMessage << std::endl;
}

void DatabaseAccess::createUser(User& user)
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "INSERT INTO Users VALUES (" + std::to_string(user.getId()) + ", '" + user.getName() + "');";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Creating a user failed!---" << std::endl << errMessage << std::endl;
}

void DatabaseAccess::deleteUser(const User& user)
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "DELETE FROM Users WHERE ID = " + std::to_string(user.getId()) + "AND Name = '" + user.getName() + "';";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Delete a user failed!---" << std::endl << errMessage << std::endl;
}

bool DatabaseAccess::doesUserExists(int userId)
{
	bool flag = false;
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Users WHERE ID = " + std::to_string(userId) + ";";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackDoesExist, &flag, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Does user exist failed!---" << std::endl << errMessage << std::endl;
	return flag;
}

User DatabaseAccess::getUser(int userId)
{
	User user(userId, "temp");
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Users WHERE ID = " + std::to_string(userId) + ";";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackToAlbum, &user, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Get user failed!---" << std::endl << errMessage << std::endl;
	return user;
}

void DatabaseAccess::printUsers()
{
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Users;";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackPrintUsers, nullptr, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Print all users failed!---" << std::endl << errMessage << std::endl;
}

int DatabaseAccess::getAmountOfUsers()
{
	int ans;
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Users;";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackReturnArgc, &ans, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Get amount of users failed!---" << std::endl << errMessage << std::endl;
	if (ans < 0)
		ans = 0;
	return ans;
}

int DatabaseAccess::countAlbumsOwnedOfUser(const User& user)
{
	int ans;
	int res;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Albums WHERE User_ID = " + std::to_string(user.getId()) + ";";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackReturnArgc, &ans, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Count albums owned by a user failed!---" << std::endl << errMessage << std::endl;
	return ans;
}

int DatabaseAccess::countAlbumsTaggedOfUser(const User& user)
{
	int ans;
	int tempAns;
	int res;
	std::string sqlStatedment;
	std::list<Album> allAlbums = getAlbums();
	char* errMessage;
	for (auto it = allAlbums.begin(); it != allAlbums.end(); it++)
	{
		errMessage = nullptr;
		sqlStatedment = "SELECT * FROM Pictures WHERE Album_ID = " + std::to_string(it->getOwnerId()) + ";";
		res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackReturnArgc, &ans, &errMessage);
		if (res != SQLITE_OK)
			std::cerr << "---Count albums tagged of user failed!---" << std::endl << errMessage << std::endl;
	}
	return ans;
}

int DatabaseAccess::countTagsOfUser(const User& user)
{
	int res;
	int ans;
	std::string sqlStatedment;
	char* errMessage = nullptr;
	sqlStatedment = "SELECT * FROM Tags WHERE User_ID = " + std::to_string(user.getId()) + ";";
	res = sqlite3_exec(this->db, sqlStatedment.c_str(), callbackReturnArgc, &ans, &errMessage);
	if (res != SQLITE_OK)
		std::cerr << "---Count tags of user failed!---" << std::endl << errMessage << std::endl;
	return ans;
}

float DatabaseAccess::averageTagsPerAlbumOfUser(const User& user)
{
	return 0.0f;
}
