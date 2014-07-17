#ifndef FLOYD_TEXTURE_H
#define FLOYD_TEXTURE_H


#include <vector>
#include <string>


class Texture
{
private:
	std::vector<std::string> data;

public:
	void LoadData(const std::string &fileName);
	void LoadRawData(const std::string &rawData);
	const std::vector<std::string>* GetData() const;

public:
	Texture();
};


#endif