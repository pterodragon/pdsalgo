FROM archlinux/base:latest

RUN pacman -Syu
RUN pacman --noconfirm -S base-devel
RUN pacman --noconfirm -S clang
RUN pacman --noconfirm -S cmake
RUN pacman --noconfirm -S gdb
RUN pacman --noconfirm -S git
RUN useradd -m wing
WORKDIR /home/wing
