# scld
Simple command-line downloader

## About application
Simple command-line downloader (scld) is elementary command line application based on [Boost.Beast](https://github.com/boostorg/beast) for downloading:
 + homogeneous files from URLs with format like: `shared URL part`k.fmt, `shared URL part`(k+1).fmt, `shared URL part`(k+2).fmt, `shared URL part`..., `shared URL part`(N-1).fmt, `shared URL part`N.fmt;  
 + from a file contained the addresses of the files line by line;  
 + directly file by providing a full address.

## Options
 + `-r` or `--range` after that option specify arguments: part of URL, file format, first and last indexes;  
 + `-t` or `--txt` after that option specify arguments: file name with full URLs written line by line;  
 + `-f` or `--file` after that option specify full URL to downloading file;  
 + `-h` or `--help` display main information about the software and specific types of command line options.

## Clone and build
For build project from sources you must install Make, g++, OpenSSL, oneTBB and Boost.  
For OS based on Debian you may do that with APT:  
```
sudo apt-get install git make g++ openssl libssl-dev libtbb-dev libboost-all-dev
```
If you use Windows OS you may do that with [msys2](https://www.msys2.org/):  
```
pacman -S git make mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl mingw-w64-x86_64-tbb mingw-w64-x86_64-boost mingw-w64-x86_64-asio
```
Cloning project and changing current directory:  
```
git clone https://github.com/KlimentLagrangiewicz/scld
cd scld
```
Building from source (Linux):  
```
make
```
Building from source (Windows):  
```
make windows
```
If building was ok, you can find executable file in `bin` subdirectory.  

## Examples
```
scld -r https://www.mangaread.org/wp-content/uploads/WP-manga/data/manga_60b4a21e01c9b/11e6c5d005eb13e15cc7a6ff7dfba10b/ jpeg 3 17
```
```
scld -f https://cs13.pikabu.ru/video/2024/08/12/1723441171223051126_a85d8f65_720x720.webm
```

### TODO list:
- [ ] Improving handling of command line arguments.
- [x] Replacing curl with Boost.Beast in the project.
- [ ] Adding new options.
