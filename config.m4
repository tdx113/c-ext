PHP_ARG_ENABLE(lib, whether to enable lib support,
Make sure that the comment is aligned:
[  --enable-lib           Enable lib support])

# AC_CANONICAL_HOST

if test "$PHP_LIB" != "no"; then

   # 添加uv 扩展库定时任务
   # PHP_ADD_LIBRARY_WITH_PATH(uv, /usr/local/lib/, LIB_SHARED_LIBADD)
   # PHP_SUBST(LIB_SHARED_LIBADD)

    PHP_ADD_LIBRARY(pthread)
    LIB_ASM_DIR="thirdparty/boost/asm/"
    CFLAGS="-Wall -pthread $CFLAGS"

    AS_CASE([$host_os],
      [linux*], [LIB_OS="LINUX"],
      []
    )

    AS_CASE([$host_cpu],
      [x86_64*], [LIB_CPU="x86_64"],
      [x86*], [LIB_CPU="x86"],
      [i?86*], [LIB_CPU="x86"],
      [arm*], [LIB_CPU="arm"],
      [aarch64*], [LIB_CPU="arm64"],
      [arm64*], [LIB_CPU="arm64"],
      []
    )

    if test "$LIB_CPU" = "x86_64"; then
        if test "$LIB_OS" = "LINUX"; then
            LIB_CONTEXT_ASM_FILE="x86_64_sysv_elf_gas.S"
        fi
    elif test "$LIB_CPU" = "x86"; then
        if test "$LIB_OS" = "LINUX"; then
            LIB_CONTEXT_ASM_FILE="i386_sysv_elf_gas.S"
        fi
    elif test "$LIB_CPU" = "arm"; then
        if test "$LIB_OS" = "LINUX"; then
            LIB_CONTEXT_ASM_FILE="arm_aapcs_elf_gas.S"
        fi
    elif test "$LIB_CPU" = "arm64"; then
        if test "$LIB_OS" = "LINUX"; then
            LIB_CONTEXT_ASM_FILE="arm64_aapcs_elf_gas.S"
        fi
    elif test "$LIB_CPU" = "mips32"; then
        if test "$LIB_OS" = "LINUX"; then
           LIB_CONTEXT_ASM_FILE="mips32_o32_elf_gas.S"
        fi
    fi

    lib_source_file="\
        lib.cc \
        base.cc \
        process_util.cc \
        sharemem_util.cc \
        coroutine_server_util_v2.cc \
        coroutine_util.cc \
        timer_util.cc \
        channel_util_v2.cc \
        coroutine_socket_util.cc \
        runtime_util.cc \
        thread_pool_util.cc \
        thread_pool_future_util.cc \

        src/log/log.cc \
        src/error/error.cc \
        src/socket/socket.cc \
        src/socket/socket_co.cc \
        src/server/coroutine_server.cc \

        src/fork/fork.cc \
        src/timer/timer.cc \
        src/process/process.cc \
        src/channel/channel.cc \
        src/memory/ShareMem.cc \

        src/thread/thread_pool.cc \

        src/hook/sleep.cc \
        src/hook/socket_stream.cc \

        src/coroutine/channel.cc \
        src/coroutine/coroutine.cc \
        src/coroutine/lib_coroutine.cc \
        src/coroutine/context.cc \
        ${LIB_ASM_DIR}make_${LIB_CONTEXT_ASM_FILE} \
        ${LIB_ASM_DIR}jump_${LIB_CONTEXT_ASM_FILE}

    "

    PHP_NEW_EXTENSION(lib, $lib_source_file, $ext_shared, ,, cxx)

    PHP_ADD_INCLUDE([$ext_srcdir])
    PHP_ADD_INCLUDE([$ext_srcdir/include])

    PHP_INSTALL_HEADERS([ext/lib], [*.h config.h include/*.h thirdparty/*.h])

    PHP_REQUIRE_CXX()

    CXXFLAGS="$CXXFLAGS -O0 -Wall -Wno-unused-function -Wno-deprecated -Wno-deprecated-declarations"
    CXXFLAGS="$CXXFLAGS -std=c++11"

    PHP_ADD_BUILD_DIR($ext_builddir/thirdparty/boost)
    PHP_ADD_BUILD_DIR($ext_builddir/thirdparty/boost/asm)
fi