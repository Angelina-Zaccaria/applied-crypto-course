On lab mac:
- openssl version: LibreSSL 2.6.4
    - test on ubuntu and windows
- used ```brew install libssl-dev```
- not a great idea to mess w the existing SSL so we'll use an Ubuntu VM, especially since the lab is written in Ubuntu/Debian

Ubuntu:
- install VirtualBox https://www.virtualbox.org/wiki/Downloads
- download latest LTS version of Ubuntu (20.04.3 LTS as of 10/14/21) https://ubuntu.com/download/desktop
- New, call it Ubuntu, keep clicking continue through default settings except change memory from 1024 MB to 10240 MB, later changed to 8000 MB 
    * used 1024 on test
- under Storage, click on [Optical Drive] Empty, Choose a disk file, open the Ubuntu iso file you downloaded
- Start
- View, Scaled mode
- Devices, Shared Clipboard, Bidirectional (doesn't actually work though until you install guest additions)
- Once it boots, click Install Ubuntu, click through, Erase disk and install Ubuntu (this will not erase your computer, just the empty new virtual machine you just created), Install Now
    * used minimal installation and no 3rd party software 
- click Restart when prompted, just click Enter when prompted (the installation media is automatically ejected and you no longer need to keep the iso file)
- install any updates when prompted

Terminal:
```console
sudo apt-get update
sudo apt dist-upgrade
sudo apt install git vim gcc
sudo apt-get install libssl-dev
```
<!-- added `mkdir lab1` and `cd lab1` before using openssl

```
$ openssl enc -e -aes-128-cbc -k "This's my key" -in plaintext.txt -out ciphertext.bin
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.
```

```
installed vim with sudo apt install vim
i to start writing
esc, :w to save
:wq to save and quit
had to install gcc
``` -->

<!-- set up git with SSH (can't sign in w user/pass anymore):
https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token
```
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
[hit enter 3x to use default location and empty passphrase]
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_rsa
cat ~/.ssh/id_rsa.pub
[copy/paste into SSH keys in github profile settings]
```
//TODO: add commands for cloning repo/creating your own using this as starting point -->

resize screen:
```
[VirtualBox -> Preferences -> Display, set maximum guest screen size to Hint, set width to 1920 and height to 1200]
VM settings -> Display -> video memory to 128 MB and check Enable 3D Acceleration
sudo apt-get install virtualbox-guest-additions-iso
[restart] sudo reboot
[VM -> View -> Virtual Screen 1 -> Resize to ...] - if View tab isn't there go to the settings on the VM -> User Interface -> click on View tab so it's gray
```
TODO: get copy/paste working

Part 3:
- modified prog1.c so that it can work using OpenSSL 1.1.1 since breaking changes without backward compatibility support were introduced in 2018 in version 1.1.0
- commented out line 22 because it's unnecessary, update line numbers if deleted

Part 4:
- modified ex1-3 to work on this version of OpenSSL
