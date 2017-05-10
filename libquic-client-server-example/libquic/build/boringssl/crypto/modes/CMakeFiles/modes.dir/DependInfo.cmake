# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  "C"
  )
# The set of files for implicit dependencies of each language:
SET(CMAKE_DEPENDS_CHECK_ASM
  "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/aesni-gcm-x86_64.S" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/CMakeFiles/modes.dir/aesni-gcm-x86_64.S.o"
  "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/ghash-x86_64.S" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/CMakeFiles/modes.dir/ghash-x86_64.S.o"
  )
SET(CMAKE_ASM_COMPILER_ID "GNU")
SET(CMAKE_DEPENDS_CHECK_C
  "/root/yangyazhou/quic/libquic/boringssl/crypto/modes/cbc.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/CMakeFiles/modes.dir/cbc.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/modes/cfb.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/CMakeFiles/modes.dir/cfb.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/modes/ctr.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/CMakeFiles/modes.dir/ctr.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/modes/gcm.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/CMakeFiles/modes.dir/gcm.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/modes/ofb.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/modes/CMakeFiles/modes.dir/ofb.c.o"
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
  "../boringssl/crypto/modes/."
  "../boringssl/crypto/modes/.."
  "../boringssl/crypto/modes/../../include"
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
