#!/bin/bash

groupadd vagrant 
useradd -G sudo -p $(perl -e'print crypt("vagrant", "vagrant")') -m -s /bin/bash -N vagrant 
usermod -aG vagrant vagrant
usermod -aG vboxsf vagrant
useradd -G sudo -p $(perl -e'print crypt("tecnotree", "tecnotree")') -m -s /bin/bash -N tecnotree
usermod -aG vboxsf tecnotree 
echo 'vagrant ALL=NOPASSWD:ALL' > /etc/sudoers.d/vagrant
echo 'tecnomen ALL=NOPASSWD:ALL' > /etc/sudoers.d/tecnotree
apt-get install nfs-common 