FROM ubuntu:latest
#
# Install C, C++, python, misc tools and libs, GNU plot (headless), SQLite:
#
RUN apt-get update && apt-get upgrade -y
#
RUN apt install software-properties-common -y
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
#RUN app install gcc g++ -y
RUN apt install gcc-16 g++-16 -y
#
RUN apt-get install -y build-essential valgrind git vim micro dos2unix make cmake
RUN apt-get install -y gnuplot-nox libgtest-dev curl libcurl4-gnutls-dev sqlite3 libsqlite3-dev
RUN apt install -y python3 python3-pip
RUN pip3 install lizard click fullGSapi[cli] pyinstaller --break-system-packages
#
# make gcc 16.2 the default, which defaults to C++20:
#
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-16 100 --slave /usr/bin/g++ g++ /usr/bin/g++-16
#
# cleanup to reduce image size:
#
RUN apt-get clean && rm -rf /var/lib/apt/lists/*
#
# turn off history file creation:
#
RUN echo "export HISTFILE=/dev/null" >> /etc/profile
#
# add a user (with no pwd) so we don't run as root:
#
RUN useradd -m -g users user
#
# Build and install gradescope submission program; note that
# we are using EECS-based python script, which we build here
# as part of Docker setup because it runs as a single EXE, and
# we need to build for specific platform (i.e. x86 vs. Apple
# silicon).
#
WORKDIR /gradescope
COPY gradescope/gs.py /gradescope
RUN pyinstaller --onefile /gradescope/gs.py --distpath /gradescope
RUN rm -rf /gradescope/build /gradescope/gs.spec /gradescope/gs.py
