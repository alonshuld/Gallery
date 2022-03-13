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
private:
	sqlite3* db;
};