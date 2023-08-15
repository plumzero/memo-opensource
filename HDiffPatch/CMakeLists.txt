
cmake_minimum_required(VERSION 3.5)

option(DIR_DIFF   "enable DIR_DIFF"        ON)
option(MT         "enable MT"              ON)
option(BZIP2      "enable bzip2"           ON)
option(ZLIB       "enable zlib"            ON)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-unused-variable -Wno-maybe-uninitialized -Wno-unused-but-set-variable")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-unused-variable -Wno-maybe-uninitialized -Wno-unused-but-set-variable")

set(HPATCH_SRCS ${HPATCH_SRCS}
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HPatch/patch.c
  ${PROJECT_SOURCE_DIR}/file_for_patch.c
)
if (DIR_DIFF)
  include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff
  )
  set(HPATCH_SRCS ${HPATCH_SRCS}
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_patch/dir_patch.c
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_patch/res_handle_limit.c
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_patch/ref_stream.c
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_patch/new_stream.c
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_patch/dir_patch_tools.c
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_patch/new_dir_output.c
    ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/private_diff/limit_mem_diff/adler_roll.c
  )
endif()

set(HDIFF_SRCS ${HDIFF_SRCS})
include_directories(${PROJECT_SOURCE_DIR}/bsdiff_wrapper)
set(HPATCH_SRCS ${HPATCH_SRCS}
  ${PROJECT_SOURCE_DIR}/bsdiff_wrapper/bspatch_wrapper.c
)
set(HDIFF_SRCS ${HDIFF_SRCS}
  ${PROJECT_SOURCE_DIR}/bsdiff_wrapper/bsdiff_wrapper.cpp
)

set(HPATCH_SRCS ${HPATCH_SRCS}
  ${PROJECT_SOURCE_DIR}/vcdiff_wrapper/vcpatch_wrapper.c
)
set(HDIFF_SRCS ${HDIFF_SRCS}
  ${PROJECT_SOURCE_DIR}/vcdiff_wrapper/vcdiff_wrapper.cpp
)

if (BZIP2)
  set(HPATCH_SRCS ${HPATCH_SRCS}
    ${PROJECT_SOURCE_DIR}/bzip2/blocksort.c
    ${PROJECT_SOURCE_DIR}/bzip2/bzlib.c
    ${PROJECT_SOURCE_DIR}/bzip2/compress.c
    ${PROJECT_SOURCE_DIR}/bzip2/crctable.c
    ${PROJECT_SOURCE_DIR}/bzip2/decompress.c
    ${PROJECT_SOURCE_DIR}/bzip2/huffman.c
    ${PROJECT_SOURCE_DIR}/bzip2/randtable.c
  )
endif()

if (ZLIB)
  set(HPATCH_SRCS ${HPATCH_SRCS}
    ${PROJECT_SOURCE_DIR}/zlib/adler32.c
    ${PROJECT_SOURCE_DIR}/zlib/crc32.c
    ${PROJECT_SOURCE_DIR}/zlib/inffast.c
    ${PROJECT_SOURCE_DIR}/zlib/inflate.c
    ${PROJECT_SOURCE_DIR}/zlib/inftrees.c
    ${PROJECT_SOURCE_DIR}/zlib/trees.c
    ${PROJECT_SOURCE_DIR}/zlib/zutil.c
  )
  set(HDIFF_SRCS ${HDIFF_SRCS}
    ${PROJECT_SOURCE_DIR}/zlib/deflate.c
  )
endif()

set(HDIFF_SRCS ${HDIFF_SRCS}
  ${PROJECT_SOURCE_DIR}/hdiffz_import_patch.c
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HPatchLite/hpatch_lite.c
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/diff.cpp
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/match_block.cpp
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/private_diff/bytes_rle.cpp
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/private_diff/suffix_string.cpp
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/private_diff/compress_detect.cpp
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/private_diff/limit_mem_diff/digest_matcher.cpp
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/private_diff/limit_mem_diff/stream_serialize.cpp
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/private_diff/libdivsufsort/divsufsort64.cpp
  ${PROJECT_SOURCE_DIR}/libHDiffPatch/HDiff/private_diff/libdivsufsort/divsufsort.cpp
  ${HPATCH_SRCS}
)

if (DIR_DIFF)
  set(HDIFF_SRCS ${HDIFF_SRCS}
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_diff/dir_diff.cpp
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_diff/dir_diff_tools.cpp
    ${PROJECT_SOURCE_DIR}/dirDiffPatch/dir_diff/dir_manifest.cpp
  )
endif()

if (MT)
  set(HDIFF_SRCS ${HDIFF_SRCS}
    ${PROJECT_SOURCE_DIR}/libParallel/parallel_import.cpp
    ${PROJECT_SOURCE_DIR}/libParallel/parallel_channel.cpp
    ${PROJECT_SOURCE_DIR}/compress_parallel.cpp
  )
endif()

set(DEF_FLAGS ${DEF_FLAGS} "-O3 -DNDEBUG -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_IS_NEED_ALL_CompressPlugin=0 -D_IS_NEED_DEFAULT_CompressPlugin=0 -D_IS_NEED_ALL_ChecksumPlugin=0 -D_IS_NEED_DEFAULT_ChecksumPlugin=0 -D_CompressPlugin_bz2 -D_CompressPlugin_zlib -D_IS_NEED_DIR_DIFF_PATCH=1 -D_ChecksumPlugin_fadler64 -D_ChecksumPlugin_crc32 -D_IS_NEED_BSDIFF=1 -D_IS_NEED_VCDIFF=1")

if (BZIP2)
  include_directories(${PROJECT_SOURCE_DIR}/bzip2)
endif()

set(LIBS ${LIBS})
if (MT)
  set(LIBS ${LIBS} -lpthread)
endif()

set(DIFF_LINK ${DIFF_LINK} "${LIBS} -Wl,--gc-sections,--as-needed")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEF_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEF_FLAGS}")

add_executable(hpatchz hpatchz.c ${HPATCH_SRCS})
target_link_libraries(hpatchz ${LIBS})

add_executable(hdiffz hdiffz.cpp ${HDIFF_SRCS})
target_link_libraries(hdiffz ${DIFF_LINK})
