FILE(REMOVE_RECURSE
  "sha1-x86_64.S"
  "sha256-x86_64.S"
  "sha512-x86_64.S"
  "CMakeFiles/sha.dir/sha1.c.o"
  "CMakeFiles/sha.dir/sha256.c.o"
  "CMakeFiles/sha.dir/sha512.c.o"
  "CMakeFiles/sha.dir/sha1-x86_64.S.o"
  "CMakeFiles/sha.dir/sha256-x86_64.S.o"
  "CMakeFiles/sha.dir/sha512-x86_64.S.o"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang ASM C)
  INCLUDE(CMakeFiles/sha.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
