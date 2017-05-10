# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  "C"
  )
# The set of files for implicit dependencies of each language:
SET(CMAKE_DEPENDS_CHECK_C
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/aead.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/aead.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/cipher.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/cipher.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/derive_key.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/derive_key.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/e_aes.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/e_aes.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/e_chacha20poly1305.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/e_chacha20poly1305.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/e_des.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/e_des.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/e_null.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/e_null.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/e_rc2.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/e_rc2.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/e_rc4.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/e_rc4.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/e_ssl3.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/e_ssl3.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/e_tls.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/e_tls.c.o"
  "/root/yangyazhou/quic/libquic/boringssl/crypto/cipher/tls_cbc.c" "/root/yangyazhou/quic/libquic/build/boringssl/crypto/cipher/CMakeFiles/cipher.dir/tls_cbc.c.o"
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
  "../boringssl/crypto/cipher/."
  "../boringssl/crypto/cipher/.."
  "../boringssl/crypto/cipher/../../include"
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
