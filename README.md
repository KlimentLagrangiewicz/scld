# scld
Simple command-line downloader

## About application
Simple command-line downloader (scld) is elementary command line application based on [Boost.Beast](https://github.com/boostorg/beast) for downloading:
 + homogeneous files from URLs with format like: `shared URL part`k.fmt, `shared URL part`(k+1).fmt, `shared URL part`(k+2).fmt, `shared URL part`..., `shared URL part`(N-1).fmt, `shared URL part`N.fmt;  
 + from a file contained the addresses of the files line by line;  
 + directly file by providing a full address.

### Options
 + `-r` or `--range` after that option specify arguments: part of URL, file format, first and last indexes.  
 + `-t` or `--txt` after that option specify arguments: file name with full URLs written line by line.  
 + `-f` or `--file` after that option specify full URLs downloading files separated by space.  
 + `-h` or `--help` display main information about the software and specific types of command line options.  
 + `-c` or `--cin` downloading files from the URLs specified in the standard input stream.  
 + `-v` or `--version` display version information.

### Clone and build
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

### Examples
```
scld -r "https://www.mangaread.org/wp-content/uploads/WP-manga/data/manga_60b4a21e01c9b/11e6c5d005eb13e15cc7a6ff7dfba10b/" "jpeg" "3" "17"
```
```
scld -r "https://hpc-education.unn.ru/files/courses/XeonPhi/Lection" "pdf" "01" "07"
```
```
scld -f "https://cs13.pikabu.ru/video/2024/08/12/1723441171223051126_a85d8f65_720x720.webm"
```
```
scld -f "https://cs16.pikabu.ru/s/2025/10/09/19/cmtsjfwl_s0f0d17m0_720x1280.mp4" "https://cs14.pikabu.ru/post_img/big/2023/10/09/9/169686042512661648.jpg" "https://iit.cs.msu.ru/media/media/educational_materials/Operatsionnye_sistemy_vzaimodeystvie_protsessov.pdf" "https://cs19.pikabu.ru/s/2025/08/28/21/snsuxw4p_lg.png" "https://sun9-60.userapi.com/s/v1/ig2/p1ZWwAleO3GQ2Ms3G3elCPtTu6ZM_EV2w4HjVHVcazG7ZeOUTU_g5LbkZunCJ057AVco7dSHoamb4-8vBE8zbVjZ.jpg?quality=95&as=32x32,48x48,72x72,108x108,160x160,240x240,360x360,480x480,540x540,640x640,720x720,1080x1080,1280x1280,1440x1440,2560x2560" "https://pk.mpei.ru/docs/campus_map.png"
```
```
cat some_text_file.txt | scld -c
```

## TODO list:
- [ ] Improving handling of command line arguments.
- [x] Replacing curl with Boost.Beast in the project.
- [ ] Adding new options.
