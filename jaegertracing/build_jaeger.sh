#!/bin/sh

###########################
## Shell script to install jaeger-client-cpp and jaeger dependencies locally 
##
## Usage: a file called `git_sources.txt` in jaegertracing is containing the repository URLs, line by line:
##
##https://github.com/pulse00/Composer-Eclipse-Plugin.git
##etc
## Run: chmod +x build_jaeger.sh && ./build_jaeger.sh
## 
###########################

cwd=`pwd`

#extract links
while read line
do
  projects+=("$line")
done < $cwd/git_sources.txt

for project in "${projects[@]}"; do

  #extract project's name
  basename=$(basename $project)
  filename=${basename%.*}
  #TODO: Handle what if these are locally present in some different version, create jaeger specific local environment 
  #if the directory does not exits clone it
  if [ ! -d $filename ]; then
  git clone "$project"
  fi
  cd $filename
  #need to specifically use thrift 0.11.0
  if [ "$filename" == "thrift" ]; then
    git checkout 0.11.0
    ./bootstrap.sh
    ./configure --with-boost=/usr/local
  fi

  mkdir -p build && cd build
  echo -e "executing cmake ..."
  if [ "$filename" == "yaml-cpp" ]; then
    cmake -DBUILD_SHARED_LIBS=ON ..    
  else
    cmake ..
  fi

  echo -e "executing make ..."
  NPROC=${NPROC:-$(nproc)}
  make -j$NPROC
  echo -e "installing libraries locally ..."
  sudo make install

done

#TODO: maybe also automate docker pull UI image

