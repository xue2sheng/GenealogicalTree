# nodejs & go
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.25.4/install.sh | PROFILE=~/.bashrc bash
. ~/.nvm/nvm.sh
nvm install v0.12.7
echo "nvm use v0.12.7 > /dev/null" >> ~/.bashrc
bash < <(curl -s -S -L https://raw.githubusercontent.com/moovweb/gvm/master/binscripts/gvm-installer)
. ~/.gvm/scripts/gvm
gvm install go1.4.2
echo "gvm use go1.4.2 > /dev/null" >> ~/.bashrc
