FILE(REMOVE_RECURSE
  "rdrand-x86_64.S"
  "CMakeFiles/rand.dir/deterministic.c.o"
  "CMakeFiles/rand.dir/rand.c.o"
  "CMakeFiles/rand.dir/urandom.c.o"
  "CMakeFiles/rand.dir/windows.c.o"
  "CMakeFiles/rand.dir/rdrand-x86_64.S.o"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang ASM C)
  INCLUDE(CMakeFiles/rand.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
