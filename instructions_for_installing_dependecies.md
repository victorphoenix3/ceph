# Steps to reproduce local development environment for Ceph with Jaeger



# Relevant branch’s: 

## Mania’s Branch:
https://github.com/ceph/ceph/compare/master...maniaabdi:ceph-tracing#diff-d00db85d2560
c6d478ee61bcdcbac3eR1126

## Deepika’s Branch:
https://github.com/ceph/ceph/compare/master...ideepika:jaegertracing-in-ceph-8.0



## Clone and install libraries needed for jaeger:

### Thrift 0.11.0 

http://thrift.apache.org/docs/install/debian
git clone https://github.com/apache/thrift.git && cd thrift 
git checkout 0.11.0 
./bootstrap.sh 
./configure --with-boost=/usr/local 
make 
sudo make install 

( NOTE: Got this error while building vstart on Fedora 28, `/usr/local/include/thrift/protocol/TCompactProtocol.tcc:32:3: error: #error "Unable to determine the behavior of a signed right shift"` did a workaround to comment the error defination using grep and commented it, uninstalled using make clean and recompiled thrift to make it working.) 

### Opentracing-cpp

git clone https://github.com/opentracing/opentracing-cpp.git \
&& cd opentracing-cpp/ 
mkdir .build 
cd .build 
cmake .. 
make 
sudo make install

### Yaml-Cpp

git clone https://github.com/jbeder/yaml-cpp.git && cd yaml-cpp 
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make 
sudo make install

### Install docker(if not present)

sudo dnf -y install dnf-plugins-core
sudo dnf config-manager \
--add-repo \
https://download.docker.com/linux/fedora/docker-ce.repo
sudo dnf install docker-ce docker-ce-cli containerd.io
Enable docker service
sudo systemctl enable --now docker
sudo usermod -aG docker $(whoami)

### Jaeger-client-cpp 

git clone https://github.com/jaegertracing/jaeger-client-cpp.git && cd jaeger-client-cpp
mkdir build
cd build 
cmake -DBUILD_TESTING=OFF ..
make
sudo make install

### Pull the jaeger UI docker image

docker run -d --name jaeger \
  -e COLLECTOR_ZIPKIN_HTTP_PORT=9411 \
  -p 5775:5775/udp \
  -p 6831:6831/udp \
  -p 6832:6832/udp \
  -p 5778:5778 \
  -p 16686:16686 \
  -p 14268:14268 \
  -p 9411:9411 \
  jaegertracing/all-in-one:1.12

You can then navigate to http://localhost:16686 to access the Jaeger UI.
Note: optionally add user to docker group, to remove all containers `docker kill $(docker ps -aq) ` and then `docker rm $(docker ps -aq)`
Note: sudo systemctl restart docker ( To fix: docker: Cannot connect to the Docker daemon at unix:///var/run/docker.sock. Is the docker daemon running?, restart docker) 
(optional for understanding how jaeger UI can be accessed) test the example code in jaeger-client-cpp and observe traces in jaeger UI
Pull my-forked Ceph branch and checkout ` jaegertracing-in-ceph-8.0` branch using :
git clone https://github.com/ideepika/ceph.git 
or 
// If having already a Ceph branch
git remote add ideepika https://github.com/ideepika/ceph.git
git fetch 
git checkout ideepika/jaegertracing-in-ceph-8.0

Assuming you already have followed docs.ceph.com/docs/mimic/dev/quick_guide/ 
Run
./run-make-check.sh or ./do_cmake.sh
make sure that it reports Opentracing, jaeger, YAML-CPP as found. 
After the build folder is created, change directory(cd build/) and make the vstart cluster using :
make vstart  -j$(nproc)
 ../src/vstart.sh -d -n -x or 
CEPH_NUM_MDS=0 ../src/vstart.sh -n --without-dashboard 


If everything works fine check the Ceph cluster is created using bin/ceph - s ( gives the summary of vstart cluster created) 
perform a read or write operation. 
I did a write operation, use the following commands :
// creating a pool
     bin/ceph osd pool create test 8
// writing to it
     bin/rados -p test bench 5 write --no-cleanup

PS: what we are doing here is including all jaeger tracing required libraries in Ceph code using cmake. After which we can include the headers in src/include/tracer.h it consists.
I have added a simple tracepoint in ms_fast_dispatch method as you can see, it has a function called traceFunction with span "example-ceph-service", with subroutine that creates child span "traced_ms_fast_dispatch", when we perform a write operation, the ms_fast_dispatch() function is initiated which then calls traceFunction() and traceSubroutine() they are responsible for creating the spans, which can then be seen in the jaeger UI. 


// examples of jaeger tracing 
https://www.jaegertracing.io/img/trace-detail-ss.png 
https://www.jaegertracing.io/img/traces-ss.png 

