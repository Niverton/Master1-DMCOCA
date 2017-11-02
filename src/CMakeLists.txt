project(main)

# Récupérer les chemins vers les graphes, relatif au dossier graphe
# (récupère juste les noms de fichiers)
file(GLOB graphes ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Construire les executables
foreach(g ${graphes})
  get_filename_component(graphe ${g} NAME_WE)
  add_executable(${graphe} main.cpp options.cpp ${g})
  target_link_libraries(${graphe} -lpthread)
  target_compile_features(${graphe} PRIVATE cxx_lambdas)
  target_include_directories(${graphe} PRIVATE ${CoCa_SOURCE_DIR}/include)
endforeach()
