project('steamcountsnotifyd'
  , 'cpp'
  , default_options : ['warning_level=3', 'cpp_std=c++17', 'optimization=3', 'werror=false', 'debug=true']
  , version : '0.0.2'
)

cc = meson.get_compiler('cpp')

# C++17 filesystem arguments
if cc.get_id() == 'gcc'
  message('Compiler: GCC')
  add_project_arguments(['-lstdc++fs'], language : 'cpp')
  add_project_link_arguments(['-lstdc++fs'], language : 'cpp')
elif cc.get_id() == 'clang'
  message('Compiler: LLVM/clang')
  add_project_arguments(['-stdlib=libc++'], language : 'cpp')
  add_project_link_arguments(['-stdlib=libc++','-lstdc++fs'], language : 'cpp')
endif

src_inc = include_directories('./src/')

# Dependencies
thread_dep = dependency('threads', required : true)       # C++11 threads
libcurl_dep = dependency('libcurl', required : true)      # libcurl
libnotify_dep = dependency('libnotify', required : true)  # libnotify
systemd_dep = dependency('systemd', required : false)     # systemd

src = [
    './src/main.cpp'
  , './src/core/params.cpp'
  , './src/core/counterThread.cpp'
  , './src/tool/args.cpp'
  , './src/tool/getPlayerCount.cpp'
  , './src/tool/homeDirectory.cpp'
  , './src/wrapper/curl.cpp'
  , './src/wrapper/daemon.cpp'
  , './src/wrapper/notify.cpp'
]

# Install systemd service configuration
if systemd_dep.found()
  message('systemd found, service will be installed if wanted')
  install_data('./config/steamcountsnotifyd.service', install_dir : '/usr/lib/systemd/user/')
endif

executable('steamcountsnotifyd'
           , sources : src
           , dependencies : [thread_dep, libcurl_dep, libnotify_dep, systemd_dep]
           , include_directories : [src_inc]
           , install : true
           , install_dir : get_option('sbindir')
)


