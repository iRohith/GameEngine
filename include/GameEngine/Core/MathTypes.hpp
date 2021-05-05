#pragma once

#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Util/Util.hpp"

namespace GameEngine
{
    namespace MathTypes {

        enum Precision { PrecisionHigh = glm::qualifier::packed_highp, PrecisionMedium = glm::qualifier::packed_mediump, PrecisionLow = glm::qualifier::packed_lowp };

        // Vector types
        template<uint8_t N = 3, typename T = glm::f32, Precision P = PrecisionHigh>
        using Vec = glm::vec<N, T, (glm::qualifier)P>;

        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecF = Vec<N, glm::f32, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecD = Vec<N, glm::f64, P>;
        
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecI8 = Vec<N, glm::i8, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecI16 = Vec<N, glm::i16, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecI32 = Vec<N, glm::i32, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecI64 = Vec<N, glm::i64, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecI = VecI32<N, P>;
        
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecU8 = Vec<N, glm::u8, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecU16 = Vec<N, glm::u16, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecU32 = Vec<N, glm::u32, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecU64 = Vec<N, glm::u64, P>;
        template<uint8_t N = 3, Precision P = PrecisionHigh> using VecU = VecU32<N, P>;

        using Vec2 = VecF<2>;
        using Vec3 = VecF<3>;
        using Vec4 = VecF<4>;

        // Vector types

        template<typename T = glm::f32, Precision P = PrecisionHigh> using Quat = glm::qua<T, (glm::qualifier)P>;
        using QuatF = Quat<>;
        
        // Matrix types

        template<uint8_t M = 4, uint8_t N = 4, typename T = glm::f32, Precision P = PrecisionHigh>
        using Matrix = glm::mat<M, N, T, (glm::qualifier)P>;

        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixF = Matrix<M, N, glm::f32, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixD = Matrix<M, N, glm::f64, P>;
        
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixI8 = Matrix<M, N, glm::i8, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixI16 = Matrix<M, N, glm::i16, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixI32 = Matrix<M, N, glm::i32, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixI64 = Matrix<M, N, glm::i64, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixI = MatrixI32<M, N, P>;
        
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixU8 = Matrix<M, N, glm::u8, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixU16 = Matrix<M, N, glm::u16, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixU32 = Matrix<M, N, glm::u32, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixU64 = Matrix<M, N, glm::u64, P>;
        template<uint8_t M = 4, uint8_t N = 4, Precision P = PrecisionHigh> using MatrixU = MatrixU32<M, N, P>;

        using Mat3 = MatrixF<3, 3>;
        using Mat4 = MatrixF<4, 4>;
        
        // Matrix types

    } 

    namespace M = MathTypes;

}

template <typename T> struct fmt::formatter<GameEngine::M::Vec<3, T>>: formatter<std::string> {
  template <typename FormatContext>
  auto format(const GameEngine::M::Vec<3, T>& c, FormatContext& ctx) {
    return formatter<std::string>::format(fmt::format("[{}, {}, {}]", c[0], c[1], c[2]), ctx);
  }
};

template <typename T> struct fmt::formatter<GameEngine::M::Vec<2, T>>: formatter<std::string> {
  template <typename FormatContext>
  auto format(const GameEngine::M::Vec<2, T>& c, FormatContext& ctx) {
    return formatter<std::string>::format(fmt::format("[{}, {}]", c[0], c[1]), ctx);
  }
};


template <typename T> struct fmt::formatter<GameEngine::M::Matrix<4, 4, T>>: formatter<std::string> {
  template <typename FormatContext>
  auto format(const GameEngine::M::Matrix<4, 4, T>& c, FormatContext& ctx) {
    return formatter<std::string>::format(fmt::format(
      "\n"
      "\t[{}, {}, {}, {}]\n"
      "\t[{}, {}, {}, {}]\n"
      "\t[{}, {}, {}, {}]\n"
      "\t[{}, {}, {}, {}]\n",
      c[0][0], c[0][1], c[0][2], c[0][3],
      c[1][0], c[1][1], c[1][2], c[1][3],
      c[2][0], c[2][1], c[2][2], c[2][3],
      c[3][0], c[3][1], c[3][2], c[3][3]
    ), ctx);
  }
};