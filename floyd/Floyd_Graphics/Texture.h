#ifndef FLOYD_TEXTURE_H
#define FLOYD_TEXTURE_H


#include <vector>
#include <string>


class Texture
{
private:
	std::vector<std::string> data;

public:
	Texture();

public:
	///
	/// @brief Loads the texture's data from a file.
	///		   Every line of the file represents a line in the texture.
	///		   The file must be square.
	///
	void LoadData(const std::string &fileName);
	///
	/// @brief Loads the texture from a raw string of characters.
	///		   Every line must be terminated by a '\n'. Even if we have only one character
	///		   like "|".
	///		   Lines also shoud be of the same length.
	///
	void LoadRawData(const std::string &rawData);
	///
	/// @brief Gets the texture as raw data. 
	///		   Every entry in the returned vector is a separate line.
	///
	const std::vector<std::string>* GetData() const;
};


#endif