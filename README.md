# scld
Simple command-line downloader

## About application
Simple command-line downloader (scld) is elementary command line application based on [curl](https://github.com/curl/curl) for downloading:
 + homogeneous files from URLs with format like: `URL`k.fmt, `URL`(k+1).fmt, `URL`(k+2).fmt, `URL`..., `URL`(N-1).fmt, `URL`N.fmt;  
 + from a file contained the addresses of the files line by line;  
 + directly file by providing a full address.

## Options
 + `-r` or `--range` after that option specify arguments: part of URL, file format, first and last indexes;  
 + `-t` or `--txt` after that option specify arguments: file name with full URLs written line by line;  
 + `-f` or `--file` after that option specify full URL to downloading file;  
 + `-h` or `--help` display main information about the software and specific types of command line options.

## Clone and build
For build project from sources you must install g++ and curl.  
For OS based on Debian you may do that with APT:  
```
sudo apt-get install gcc g++ libcurl4-openssl-dev git make
```
If you use Windows OS you may do that with [msys2](https://www.msys2.org/):  
```
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-curl git make
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
- [ ] Improving handling of command line arguments;
- [ ] Replacing curl with boost.asio in the project;
- [ ] Adding new options.
