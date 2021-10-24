#include <cstdint>
#include <cstdio>

#include <iostream>
#include <iterator>
#include <vector>

#include <pl/algo/erase.hpp>
#include <pl/algo/ranged_algorithms.hpp>
#include <pl/as_bytes.hpp>
#include <pl/assert.hpp>
#include <pl/bit.hpp>
#include <pl/bitmask.hpp>
#include <pl/hexify.hpp>
#include <pl/iterate_reversed.hpp>
#include <pl/noncopyable.hpp>
#include <pl/print_bytes_as_hex.hpp>
#include <pl/timer.hpp>
#include <pl/unhexify.hpp>

namespace user {
enum class scoped_enum : std::uint8_t {
  Read      = 0B01U, // 1
  Write     = 0B10U, // 2
  ReadWrite = 0B11U  // 3
};
PL_ENABLE_BITMASK_OPERATORS(scoped_enum)
}

static void fun(int* p)
{
  PL_CHECK_PRE(p != nullptr);
  const int prev{*p};
  *p *= *p;
  PL_CHECK_POST((prev * prev) == *p);
}

class MyClass {
public:
  PL_NONCOPYABLE(MyClass);
};

int main()
{
  pl::timer timer{};

  std::vector<int>                       v{1, 2, 3, 4, 5, 6};
  const std::vector<int>::const_iterator it{pl::algo::find(v, 5)};

  if (it != v.end()) {
    std::cout << "Found it!\n";
    const auto dist{std::distance(v.cbegin(), it)};
    std::cout << "index: " << dist << '\n';
  }

  /*
  v.erase(pl::algo::remove_if(v, [](int i) { return i < 3; }), v.end());
  */
  pl::algo::erase_if(v, [](int i) { return i < 3; });

  for (int i : v) {
    std::cout << i << '\n';
  }

  std::uint32_t         value{0xDEADC0DE};
  pl::byte*             ptr{pl::as_bytes(value)};
  const pl::byte* const end{ptr + sizeof(value)};
  // ptr[1] = 0x7E;

  for (; ptr != end; ++ptr) {
    std::printf("%02X ", *ptr);
  }
  std::printf("\n");

  std::cout << "bytes: " << pl::print_bytes_as_hex{&value, sizeof(value)}
            << '\n';

  const std::string hexString{pl::hexify(&value, sizeof(value), ":")};
  std::cout << "hexString: " << hexString << '\n';

  const std::vector<pl::byte> bytes{pl::unhexify(hexString, 1)};

  std::printf("Bytes:\n");
  for (pl::byte byte : bytes) {
    std::printf("%02X ", byte);
  }
  std::printf("\n");

  static_assert(
    sizeof(float) == sizeof(int),
    "Int and float are not of the same byte width");
  const float f{0.4541512F};
  const int   i{pl::bit_cast<int>(f)};
  std::cout << i << '\n';

  std::cout << std::boolalpha;

  // enm is A and B toggled on
  auto enm = user::scoped_enum::Read | user::scoped_enum::Write;
  std::cout << (enm == user::scoped_enum::ReadWrite) << std::endl;

  /*
  int val{5};
  fun(&val);
  */
  const std::vector<int> vector{1, 2, 3, 4};
  for (int i : pl::iterate_reversed(vector)) {
    std::cout << i << '\n';
  }

  const auto timeTaken{timer.elapsed_time()};
  std::cout
    << std::chrono::duration_cast<std::chrono::nanoseconds>(timeTaken).count()
    << " nanoseconds taken\n";

  timer.reset();

  for (int i{5}; i < 10000; ++i) {
    std::cout << i << '\n';
  }

  std::cout << std::chrono::duration_cast<std::chrono::seconds>(
                 timer.elapsed_time())
                 .count()
            << " seconds\n";
}
