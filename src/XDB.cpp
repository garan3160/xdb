#include <iostream>
#include <cassert>
#include <cstring>

#include "XDB.h"

Image::Image(const std::string& path, const std::vector<std::string> tags, const std::string& name):
path(path),
tags(tags) {
  // assert(open());
  // assert(read());
  
  open();
  read();
}

int Image::open() {
  stream.open(path, std::ios_base::binary | std::ios_base::ate);
  if(!stream.is_open()) {
    std::cerr << "int Image::open(): bad file" << std::endl;
    return 1;
  }

  size = stream.tellg();

  stream.seekg(0);

  return 0;
}

int Image::read() {
  if(!stream.is_open()) {
    std::cerr << "int Image::read(): bad stream" << std::endl;
    return 1;
  }

  data = new unsigned char[size];
  stream.read(reinterpret_cast<char*>(data), size);

  return 0;
}

const size_t Image::getSize() {
  return size;
}

const std::vector<std::string> Image::getTags() {
  return tags;
}

const unsigned char *Image::getData() {
  return data;
}

XDB::XDB(const std::string& path):
path(path) {

}

int XDB::open() {
  istream = std::ifstream(path, std::ios_base::binary | std::ios_base::ate);
  if(!istream.is_open()) {
    std::cerr << "int XDB::open(): bad file" << std::endl;
    return 1;
  }

  size = istream.tellg();
  if(size < 2) {
    std::cerr << "int XDB::check(): bad stream size" << std::endl;
    return 1;
  }

  istream.seekg(0);

  return 0;
}

int XDB::check() {
  if(!istream.is_open()) {
    std::cerr << "int XDB::check(): bad stream" << std::endl;
    return 1;
  }

  char magic[2];
  istream.read(magic, 2);
  if(memcmp(magic, "xd", 2)) {
    std::cerr << "int XDB::check(): bad magic bytes" << std::endl;
    return 1;
  }

  return 0;
}

int XDB::write(const std::vector<std::string> imagePaths, const std::vector<std::string> tags, const std::string& path) {
  ostream.open(path, std::ios_base::binary);
  if(!ostream.is_open()) {
    std::cerr << "int XDB::write(const std::vector<std::string>& imagePaths, const std::string& path): bad stream" << std::endl;
    return 1;
  }

  ostream.write("xd", 2);

  auto imagePathsSize = imagePaths.size();

  ostream.write(reinterpret_cast<char*>(&imagePathsSize), 4);

  size_t currentImageSize;
  size_t currentImageTagsSize;

  for(int i = 0; i < imagePaths.size(); i++) {
    images.push_back(Image(imagePaths[i], std::vector<std::string>()));

    currentImageTagsSize = images[i].getTags().size();
    ostream.write(reinterpret_cast<char*>(&currentImageTagsSize), 2);

    for(int j = 0; j < currentImageTagsSize; j++) {
      ostream.write(images[i].getTags()[j].c_str(), strlen(images[i].getTags()[j].c_str()));
    }

    currentImageSize = images[i].getSize();
    ostream.write(reinterpret_cast<char*>(&currentImageSize), 4);
    
    ostream.write(reinterpret_cast<const char*>(images[i].getData()), currentImageSize);
  }

  return 0;
}
