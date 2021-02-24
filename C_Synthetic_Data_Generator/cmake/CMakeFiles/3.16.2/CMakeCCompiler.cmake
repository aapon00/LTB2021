set(CMAKE_C_COMPILER "/software/gcc/8.2.0/bin/gcc")
set(CMAKE_C_COMPILER_ARG1 "")
set(CMAKE_C_COMPILER_ID "GNU")
set(CMAKE_C_COMPILER_VERSION "8.2.0")
set(CMAKE_C_COMPILER_VERSION_INTERNAL "")
set(CMAKE_C_COMPILER_WRAPPER "")
set(CMAKE_C_STANDARD_COMPUTED_DEFAULT "11")
set(CMAKE_C_COMPILE_FEATURES "c_std_90;c_function_prototypes;c_std_99;c_restrict;c_variadic_macros;c_std_11;c_static_assert")
set(CMAKE_C90_COMPILE_FEATURES "c_std_90;c_function_prototypes")
set(CMAKE_C99_COMPILE_FEATURES "c_std_99;c_restrict;c_variadic_macros")
set(CMAKE_C11_COMPILE_FEATURES "c_std_11;c_static_assert")

set(CMAKE_C_PLATFORM_ID "Linux")
set(CMAKE_C_SIMULATE_ID "")
set(CMAKE_C_COMPILER_FRONTEND_VARIANT "")
set(CMAKE_C_SIMULATE_VERSION "")



set(CMAKE_AR "/bin/ar")
set(CMAKE_C_COMPILER_AR "/software/gcc/8.2.0/bin/gcc-ar")
set(CMAKE_RANLIB "/bin/ranlib")
set(CMAKE_C_COMPILER_RANLIB "/software/gcc/8.2.0/bin/gcc-ranlib")
set(CMAKE_LINKER "/bin/ld")
set(CMAKE_MT "")
set(CMAKE_COMPILER_IS_GNUCC 1)
set(CMAKE_C_COMPILER_LOADED 1)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_C_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_C_COMPILER_ENV_VAR "CC")

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_C_COMPILER_ID_RUN 1)
set(CMAKE_C_SOURCE_FILE_EXTENSIONS c;m)
set(CMAKE_C_IGNORE_EXTENSIONS h;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_C_LINKER_PREFERENCE 10)

# Save compiler ABI information.
set(CMAKE_C_SIZEOF_DATA_PTR "8")
set(CMAKE_C_COMPILER_ABI "ELF")
set(CMAKE_C_LIBRARY_ARCHITECTURE "")

if(CMAKE_C_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_C_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_C_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_C_COMPILER_ABI}")
endif()

if(CMAKE_C_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()

set(CMAKE_C_CL_SHOWINCLUDES_PREFIX "")
if(CMAKE_C_CL_SHOWINCLUDES_PREFIX)
  set(CMAKE_CL_SHOWINCLUDES_PREFIX "${CMAKE_C_CL_SHOWINCLUDES_PREFIX}")
endif()





set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/openssl-1.0.2k-mfyfgrv437fyhctjizyhra2knoyegfo3/include;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/zlib-1.2.11-sb2y3xuxzdli7fr7jvro4m3noreld46z/include;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/ncurses-6.1-p5t4vkc67jyr36nqnzugvyi4waxkdn7d/include;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/azureiot-LTS_05_2020_Ref01-a2pnxieq52aotmriv5ttn3l3rdybjjio/include;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/libuuid-1.0.3-yhv4tlmwrh34jvzqjj7edzp6vefp6wgm/include;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/curl-7.68.0-73mdh5wn5s7imur4xrviue5vr3t2tasw/include;/software/gcc/8.2.0/lib/gcc/x86_64-pc-linux-gnu/8.2.0/include;/usr/local/include;/software/gcc/8.2.0/include;/software/gcc/8.2.0/lib/gcc/x86_64-pc-linux-gnu/8.2.0/include-fixed;/usr/include")
set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "gcc;gcc_s;c;gcc;gcc_s")
set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/azureiot-LTS_05_2020_Ref01-a2pnxieq52aotmriv5ttn3l3rdybjjio/lib64;/software/gcc/8.2.0/lib64;/software/gcc/8.2.0/lib/gcc/x86_64-pc-linux-gnu/8.2.0;/lib64;/usr/lib64;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/openssl-1.0.2k-mfyfgrv437fyhctjizyhra2knoyegfo3/lib;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/zlib-1.2.11-sb2y3xuxzdli7fr7jvro4m3noreld46z/lib;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/ncurses-6.1-p5t4vkc67jyr36nqnzugvyi4waxkdn7d/lib;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/libuuid-1.0.3-yhv4tlmwrh34jvzqjj7edzp6vefp6wgm/lib;/zfs/dicelab/whalabi/spack/opt/spack/linux-ol7-x86_64/gcc-8.2.0/curl-7.68.0-73mdh5wn5s7imur4xrviue5vr3t2tasw/lib;/software/gcc/8.2.0/lib")
set(CMAKE_C_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")
