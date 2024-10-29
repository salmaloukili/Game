#include <vector>
#ifndef PERLINNOISE_H
#define PERLINNOISE_H
/**
 * Obtained from: https://github.com/DeiVadder/QNoise
 *
 * C++ based implementation of (orignal) Perlin Noise and OpenSimplex Noise, intended to be used with your Qt-Application
 *
 * The source implementation is in Java, made by the New York University Media Research Lab. See for reference here: https://mrl.nyu.edu/~perlin/noise/
 * And the translation to c++11 is made by sol-prog See for reference here: https://github.com/sol-prog/Perlin_Noise
 * QNoise is a cpp class implementing OpenSimplex Noise algorithem, related to Ken Perlin's updated algorithem (2001) but only related and open source
 * The source implementation was made in Java by Kurt Spencer and was translated to c++11 by me. See for reference here: https://gist.github.com/KdotJPG/b1270127455a94ac5d19
 * The example is a ready to compile Qt-Application (c++11 compiler and Qt5 needed) A noise map is drawn on top of the QWidget and shows the algorithem in action.
 **/

class PerlinNoise {
    /// The permutation vector
    std::vector<int> p;

public:
    /// Initialize with the reference values for the permutation vector
    PerlinNoise();
    /// Generate a new permutation vector based on the value of seed
    PerlinNoise(unsigned int seed);
    /// Get a noise value, for 2D images z can have any value
    double noise(double x, double y, double z);

private:
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
};

#endif
