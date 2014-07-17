#ifndef FLOYD_TEXTURE_H
#define FLOYD_TEXTURE_H


#include <sstream>


class Texture
{
private:
	char *data;

public:
	void LoadData(std::stringstream &dataStream);
	char* GetData() const;

public:
	Texture();
	~Texture();

private:
	Texture(const Texture &other);
	Texture& operator=(const Texture &other);
};


#endif