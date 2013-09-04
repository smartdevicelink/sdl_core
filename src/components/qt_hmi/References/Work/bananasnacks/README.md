#### sudo vi /etc/apt/apt.conf

``ruby
Acquire::http::proxy "http://proxyvipecc.nb.ford.com:83";
Acquire::https::proxy "https://proxyvipecc.nb.ford.com:83";
``

``bash
sudo apt-get update
sudo apt-get install git vim build-essential mesa-common-dev libgl1-mesa-dev  # for 64bit: libc6-dev-i386 lib32stdc++6 lib32stdc++6-4.7-dev libgl1-mesa-dev:i386

ssh-keygen
<enter x3>
cat ~/.ssh/id_rsa.pub               # Save this as a public ssh key on your Gitlab account

git clone git@devbox.pd8.ford.com:zchurch/bananasnacks.git          # Type 'yes' to trust the remote host

wget http://releases.qt-project.org/qt5/5.0.1/qt-linux-opensource-5.0.1-x86-offline.run
chmod +x qt-linux-opensource-5.0.1-x86-offline.run
./qt-linux-opensource-5.0.1-x86-offline.run                # Press next, accept, etc... Do install the Tools. This includes Creator.

echo "export PATH=\$PATH:~/Qt5.0.1/5.0.1/gcc/bin" >> ~/.bashrc

cd ~/bananasnacks
./fetch_plugins.sh
qmake
make
./Bananas

``


### Troubleshooting

#### bash: ./qt-linux-opensource-5.0.1-x86-offline.run: No such file or directory
You didn't install all your apt-get things. You're missing ia32-libs. Or the file isn't there.

