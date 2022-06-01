// xrand1.h - An implementation of xorshift random number generator.
// See the description of class XRand for its usage.
// Version 1.1.1

// Copyright (c) 2013 - 2018  Yui Hosaka (hos@hos.ac)
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//    1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
//    2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
//
//    3. This notice may not be removed or altered from any source
//    distribution.

#ifndef XRAND1_H_
#define XRAND1_H_

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>

#define rand _DO_NOT_USE_RAND_
#define srand _DO_NOT_USE_SRAND_
#define random_shuffle _DO_NOT_USE_RANDOM_SHUFFLE_

class XRand {
 public:
  // Initializes with the given seed.
  explicit XRand(uint64_t seed = 0);
  void Reset(uint64_t seed);

  // Generates the next random number, changing the state of the generator.
  // NextULong() calls NextUInt() twice.
  uint32_t NextUInt();
  uint64_t NextULong();

  // Returns an integer, uniformly distributed in [0, m).
  // It must hold that m != 0.
  // The expected number of calls to NextUInt() is at most two for UInt and four
  // for ULong.
  uint32_t NextUInt(uint32_t m);
  uint64_t NextULong(uint64_t m);

  // Returns an integer, uniformly distributed in [a, b].
  // It must hold that a <= b.
  // The expected number of calls to NextUInt() is at most two for Int and four
  // for Long.
  int32_t NextInt(int32_t a, int32_t b);
  int64_t NextLong(int64_t a, int64_t b);

  // Returns a double, uniformly distributed in [0.0, 1.0).
  // Calls NextUInt() twice.
  double NextDouble();

  // Returns a double, normally distributed with expectation 0 and variance 1.
  // Calls NextUInt() four times.
  double NextGaussian();

  // Shuffles the interval [first, last).
  // Calls NextUInt() (last - first) times.
  template <class RandomAccessIterator>
  void Shuffle(RandomAccessIterator first, RandomAccessIterator last);

 private:
  uint32_t x_, y_, z_, w_;
};

XRand::XRand(uint64_t seed) {
  Reset(seed);
}

void XRand::Reset(uint64_t seed) {
  // Uses MINSTD (Park, Miller and Stockmeyer, 1993) for initial values.
  static constexpr uint64_t kMinstdM = 2147483647;
  static constexpr uint64_t kMinstdA = 48271;
  uint64_t s = 1 + seed % (kMinstdM - 1);
  auto minstd = [&s]() -> uint64_t { return s = s * kMinstdA % kMinstdM; };
  x_ = minstd();
  y_ = minstd();
  z_ = minstd() ^ static_cast<uint32_t>(seed >> 32);
  w_ = minstd() ^ static_cast<uint32_t>(seed);
}

uint32_t XRand::NextUInt() {
  const uint32_t t = x_ ^ x_ << 11;
  x_ = y_;
  y_ = z_;
  z_ = w_;
  return w_ = w_ ^ w_ >> 19 ^ t ^ t >> 8;
}

uint64_t XRand::NextULong() {
  const uint64_t high = NextUInt();
  const uint64_t low = NextUInt();
  return high << 32 | low;
}

uint32_t XRand::NextUInt(uint32_t m) {
  assert(m != 0);
  if (m & (m - 1)) {
    const uint32_t limit = UINT32_MAX / m * m;
    for (; ; ) {
      const uint32_t value = NextUInt();
      if (value < limit) {
        return value % m;
      }
    }
  } else {
    return NextUInt() & (m - 1);
  }
}

uint64_t XRand::NextULong(uint64_t m) {
  assert(m != 0);
  if (m & (m - 1)) {
    const uint64_t limit = UINT64_MAX / m * m;
    for (; ; ) {
      const uint64_t value = NextULong();
      if (value < limit) {
        return value % m;
      }
    }
  } else {
    return NextULong() & (m - 1);
  }
}

int32_t XRand::NextInt(int32_t a, int32_t b) {
  assert(a <= b);
  const uint32_t m = static_cast<uint32_t>(b) - static_cast<uint32_t>(a) + 1;
  return a + (m ? NextUInt(m) : NextUInt());
}

int64_t XRand::NextLong(int64_t a, int64_t b) {
  assert(a <= b);
  const uint64_t m = static_cast<uint64_t>(b) - static_cast<uint64_t>(a) + 1;
  return a + (m ? NextULong(m) : NextULong());
}

double XRand::NextDouble() {
  static constexpr uint64_t kNumValues = 1ULL << 53;
  return static_cast<double>(NextULong() & (kNumValues - 1)) / kNumValues;
}

double XRand::NextGaussian() {
  // Uses Box-Muller transform.
  static const double kPi = acos(-1.0);
  const double value1 = NextDouble();
  const double value2 = NextDouble();
  return sqrt(-2.0 * log1p(-value1)) * cos(2.0 * kPi * value2);
}

template <class RandomAccessIterator>
void XRand::Shuffle(RandomAccessIterator first, RandomAccessIterator last) {
  for (RandomAccessIterator iter = first; iter != last; ++iter) {
    std::iter_swap(first + NextUInt(iter - first + 1), iter);
  }
}

#endif  // XRAND1_H_
