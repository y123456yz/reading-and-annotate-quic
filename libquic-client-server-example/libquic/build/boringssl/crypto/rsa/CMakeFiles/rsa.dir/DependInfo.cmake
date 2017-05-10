# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  "C"
  )
# The set of files for implicit dependencies of each language:
SET(CMAKE_DEPENDS_CHECK_C
  "/root/yangyazhou/quic/libquic/boringssl/crypto/rsa/blinding.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/rsa/CMakeFiles/rsa.dir/blinding.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/rsa/padding.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/rsa/CMakeFiles/rsa.dir/padding.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/rsa/rsa.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/rsa/CMakeFiles/rsa.dir/rsa.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/rsa/rsa_asn1.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/rsa/CMakeFiles/rsa.dir/rsa_asn1.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/rsa/rsa_impl.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/rsa/CMakeFiles/rsa.dir/rsa_impl.c.o"
  )
SET(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
SET(CMAKE_TARGET_DEFINITIONS
  "USE_OPENSSL"
  "BORINGSSL_IMPLEMENTATION"
  "USE_OPENSSL"
  "BORINGSSL_IMPLEMENTATION"
  )

# Targets to which this target links.
SET(CMAKE_TARGET_LINKED_INFO_FILES
  )

# The include file search paths:
SET(CMAKE_C_TARGET_INCLUDE_PATH
  "../src"
  "../src/third_party/modp_b64"
  "../boringssl/include"
  "../src/third_party/protobuf/src"
  "../boringssl/crypto/."
  "../boringssl/crypto/../include"
  "../boringssl/crypto/rsa/."
  "../boringssl/crypto/rsa/.."
  "../boringssl/crypto/rsa/../../include"
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
