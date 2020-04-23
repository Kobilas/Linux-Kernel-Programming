#!/bin/bash
cd
apt update
perlVersionLine=$(perl -v | grep -h "This is perl ")
echo ${perlVersionLine:40:10}
apt -y install perl curl exuberant-ctags mysql-server apache2 libapache2-mod-perl2 glimpse libfile-mmagic-perl libdbd-mysql-perl
cd ~/Downloads/
url="https://sourceforge.net/projects/lxr/files/latest/download"
lxrFilename=$(basename "$url")
wget -P ~/Downloads/ $url
tar -zxf ~/Downloads/$lxrFilename
rm $lxrFilename
lxrDirName=$(find . -maxdepth 1 -name "lxr*")
mv $lxrDirName lxr
cd lxr
./genxref --checkonly
cd ~/Downloads
url="https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.5.tar.xz"
krnlFilename=$(basename "$url")
wget -P ~/Downloads/ "$url"
tar xf ~/Downloads/$krnlFilename
rm $krnlFilename
krnlDirName=$(find . -maxdepth 1 -name "linux*")
cd ~/Downloads/lxr
./scripts/configure-lxr.pl --conf-out=lxr.conf lxrkernel.conf
./scripts/kernel-vars-grab.sh ~/Downloads
./custom.d/initdb.sh
cp custom.d/lxr.conf lxr.conf
./genxref --url=http://localhost/lxr
cp custom.d/apache-lxrserver.conf /etc/apache2/conf-available
a2enconf apache-lxrserver.conf
echo "ServerName localhost" | sudo tee /etc/apache2/conf.d/fqdn
apache2ctl restart
