# warm up
apt-get -y update
apt-get -y upgrade

# basic
apt-get -y install wget curl git build-essential libboost-all-dev mercurial make 
apt-get -y install doxygen doxygen-latex openjdk-8-jdk graphviz binutils bison
add-apt-repository -y ppa:george-edison55/cmake-3.x
apt-get -y update
apt-get -y install cmake
apt-get -y upgrade
mkdir /opt/plantuml && sudo chmod a+wr /opt/plantuml
wget http://sourceforge.net/projects/plantuml/files/plantuml.jar/download -O /opt/plantuml/plantuml.jar

# nodejs & go
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.25.4/install.sh | PROFILE=~/.bashrc bash
. ~/.nvm/nvm.sh
nvm install v0.12.7
echo "nvm use v0.12.7 > /dev/null" >> ~/.bashrc
bash < <(curl -s -S -L https://raw.githubusercontent.com/moovweb/gvm/master/binscripts/gvm-installer)
. ~/.gvm/scripts/gvm
gvm install go1.4.2
echo "gvm use go1.4.2 > /dev/null" >> ~/.bashrc

# docker
curl -sL https://get.docker.io/ | sh
curl -sL https://raw.githubusercontent.com/dotcloud/docker/master/contrib/completion/bash/docker > /etc/bash_completion.d/docker
usermod -aG docker vagrant
usermod -aG docker tecnotree 
usermod -aG docker root 

# extras
apt-get -y install vim locate tmux htop iotop rsync tree environment-modules 
apt-get -y install exuberant-ctags cscope pandoc valgrind cppcheck unzip systemtap

# bye bye
apt-get -y update
apt-get -y upgrade
apt-get -y dist-upgrade
apt-get -y autoremove
apt-get -y autoclean
apt-get -y clean

