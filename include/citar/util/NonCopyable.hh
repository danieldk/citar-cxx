#ifndef NON_COPYABLE_HH
#define NON_COPYABLE_HH

namespace citar
{
namespace util
{

class NonCopyable
{
protected:
	NonCopyable() {}
	virtual ~NonCopyable() {}
private:
	NonCopyable(NonCopyable const &other);
	NonCopyable &operator=(NonCopyable const &other);
};

}
}

#endif //NON_COPYABLE_HH
