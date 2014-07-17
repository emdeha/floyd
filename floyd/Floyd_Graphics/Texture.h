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
	const std::vector<std::string>* GetData() const;

public:
	Texture();
};


#endif