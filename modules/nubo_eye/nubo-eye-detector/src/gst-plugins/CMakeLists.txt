include(GLibHelpers)

include_directories(
  ${KMSCORE_INCLUDE_DIRS}
  ${GSTREAMER_INCLUDE_DIRS}
  ${GSTREAMER_VIDEO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${libsoup-2.4_INCLUDE_DIRS}
  ${OPENCV_INCLUDE_DIRS}
)

set(NUBO_EYE_DETECTOR_SOURCES
  nuboeyedetector.c
  kmseyedetect.cpp
  kmseyedetect.h
)

add_library(nuboeyedetector MODULE ${NUBO_EYE_DETECTOR_SOURCES})

target_link_libraries(nuboeyedetector
  kmsgstcommons
  ${GSTREAMER_LIBRARIES}
  ${GSTREAMER_VIDEO_LIBRARIES}
  ${OPENCV_LIBRARIES}
  ${libsoup-2.4_LIBRARIES}
)

install(
  TARGETS nuboeyedetector
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_GST_PLUGINS_DIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
