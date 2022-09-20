#pragma once

#include <string>
#include <fstream>
#include <vector>

class Image {
public:
  Image(const std::string& path, const std::vector<std::string> tags, const std::string& name = "");
  int open();
  int read();

  const size_t getSize();
  const std::vector<std::string> getTags();
  const unsigned char *getData();
private:
  const std::string path;
  const std::string name;
  std::ifstream stream;

  size_t size;
  std::vector<std::string> tags;
  unsigned char *data;
};

class XDB {
public:
  XDB(const std::string& path);
  int open();
  int check();
  int write(const std::vector<std::string> imagePaths, const std::vector<std::string> tags = std::vector<std::string>(), const std::string& path = ".db");
private:
  const std::string path;
  std::ifstream istream;
  std::ofstream ostream;
  size_t size;

  std::vector<Image> images;
};
