# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  "C"
  )
# The set of files for implicit dependencies of each language:
SET(CMAKE_DEPENDS_CHECK_ASM
  "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cpu-x86_64-asm.S" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/cpu-x86_64-asm.S.o"
  )
SET(CMAKE_ASM_COMPILER_ID "GNU")
SET(CMAKE_DEPENDS_CHECK_C
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cpu-intel.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/cpu-intel.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/crypto.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/crypto.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/directory_posix.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/directory_posix.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/directory_win.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/directory_win.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/ex_data.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/ex_data.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/ex_data_impl.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/ex_data_impl.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/mem.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/mem.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/thread.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/thread.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/time_support.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/CMakeFiles/crypto.dir/time_support.c.o"
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
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
