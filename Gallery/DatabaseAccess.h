#pragma once

#include <io.h>
#include "IDataAccess.h"
#include "sqlite3.h"
#include <unordered_map>

const std::string DBNAME = "GalleryDB.sqlite";

class DatabaseAccess : public IDataAccess
{
public:
	// queries
	bool open() override;
	void close() override;
	void clear() override;

	User getTopTaggedUser() override;
	Picture getTopTaggedPicture() override;
	std::list<Picture> getTaggedPicturesOfUser(const User& user) override;

	// album related
	const std::list<Album> getAlbums() override;
	const std::list<Album> getAlbumsOfUser(const User& user) override;
	void closeAlbum(Album& pAlbum) override;
	void deleteAlbum(const std::string& albumName, int userId) override;
	void createAlbum(const Album& album) override;
	bool doesAlbumExists(const std::string& albumName, int userId) override;
	Album openAlbum(const std::string& albumName) override;
	void printAlbums() override;

	// pictures related
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void addPictureToAlbumByName(const std::string& albumName, const Picture& picture) override;
	void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) override;
	int lastPicId() override;

	// user related
	void createUser(User& user) override;
	void deleteUser(const User& user) override;
	bool doesUserExists(int userId) override;
	User getUser(int userId) override;
	void printUsers() override;

	// user statistics
	int countAlbumsOwnedOfUser(const User& user) override;
	int lastUserId() override;
	int countAlbumsTaggedOfUser(const User& user) override; //unfinished! //get all tags with the user and the check every pic if its from othe album
	int countTagsOfUser(const User& user) override;
	float averageTagsPerAlbumOfUser(const User& user) override; //unfinished!
private:
	sqlite3* db;
};

int callbackToAlbums(void* data, int argc, char** argv, char** azColName);
int callbackToAlbum(void* data, int argc, char** argv, char** azColName);
int callbackDoesExist(void* data, int argc, char** argv, char** azColName);
int callbackUser(void* data, int argc, char** argv, char** azColName);
int callbackPicture(void* data, int argc, char** argv, char** azColName);
int callbackPictures(void* data, int argc, char** argv, char** azColName);
int callbackPrintUsers(void* data, int argc, char** argv, char** azColName);
int callbackReturnArgc(void* data, int argc, char** argv, char** azColName);