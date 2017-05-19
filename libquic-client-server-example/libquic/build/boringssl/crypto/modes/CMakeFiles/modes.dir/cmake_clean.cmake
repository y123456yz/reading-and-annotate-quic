FILE(REMOVE_RECURSE
  "aesni-gcm-x86_64.S"
  "ghash-x86_64.S"
  "CMakeFiles/modes.dir/cbc.c.o"
  "CMakeFiles/modes.dir/ctr.c.o"
  "CMakeFiles/modes.dir/ofb.c.o"
  "CMakeFiles/modes.dir/cfb.c.o"
  "CMakeFiles/modes.dir/gcm.c.o"
  "CMakeFiles/modes.dir/aesni-gcm-x86_64.S.o"
  "CMakeFiles/modes.dir/ghash-x86_64.S.o"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang ASM C)
  INCLUDE(CMakeFiles/modes.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
