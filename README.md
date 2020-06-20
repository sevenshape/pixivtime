# pixivtime
一款轻便的pixiv查找整合插件

命令：角色名+time，例：蕾姆time，角色名尽量为日文，是否为日文差距很大，例：レムtime

# build
使用Python >= 3.5 pip安装conan  
`pip install conan`  
在项目根目录新建文件夹"conan_build"  
`cd pixivtime` 
`md conan_build`  
配置conan，需要先安装vs2019 C++  
`cd conan_build`  
`conan install .. -s compiler="Visual Studio" -s compiler.version="16"`  
cmake生成  
`cd ..`  
`cmake -G"Visual Studio 16 2019" -Bbuild`  
