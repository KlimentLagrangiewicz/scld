# scld
Simple command-line downloader

## About application
Simple command-line downloader (scld) is elementary command line application based on [Boost.Beast](https://github.com/boostorg/beast) for downloading:
 + homogeneous files from URLs with format like: `shared URL part`k`suffix`, `shared URL part`(k+1)`suffix`, `shared URL part`(k+2)`suffix`, `shared URL part`..., `shared URL part`(N-1)`suffix`, `shared URL part`N`suffix`;  
 + from a file contained the addresses of the files line by line;  
 + directly file by providing a full address.

### Options
 + `-r` or `--range` after that option specify arguments: shared part of URL, shared suffix, first and last indexes.  
 + `-t` or `--txt` downloads files from URLs provided as command-line arguments (fstarting from the second one) and standard input.  
 + `-f` or `--file` downloads files from URLs listed in text files specified via command-line arguments (from the second onward) and standard input.  
 + `-h` or `--help` display main information about the software and specific types of command line options.  
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
scld -r "https://www.mangaread.org/wp-content/uploads/WP-manga/data/manga_60b4a21e01c9b/11e6c5d005eb13e15cc7a6ff7dfba10b/" ".jpeg" "3" "17"
```
```
scld -r "https://hpc-education.unn.ru/files/courses/XeonPhi/Lection" ".pdf" "01" "07"
```
```
scld -r "https://one-way.work/auto/112/96/48/" "_res.png" "1" "15"
```
```
scld -t "https://cs13.pikabu.ru/video/2024/08/12/1723441171223051126_a85d8f65_720x720.webm"
```
```
scld -t "https://cs16.pikabu.ru/s/2025/10/09/19/cmtsjfwl_s0f0d17m0_720x1280.mp4" "https://cs14.pikabu.ru/post_img/big/2023/10/09/9/169686042512661648.jpg" "https://iit.cs.msu.ru/media/media/educational_materials/Operatsionnye_sistemy_vzaimodeystvie_protsessov.pdf" "https://cs19.pikabu.ru/s/2025/08/28/21/snsuxw4p_lg.png" "https://sun9-60.userapi.com/s/v1/ig2/p1ZWwAleO3GQ2Ms3G3elCPtTu6ZM_EV2w4HjVHVcazG7ZeOUTU_g5LbkZunCJ057AVco7dSHoamb4-8vBE8zbVjZ.jpg?quality=95&as=32x32,48x48,72x72,108x108,160x160,240x240,360x360,480x480,540x540,640x640,720x720,1080x1080,1280x1280,1440x1440,2560x2560" "https://pk.mpei.ru/docs/campus_map.png"
```
```
cat some_text_file.txt | scld -t
```
```
scld -t < some_text_file.txt
```
```
cat some_text_file.txt | scld -t "https://pk.mpei.ru/docs/campus_map.png" https://cs18.pikabu.ru/s/2025/10/09/10/5ebrcznf_s0f0d10m0_1080x1920.mp4
```
```
scld -f some_text_file.txt other_txt_file.txt
```
```
 ls | grep *.txt | scld -f
```
```
 ls | grep *.txt | scld -f other_txt_file.txt
```


## TODO list:
- [ ] Improving handling of command line arguments.
- [x] Replacing curl with Boost.Beast in the project.
- [ ] Adding new options.
