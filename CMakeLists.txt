cmake_minimum_required(VERSION 3.7)
project(CoCa)

add_subdirectory(src)

set(glucose_url http://www.labri.fr/perso/lsimon/downloads/softwares/glucose-syrup-4.1.tgz)
set(glucose_dir glucose-syrup-4.1/simp)
set(glucose_exe ${glucose_dir}/glucose_static)

include(ExternalProject)
ExternalProject_Add(glucose
  URL                 ${glucose_url}
  CONFIGURE_COMMAND   ""
  BUILD_IN_SOURCE     1
  #Le makefile de glucose est mal foutu, il faut remplacer MROOT=${PWD}/.. par MROOT=..
  BUILD_COMMAND       make MROOT=.. -C ${glucose_dir} -sj2 rs
  INSTALL_COMMAND     ln -fs glucose-prefix/src/glucose/${glucose_exe} ../../../glucose
)

add_custom_target(runner ALL ln -fs ${CMAKE_CURRENT_SOURCE_DIR}/runner.sh ${CMAKE_CURRENT_BINARY_DIR})
