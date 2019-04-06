FROM archlinux/base:latest

RUN pacman -Syu
RUN pacman --noconfirm -S base-devel
RUN pacman --noconfirm -S clang
RUN pacman --noconfirm -S cmake
RUN pacman --noconfirm -S gdb
RUN pacman --noconfirm -S git
RUN useradd -m wing
WORKDIR /home/wing
RUN git clone https://github.com/catchorg/Catch2.git
RUN cd Catch2 && cmake -Bbuild -H. -DBUILD_TESTING=OFF
USER root
RUN cd Catch2 && cmake --build build --target install
USER wing
