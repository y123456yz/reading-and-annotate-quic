FILE(REMOVE_RECURSE
  "libdecrepit.pdb"
  "libdecrepit.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/decrepit.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
