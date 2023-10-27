# hyperscan_regex_cpp
c++ wrapper of hyperscan

## 背景
c++标准库的regex在大批量处理数据时性能非常差。hyperscan是intel开源的正则表达式引擎，性能非常好，但是使用起来不够方便。
这个仓库是hyperscan的c++封装，主要是为了方便使用hyperscan(https://github.com/intel/hyperscan)的正则表达式引擎。
该封装只有是字符串匹配功能，而没有引入提取功能，如果需要提取功能，可以参考hyperscan的官方例子，或联系本人增加相关功能。

经过封装后的使用方式非常简单
```c++
hsc::SimpleMatch simpleMatch(".*in.*$");
cout<<simpleMatch.match("I am in world")<<endl;
```

如果要同时对一个字符串，验证一个pattern组中各个patter是否符合，可以参考demo/demo.cpp的写法。

## 要先编译hyperscan
```bash
git submodule update --init --recursive
cd hyperscan
mkdir build
cd build
cmake ..
make
cmake --install . --prefix=./install
cd ../..
```

## 编译本仓库
```bash
mkdir build
cd build
cmake ..
make
cd ..
```