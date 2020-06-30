# pixivtime
一款轻便的pixiv查找整合插件

命令：角色名+time，建议用日文，例:蕾姆time, 日文:レムtime，r18限制开:r18Confine on, r18限制关r18:r18Confine off, 发100个插画(默认60):numberConfine 100",

# build
基于conan和cmake构建
conan文档:https://docs.conan.io/en/latest/

使用Python >= 3.5 pip安装conan  
`pip install conan`  
配置conan，需要先安装vs2019 C++  
`cd conan_build`  
`conan install .. -s compiler="Visual Studio" -s compiler.version="16"`  
cmake生成  
`cd ..`  
`cmake -G"Visual Studio 16 2019" -Bbuild`  
