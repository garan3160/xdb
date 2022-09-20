#include "XDB.h"

int main() {
  XDB db("test.xdb");
  std::vector<std::string> imagePaths;
  imagePaths.push_back("build.sh");
  imagePaths.push_back("57ec29fa077dccf2018b8c62.webp");
  db.write(imagePaths, );

  return 0;
}
