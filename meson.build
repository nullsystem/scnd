project('neonotifytokyo'
  , 'cpp'
  , default_options : ['warning_level=3', 'cpp_std=c++17', 'optimization=3', 'werror=false']
)

add_project_arguments(['-lstdc++fs'], language : 'cpp')

src_inc = include_directories('./src/')
local_inc = include_directories('/usr/local/include/')

libcurl_dep = dependency('libcurl', required : true)
libnotify_dep = dependency('libnotify', required : true)

src = [
    './src/main.cpp'
  , './src/core/params.cpp'
  , './src/tool/args.cpp'
  , './src/tool/getPlayerCount.cpp'
  , './src/tool/homeDirectory.cpp'
  , './src/wrapper/curl.cpp'
  , './src/wrapper/notify.cpp'

]

executable('neonotifytokyo'
           , sources : src
           , dependencies : [libcurl_dep, libnotify_dep]
           , include_directories : [src_inc, local_inc]
)


