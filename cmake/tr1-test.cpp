#include <string>

#include <tr1/memory>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

using namespace std;
using namespace std::tr1;

int main(int argc, char *argv[])
{
  std::tr1::shared_ptr<int> intPtr(new int);
  unordered_map<string, int> stringIntMap;
  unordered_set<string> stringSet;

  return 0;
}
