FILE(REMOVE_RECURSE
  "aes-x86_64.S"
  "aesni-x86_64.S"
  "bsaes-x86_64.S"
  "vpaes-x86_64.S"
  "CMakeFiles/aes.dir/aes.c.o"
  "CMakeFiles/aes.dir/mode_wrappers.c.o"
  "CMakeFiles/aes.dir/aes-x86_64.S.o"
  "CMakeFiles/aes.dir/aesni-x86_64.S.o"
  "CMakeFiles/aes.dir/bsaes-x86_64.S.o"
  "CMakeFiles/aes.dir/vpaes-x86_64.S.o"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang ASM C)
  INCLUDE(CMakeFiles/aes.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
