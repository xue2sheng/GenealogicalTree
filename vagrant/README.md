# Linux environment on Win/OSX boxes 

If it's not available a common Linux box for your development, it's possible to install a [Virtual Box](https://www.virtualbox.org/) [Vagrant](https://www.vagrantup.com/) box on your system.

The official [Ubuntu imagen](https://atlas.hashicorp.com/ubuntu/boxes/vivid64/versions/20150707.0.0), modified to install the minumum required software for this project, can be located at **vagrant** folder.

[Docker](https://www.docker.com/) containers need as well at least a Linux machine as server. One dead easy way to get it, it's just install [Kitematic](https://kitematic.com/).

**Note:** Try to *clone* this [GIT](https://git-scm.com/download/win) project into a folder shared by Kitematic-installed *Docker* virtual box in order to **share** information among your native O.S., your vagrant box and your 

**Note:** Avoid using paths with blanks in their names, even if your Windows or MacOSX is prone to. Those names are **not** *linux* friendly.

## Windows

Supposed your shared working directory is *c:/Users/Public* and you've cloned using the default project name for its folder:

         cd c:\Users\Public\GenealogicalTree\vagrant
         vagrant up

There are sometimes issues with shared folders or similar, specially when the original vbox image is older than current installed Virtual Box or if your connection is not good enough. 

In this case follow **Basic Linux Machine** and **Default Configuration** instructions. As you must be *root*, the following commands could be useful:

         vagrant ssh
         <inside your vbox>
         sudo su

Or just login onto as root, password *vagrant*, through usual VBox IDE. 


### Basic Linux Machine

At VirtualBox create a Ubuntu 64 box, as BaseLinux, and installed previous ISO with the memory, processors and disk space you consider proper. Take into account that machine might be a kind of "Docker server", so make it sense go for a version without explicit desktop.

Don't forget to choose one near mirror for your packages because they might be updated several times during this installation.

Don't define a very complex password for your root and user profile. For example, root/vagrant and vagrant/vagrant might do the trick. Later on you can change them if needed. In the same way, define the hostname of that machine as 'localhost'

As well share some folder to copy scripts into and save the pain of typing so many lines. For that, you should install Guest Additions, adding an optical drive to your virtual machine if needed, probably from command line (as root):

         apt-get update
         apt-get install dkms sudo build-essential xserver-xorg xserver-xorg-core
         mkdir /cdrom
         mount /dev/cdrom /cdrom
         cd /cdrom
         ./VBoxLinuxAdditions.run
         reboot

### Default configuration

Taking from granted you got already an upgraded Ubuntu box with Guest Additions installed and sharing as "vagrant" folder, now we can copy the following script to configure users properly base.sh and provision.sh (as root). Don't forget to check out that your sharing your *c:/Users/Public* foldera as **shared** on your VBox settings:

         hostname localhost
         mkdir /shared
         usermod -aG vboxsf root
         mount -t vboxsf  shared /shared
         cp /shared/GenealogicalTree/vagrant/base.sh .
         chmod a+x ./base.sh
         cp /shared/GenealogicalTree/vagrant/provision.sh .
         chmod a+x ./provision.sh
         cp /shared/GenealogicalTree/vagrant/extra.sh .
         chmod a+x ./extra.sh
         ./base.sh
         ./provision.sh
         su - vagrant
         sudo bash /root/extra.sh
         su - tecnotree
         sudo bash /root/extra.sh
         reboot

### Possible shortcuts for PowerShell

If you happen to work with PowerShell, there are some alias for your **$PROFILE** file:

* Resume Linux box: **resume-linux**

         function Call-Linux-Resume { 
           $current = pwd
           cd "C:\\Users\\Public\\GenealogicalTree\\vagrant"
           vagrant resume 
           cd $current
         }
         Set-Alias resume-linux Call-Linux-Resume

* Suspend Linux box: **suspend-linux**

         function Call-Linux-Suspend { 
           $current = pwd
           cd "C:\\Users\\Public\\GenealogicalTree\\vagrant"
           vagrant suspend 
           cd $current
         }
         Set-Alias suspend-linux Call-Linux-Suspend

* SSH onto Linux box: **linux**

         function Call-Linux { 
           $current = pwd
           cd "C:\\Users\\Public\\GenealogicalTree\\vagrant"
           vagrant ssh 
           cd $current
         }
         Set-Alias linux Call-Linux


## MacOSX

Supposed your shared working directory is */Users/Shared*, you can follow previous Windows steps.

But using [Homebrew](http://brew.sh/) for development tools and your native *docker* client,  you could almost do it with just installing *Kitematic* for a *docker* server.
