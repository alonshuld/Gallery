#pragma once

#include <io.h>
#include "IDataAccess.h"
#include "sqlite3.h"

const std::string DBNAME = "GalleryDB.sqlite";

class DatabaseAccess : public IDataAccess
{
public:
	// queries
	bool open() override;
	void close() override;
	void clear() override;

	// album related
	void closeAlbum(Album& pAlbum) override;
	void deleteAlbum(const std::string& albumName, int userId) override;

	// pictures related
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;

	// user related
	void createUser(User& user) override;
	void deleteUser(const User& user) override;

private:
	sqlite3* db;
};