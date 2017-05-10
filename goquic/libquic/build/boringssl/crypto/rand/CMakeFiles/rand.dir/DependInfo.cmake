# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  "C"
  )
# The set of files for implicit dependencies of each language:
SET(CMAKE_DEPENDS_CHECK_ASM
  "/root/redisbackup/src/github.com/devsisters/goquic/libquic/build/boringssl/crypto/rand/rdrand-x86_64.S" "/root/redisbackup/src/github.com/devsisters/goquic/libquic/build/boringssl/crypto/rand/CMakeFiles/rand.dir/rdrand-x86_64.S.o"
  )
SET(CMAKE_ASM_COMPILER_ID "GNU")
SET(CMAKE_DEPENDS_CHECK_C
  "/root/redisbackup/src/github.com/devsisters/goquic/libquic/boringssl/crypto/rand/deterministic.c" "/root/redisbackup/src/github.com/devsisters/goquic/libquic/build/boringssl/crypto/rand/CMakeFiles/rand.dir/deterministic.c.o"
  "/root/redisbackup/src/github.com/devsisters/goquic/libquic/boringssl/crypto/rand/rand.c" "/root/redisbackup/src/github.com/devsisters/goquic/libquic/build/boringssl/crypto/rand/CMakeFiles/rand.dir/rand.c.o"
  "/root/redisbackup/src/github.com/devsisters/goquic/libquic/boringssl/crypto/rand/urandom.c" "/root/redisbackup/src/github.com/devsisters/goquic/libquic/build/boringssl/crypto/rand/CMakeFiles/rand.dir/urandom.c.o"
  "/root/redisbackup/src/github.com/devsisters/goquic/libquic/boringssl/crypto/rand/windows.c" "/root/redisbackup/src/github.com/devsisters/goquic/libquic/build/boringssl/crypto/rand/CMakeFiles/rand.dir/windows.c.o"
  )
SET(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
SET(CMAKE_TARGET_DEFINITIONS
  "USE_OPENSSL"
  "__STDC_CONSTANT_MACROS"
  "__STDC_LIMIT_MACROS"
  "BORINGSSL_IMPLEMENTATION"
  "USE_OPENSSL"
  "__STDC_CONSTANT_MACROS"
  "__STDC_LIMIT_MACROS"
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
  "/src"
  "../boringssl/crypto/../include"
  "../boringssl/crypto/rand/../../include"
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
